# coding=utf-8
import requests  # send requests
from bs4 import BeautifulSoup
import json
import sys
import re
reload(sys)
sys.setdefaultencoding('utf-8')

def Spyder_url(url_in):   # url_in is a txt including all the url achieved in the last step.
    f = open(url_in, 'r')
    cnt = 0
    temp_list = []
    url_list = f.readlines()
    for item in url_list:
        url = item.replace('\n', '').strip()
        temp_list.append(url)
    # store the output URL in a txt.
    f_out = open('url_output.txt', 'a')
    for item in url_list:
        url = item.replace('\n','').strip()      # url is the url you will use in the following steps.
        res = requests.get(url)
        soup = BeautifulSoup(res.text, 'html.parser')

        template = re.compile('http://5sing.kugou.com/yc/')
        t1 = soup.find_all('a')
        for i in t1:
            href = str(i.get('href'))
            if (template.match(href) and  href not in temp_list):
                new_url = href.replace('#listenContent','')
                temp_list.append(new_url)
    for url_ in temp_list:
        f_out.write(url_ + '\n')
        cnt += 1
    f_out.close()
    print cnt
    f.close()



def Spyder_info(url_in):   # url_in is a txt including all the url achieved in the last step.
    f = open(url_in, 'r')
    url_list = f.readlines()
    for item in url_list:
        url = item.replace('\n','').strip()      # url is the url you will use in the following steps.
        res = requests.get(url)
        soup = BeautifulSoup(res.text, 'html.parser')

        # store current song's info in a txt.
        f_info = open('song_info.txt', 'a')
        musicTitle = soup.select('.view_tit')[0].text  # 歌曲名称
        musicInfo = soup.select('.view_box')[0].select('li')
        list_info = []
        for info in range(0, 8):
            list_info.append(musicInfo[info].text)

        data = {}
        title =['Language', 'Arranger', 'Writer', 'Singer', 'Classification', 'Style', 'Composer', 'Mixer']
        data['MusicName'] = musicTitle
        for i in range(0, 8):
            tmp = str(list_info[i]).split('：')
            data[title[i]] = tmp[1].decode()

        # download and collect
        # download = str(musicInfo[10].text)     # is 0 ,not know why.
        # download_num = download.replace('下载','')
        # data['Download'] = download_num
        # collect = musicInfo[12].text          # is 0 ,not know why.
        # collect_ = (str(collect).split('\n'))[1]
        # collect_num = collect_.replace('收藏','')
        # data['Collect'] = collect_num
        json_str = str(json.dumps(data, ensure_ascii=False))
        f_info.write(json_str + '\n')
        f_info.close()
    f.close()



# main
Spyder_url('URLlist.txt')
Spyder_info('URLlist.txt')
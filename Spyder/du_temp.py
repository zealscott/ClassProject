# coding=utf-8
from bs4 import BeautifulSoup
import requests  # send request
import re
import os
import datetime
import sys

reload(sys)
sys.setdefaultencoding('utf-8')
probeginTime = datetime.datetime.now()  # 程序开始时间

RootPath = os.getcwd()


def Is_yc(url):          # Judge whether the url is a yc song's url.
    pattern = re.compile(r'http://5sing.kugou.com/yc/\d')     # \d follows with numbers.
    if pattern.search(url):
        return True
    else:
        return False


def Is_existed(level, url):         # Judge whether the url is already existed.
    for i in range(1, level + 2):       # level+2 to ensure that we also examine the url in current level.
        filename = "WebGet_" + str(i) + ".txt"
        # the file may not exit
        if os.path.exists(RootPath + "\\" + filename):
            f = open(filename, "r")
            data = f.readlines()
            for line in data:
                line = line.replace("\n", "")
                if line == url:
                    f.close()
                    return True
            f.close()
    return False


def write_url(level, url):       # write the url in current level's output txt if it is not existed.
    outFile = "WebGet_" + str(level + 1) + ".txt"
    if Is_existed(level, url) is True:
        #        print "%s is exit"%(url)
        return 0
    f_out = open(outFile, "a")
    f_out.write(url)
    f_out.write("\n")
    f_out.close()
    return 1


def write_songInfo(level, url):    # if url is match yc, then download its info.
    if level == 1:         # the first page
        return 0
    if check_info(url) == 0:
        print "the %s contain no songinfo!" % (url)
        return 0
    res = requests.get(url)
    soup = BeautifulSoup(res.text, 'html.parser')
    songView = soup.find_all("div", class_="view")[0]
    title = songView.find_all("div", class_="view_tit")[0].text
    lt = songView.find_all("ul", class_="lt mb15")[0].text
    eng_info = {u"演唱": "Singer", u"作词": "Writer", u"作曲": "Composer", u"编曲": "Arranger",
                u"混缩": "Mixer", u"分类": "Classification", u"语种": "Language", u"曲风": "Style"}
    info = "{MusicName:" + '"' + title + '",'
    count = 0
    for i in lt.split("\n"):
        if i == '':
            continue
        if count == 8:
            break
        item = i.split(u"：")
        info = info + eng_info[item[0]].decode("utf-8") + ":" + '"' + (item[1].strip()).decode("utf-8") + '",'
        count += 1
    info = info[:-1]     # delete the last ','
    info = info + "}"
    outFile = "songinfo_" + str(level) + ".txt"
    #    put info into txt.
    f_out = open(outFile, "a")
    f_out.write(info)
    f_out.write("\n")
    f_out.close()
    return 1


def check_info(url):  # avoid link error and invalid url.
    try:
        res = requests.get(url)
        soup = BeautifulSoup(res.text, 'html.parser')
        if len(soup.find_all("div", class_="view")) == 0:
            return 0
        songView = soup.find_all("div", class_="view")[0]
        if len(songView.find_all("div", class_="view_tit")) == 0:
            return 0
        if len(songView.find_all("ul", class_="lt mb15")) == 0:
            return 0
    except:
        print "%s error" % (url)
    else:
        return 1


def spyder(level):
    inFile = "WebGet_" + str(level) + ".txt"
    count_url = 0
    count_song = 0
    f_in = open(inFile, "r")
    data = f_in.readlines()
    for url in data:
        url = url.replace("\n", "")
        try:
            res = requests.get(url)
            soup = BeautifulSoup(res.text, 'html.parser')
            if Is_yc(url) is True:
                count_song += write_songInfo(level, url)
            tag_a = soup.find_all("a", href=True)
            pattern = re.compile(r"http://5sing.kugou.com/")
            for i in tag_a:
                get_url = i["href"]
                if pattern.match(get_url):
                    # if the url is exit, return 0, else ,return 1
                    count_url += write_url(level, get_url)
            s = requests.session()
            s.keep_alive = False
        except:
            print res
            print "connect error!"
    f_in.close()
    return count_url, count_song


for i in range(1, 8):
    count = spyder(i)
    print "level %d has spyder %d urls, has songInfo %d" % (i, count[0], count[1])

endtime = datetime.datetime.now()

proendTime = datetime.datetime.now()  # 结束时间
print("running time："), proendTime - probeginTime

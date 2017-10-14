# -*- coding: UTF-8 -*-
import requests
import re
import json
from bs4 import BeautifulSoup
import sys
reload(sys)
sys.setdefaultencoding('utf8')
url_list=[]

def judge(url):    #正则表达式判断是否是原唱歌曲
    res = requests.get(url)
    soup = BeautifulSoup(res.text, 'html.parser')
    if re.match("http://5sing.kugou.com/yc/\d+.html", url):
        if len(soup.find_all("div", class_="view")) != 0:
            return True
    return False

def get_all(url):            #获取网页全部超链接
    res = requests.get(url)  #url是母网页
    #print res.text
    url_list=[]
    soup = BeautifulSoup(res.text, 'html.parser')
    tag_a_list = soup.find_all("a", href=True)
    f = open("f:\level1.txt", "a")
    for i in tag_a_list:
        nw = i.get("href", "null")
        if judge(nw):
            url_list.append(nw)
    url_list=list(set(url_list))  #去重
    for i in url_list:
        f.write(i)

def get_message(url):
    res = requests.get(url)
    soup = BeautifulSoup(res.text, 'html.parser')
    # print soup
    musicTitle = soup.select('.view_tit')[0].text  # 歌曲名称
    musicInfo = soup.select('.view_box')[0].select('li')
    list = []
    for item in range(0, 8):
        list.append(musicInfo[item].text)
    data = {}
    data['歌曲名称'] = musicTitle
    for item in list:
        tmp = str(item).split(":")
        data[tmp[0]] = tmp[1].decode()
    json_str = str(json.dumps(data, ensure_ascii=False))
    f=open("message.txt","a")
    f.write(json_str + '\n')
    print json_str

#f=open("f:\level1.txt","r")
#f_url=f.readlines()
#for data in f_url:
   # data=data.replace("\n","")
    #if(judge(data)==False):continue
data="http://5sing.kugou.com/yc/3117296.html"
get_message(data)

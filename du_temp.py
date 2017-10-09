# coding=utf-8
from bs4 import BeautifulSoup
import requests  # send request
import Queue
import re

q = Queue.Queue()

url='http://5sing.kugou.com/subject/nzpd/index'




web=[]
res=requests.get(url)
soup=BeautifulSoup(res.text,'html.parser')
yc=soup.find_all("div",class_="yft-content yc-content clearfix")
#root=soup.find_all("div",class_="yft-content fc-content clearfix")
root=yc[0].find_all("div",class_="part gt-part")
tag_a=root[0].find_all("div",class_="song-name")
for i in tag_a:
    a = i.find_all("a")[0]
    print a["href"]
    web.append(a["href"])


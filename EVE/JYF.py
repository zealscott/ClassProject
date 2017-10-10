from lxml import etree
import requests
import re
import sys
headers= { 'User-Agent' : 'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36(KHTML,like Gecko) Chrome/49.0.2623.211 Safari/537.36 SE 2.X MetaSr 1.0' }
page_url="http://5sing.kugou.com/subject/nzpd/index"
html = requests.get(page_url,headers=headers).content
selector = etree.HTML(html)
links = selector.xpath("//div[@class='song-name']/a/@href")
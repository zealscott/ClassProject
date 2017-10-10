# coding=utf-8
from bs4 import BeautifulSoup
import requests  # send request
import Queue
import re
import os
import datetime

starttime = datetime.datetime.now()



RootPath = os.getcwd()


# q = Queue.Queue()


def Is_yc(url):
    pattern = re.compile(r'http://5sing.kugou.com/yc')
    if pattern.search(url):
        return True
    else:
        return False


def exitURL(level, url):
    for i in range(1, level + 2):
        filename = "WebGet_" + str(level) + ".txt"
        f = open(filename, "r")
        data = f.readlines()
        for line in data:
            line=line.replace("\n","")
            if line == url:
                return True
    return False


def write_url(level, RootPath, url):
    outFile = "WebGet_" + str(level + 1) + ".txt"
    if exitURL(level, url) is True:
        # print "url is exit"
        return 0
    f_out = open(outFile, "a")
    f_out.write(url)
    f_out.write("\n")
    f_out.close()
    return 1


def get_yc_url(level, RootPath):
    # open source text
    inFile = "WebGet_" + str(level) + ".txt"
    count = 0
    f_in = open(inFile, "r")
    data = f_in.readlines()
    for url in data:
        if level != 1:
            # if the url isn't a yc
            if Is_yc(url) is False:
                # print "isn't a yc"
                continue
        url = url.replace("\n", "")
        res = requests.get(url)
        soup = BeautifulSoup(res.text, 'html.parser')
        tag_a = soup.find_all("a", href=True)
        for i in tag_a:
            get_url = i["href"]
            pattern = re.compile(r'http://5sing.kugou.com')
            if pattern.match(get_url):
                # if the url is exit, return 0, else ,return 1
                count += write_url(level, RootPath, get_url)
    f_in.close()
    return count


for i in range(1, 7):
    number = get_yc_url(i, RootPath)
    print "the level %d has %d urls" % (i+1, number)



endtime = datetime.datetime.now()

print (endtime - starttime).seconds
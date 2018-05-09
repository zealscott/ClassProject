import numpy as np
import numpy as np
from numpy import *
import pandas as pd
import random
import nltk
nltk.download()
from nltk.corpus import wordnet as wn
from nltk.corpus import sentiwordnet as swn

def read_dic():
    pos = open("positive-words.txt","r")
    neg  = open("negative-words.txt","r")
    possentence = pos.readlines()
    negsentence = neg.readlines()
    posdic = []
    for line in possentence:
        posdic.append(line.split('\n')[0])
    negdic = []
    for line in possentence:
        negdic.append(line.split('\n')[0])
    return posdic,negdic

def get_acc(predict, target):
    totle = len(predict)
    trueCount = 0
    for i in range(totle):
        if predict[i] == target[i]:
            trueCount += 1
    return float(trueCount) / float(totle)

def single_review_sentiment_score(seg_sent):
    i = 0  # 记录扫描到的词的位置
    s = 0  # 记录情感词的位置
    poscount = 0  # 记录该分句中的积极情感得分
    negcount = 0  # 记录该分句中的消极情感得分

    for word in seg_sent:   # 逐词分析
        if word in posdict:  # 如果是积极情感词
            # print("posword:", word)
            poscount += 1   # 积极得分+1
            # print("poscount:", poscount)

        elif word in negdict:  # 如果是消极情感词
            # print("negword:", word)
            negcount += 1
            # print("negcount:", negcount)
        #print "poscount,negcount", poscount, negcount
    result = poscount - negcount   # 该条微博情感的最终得分
    result = round(result, 1)
    return result

def getSenti(word):
    return swn.senti_synset(word.name())



# posdict,negdict = read_dic()


# fProcess = open("process.txt","r")
# trainsentence = fProcess.readlines()
#
#
# label = pd.read_csv("marker.txt")
# label = label.values
# label = label[:,0]

swninfo = getSenti("know")
p1 = p1 + swninfo.pos_score()

# #将词拆分
# data = []
# for w in trainsentence:
#     line = w.split("\n")[0]
#     temp = line.split(" ")
#     data.append(temp)
# # running
# score = []
# for context in data:
#     result = single_review_sentiment_score(context)
#     score.append(result)
#
# s = get_acc(score,label)
# print(s)
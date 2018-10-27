
# coding: utf-8
import numpy as np
import pandas as pd
import random
from sklearn.model_selection import StratifiedKFold
from sklearn.feature_extraction.text import TfidfTransformer
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.linear_model.logistic import LogisticRegression
from sklearn.feature_extraction.text import TfidfVectorizer as TFIDF

def to_matrix(all):
    tfidf = TFIDF(min_df=3,  # 最小支持度为3
                  max_features=None,
                  strip_accents='unicode',
                  analyzer='word',
                  token_pattern=r'\w{1,}',
                  ngram_range=(1, 4),  # 二元文法模型
                  use_idf=1,
                  smooth_idf=1,
                  sublinear_tf=1,
                  stop_words='english')  # 去掉英文停用词
    tfidf.fit(all)
    data_all = tfidf.transform(all)
    return data_all



# min_df=5,ngram_range=(1, 5),0.8854984495843554


# min_df=7,ngram_range=(1, 4),0.8861234498284961




def get_acc(predict, target):
    totle = len(predict)
    trueCount = 0
    for i in range(totle):
        if predict[i] == target[i]:
            trueCount += 1
    return float(trueCount) / float(totle)



# 构造数据
fProcess = open("process.txt","r")
s = fProcess.readlines()
sentence = []
for ss in s:
    sentence.append(ss.split('\n')[0])


ftest = open("testprocess.txt","r")
t = ftest.readlines()
for tt in t:
    sentence.append(tt.split('\n')[0])


RAWdata = to_matrix(sentence)


print("RAWdata",RAWdata.shape)

label = pd.read_csv("marker.txt")
label = label.values
label = label[:,0]

Z = RAWdata[-5000:]

print("Z",Z.shape)

data = RAWdata[:-5000]

print("data",data.shape)


# 4000个测试数据， 其余验证数据+训练数据

indexList = list(range(0, data.shape[0]))
random.seed(1)
random.shuffle(indexList)

# 取前16000个为训练集，后4000个为测试集
testIndex = indexList[-4000:]
trainIndex = indexList[:-4000]

# 得到训练数据和验证数据集
trainData = data[trainIndex]
trainLabel = label[trainIndex]

testData = data[testIndex]
testLabel = label[testIndex]


skf = StratifiedKFold(n_splits=10)

clf = LogisticRegression()

accList = []
for train, validation in skf.split(trainData, trainLabel):
    clf.fit(trainData[train], trainLabel[train])
    labelPredict = clf.predict(trainData[validation])
    accList.append(get_acc(labelPredict, trainLabel[validation]))

averageACC = sum(accList) / len(accList)

print(averageACC)


# 测试数据
test_target_value = clf.predict(Z)
result_df = pd.DataFrame(columns=['markers'])
result_df['markers'] = test_target_value.astype(int)
result_df.loc[(result_df.markers == 1),'markers'] = 'positive'
result_df.loc[(result_df.markers == 0),'markers'] = 'negative'
result_df.to_csv('MyPredict7.txt', index=False)




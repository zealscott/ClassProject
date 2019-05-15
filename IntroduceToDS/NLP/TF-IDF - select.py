
# coding: utf-8
import numpy as np
import pandas as pd
import random
from sklearn.model_selection import StratifiedKFold
from sklearn.feature_extraction.text import TfidfTransformer
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.linear_model.logistic import LogisticRegression

def to_matrix(all):
    vectorizer = CountVectorizer()  # 该类会将文本中的词语转换为词频矩阵，矩阵元素a[i][j] 表示j词在i类文本下的词频
    transformer = TfidfTransformer()  # 该类会统计每个词语的tf-idf权值
    tfidf = transformer.fit_transform(vectorizer.fit_transform(all))  # 第一个fit_transform是计算tf-idf，第二个fit_transform是将文本转为词频矩阵
    word = vectorizer.get_feature_names()  # 获取词袋模型中的所有词语
    weight = tfidf.toarray()  # 将tf-idf矩阵抽取出来，元素a[i][j]表示j词在i类文本中的tf-idf权重
    return weight


def get_acc(predict, target):
    totle = len(predict)
    trueCount = 0
    for i in range(totle):
        if predict[i] == target[i]:
            trueCount += 1
    return float(trueCount) / float(totle)


# 构造数据

fProcess = open("process.txt","r")
sentence = fProcess.readlines()

ftest = open("testprocess.txt","r")
testsentence = ftest.readlines()

sentence.extend(testsentence)

RAWdata = to_matrix(sentence)

label = pd.read_csv("marker.txt")
label = label.values
label = label[:,0]

#--------------------------------------
# print("RAWdata",RAWdata.shape)
#
from sklearn.svm import LinearSVC
from sklearn.feature_selection import SelectFromModel
lsvc = LinearSVC(C=0.01, penalty="l1", dual=False).fit(RAWdata[:-5000], label)
model = SelectFromModel(lsvc, prefit=True)
RAWdata = model.transform(RAWdata)

print("RAWdata-new",RAWdata.shape)

#-------------------------------------
Z = RAWdata[-5000:]

# print("Z",Z.shape)

data = RAWdata[:-5000]

# print("data",Z.shape)



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
# test_target_value = clf.predict(Z)
# result_df = pd.DataFrame(columns=['markers'])
# result_df['markers'] = test_target_value.astype(int)
# result_df.loc[(result_df.markers == 1),'markers'] = 'positive'
# result_df.loc[(result_df.markers == 0),'markers'] = 'negative'
# result_df.to_csv('MyPredict2.txt', index=False)
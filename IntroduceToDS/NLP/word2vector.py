
# coding: utf-8
import numpy as np
from numpy import *
import pandas as pd
import random
from gensim.models import Word2Vec
from sklearn.model_selection import StratifiedKFold
from sklearn.feature_extraction.text import TfidfTransformer
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.linear_model.logistic import LogisticRegression

SIZE = 800
def get_vector(sentence):
    sentences = []
    for review in sentence:
        line = review.split("\n")[0]
        temp = line.split(" ")
        sentences.append(temp)
    # train model
    model = Word2Vec(sentences, min_count=1, workers=4,
                     size=SIZE,window=10,iter=10,sg=1)
    return model

def get_acc(predict, target):
    totle = len(predict)
    trueCount = 0
    for i in range(totle):
        if predict[i] == target[i]:
            trueCount += 1
    return float(trueCount) / float(totle)

fProcess = open("process.txt","r")
trainsentence = fProcess.readlines()
ftest = open("testprocess.txt","r")
testsentence = ftest.readlines()

sentence = trainsentence.copy()
sentence.extend(testsentence)
#得到模型
model = get_vector(sentence)

#将词拆分
data = []
for w in trainsentence:
    line = w.split("\n")[0]
    temp = line.split(" ")
    data.append(temp)

test = []
for w in testsentence:
    line = w.split("\n")[0]
    temp = line.split(" ")
    test.append(temp)


#得到每个句子的向量
data_vector = zeros([1,SIZE])
for line in data:
    line_vector = zeros([1,SIZE])
    for word in line:
        line_vector  = line_vector + model[word]
    # print(line_vector.shape)
    data_vector = np.row_stack((data_vector,line_vector))
data_vector = np.delete(data_vector,0,0)

test_vector = zeros([1,SIZE])
for line in test:
    line_vector = zeros([1,SIZE])
    for word in line:
        line_vector  = line_vector + model[word]
    # print(line_vector.shape)
    test_vector = np.row_stack((test_vector,line_vector))
test_vector = np.delete(test_vector,0,0)

label = pd.read_csv("marker.txt")
label = label.values
label = label[:,0]

# 4000个测试数据， 其余验证数据+训练数据

indexList = list(range(0, data_vector.shape[0]))
random.seed(1)
random.shuffle(indexList)

# 取前16000个为训练集，后4000个为测试集
testIndex = indexList[-4000:]
trainIndex = indexList[:-4000]

# 得到训练数据和验证数据集
trainData = data_vector[trainIndex]
trainLabel = label[trainIndex]

testData = data_vector[testIndex]
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
#0.852748598754
# size = 400 ,0.859873834448
# size = 600 0.877436299462
# 0.877187198364


# 测试数据
test_target_value = clf.predict(test_vector)
result_df = pd.DataFrame(columns=['markers'])
result_df['markers'] = test_target_value.astype(int)
result_df.loc[(result_df.markers == 1),'markers'] = 'positive'
result_df.loc[(result_df.markers == 0),'markers'] = 'negative'
result_df.to_csv('MyPredict3.txt', index=False)
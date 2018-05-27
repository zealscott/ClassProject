# classifier
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import StratifiedKFold
import numpy as np

import random
from gensim.models import Doc2Vec


class GetData(object):
    def __init__(self, ModelName, pos_prefix, neg_prefix, pos_num, neg_num, test_prefix, test_num):
        """
        get the file path and data size 
        the pos_num should equal neg_num
        """
        self.ModelName = ModelName
        self.pos_prefix = pos_prefix
        self.neg_prefix = neg_prefix
        self.pos_num = pos_num
        self.neg_num = neg_num
        self.test_num = test_num
        self.test_prefix = test_prefix

    def GetModel(self):
        """
        return model  
        """
        self.model = Doc2Vec.load(self.ModelName)
        return self.model

    def GetArray(self):
        """
        return [train_arrays,train_labels,test_arrays]  
        """
        train_arrays = np.zeros(
            (self.pos_num+self.neg_num, self.model.vector_size))
        train_labels = np.zeros(self.pos_num+self.neg_num)
        for i in range(self.pos_num):
            prefix_train_pos = self.pos_prefix + str(i)
            prefix_train_neg = self.neg_prefix + str(i)
            train_arrays[i] = model[prefix_train_pos]
            train_arrays[self.pos_num + i] = model[prefix_train_neg]
            train_labels[i] = 1
            train_labels[self.pos_num + i] = 0

        test_arrays = np.zeros((self.test_num, self.model.vector_size))
        for i in range(self.test_num):
            prefix_test = self.test_prefix + str(i)
            test_arrays[i] = model[prefix_test]

        return train_arrays, train_labels, test_arrays


def get_acc(predict, target):
    totle = len(predict)
    trueCount = 0
    for i in range(totle):
        if predict[i] == target[i]:
            trueCount += 1
    return float(trueCount) / float(totle)

def myhash(obj):
    return hash(obj)%(2**32)

data = GetData("./Model/imdb.d2v", "TRAIN_POS_",
               "TRAIN_NEG_", 12500, 12500, "TEST_", 25000)

model = data.GetModel()

data, label, test = data.GetArray()

print("data size:",data.shape)
print("label size:",label.shape)
print("test size:",test.shape)

# 随机取验证数据+训练数据

indexList = list(range(0, data.shape[0]))
random.seed(1)
random.shuffle(indexList)

# 取前20000个为训练集，后5000个为测试集
testIndex = indexList[-5000:]
trainIndex = indexList[:-5000]

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
# result_df.to_csv('MyPredict7.txt', index=False)
import numpy as np
import random
from collections import Counter
from sklearn.datasets import load_iris
from sklearn.model_selection import StratifiedKFold
import matplotlib.pyplot as plt

iris = load_iris()

# 30个测试数据， 其余验证数据+训练数据
indexList = list(range(0, iris.data.shape[0]))
random.seed(1)
random.shuffle(indexList)

# 取前120个为训练集，后30个为测试集
testIndex = indexList[-30:]
trainIndex = indexList[:-30]

# 构造数据
data = iris.data
label = iris.target

tvData = data[trainIndex]
tvLabel = label[trainIndex]

testData = data[testIndex]
testLabel = label[testIndex]

# 交叉验证
skf = StratifiedKFold(n_splits=10)


def get_acc(predict, target):
    totle = len(predict)
    trueCount = 0
    for i in range(totle):
        if predict[i] == target[i]:
            trueCount += 1
    return float(trueCount) / float(totle)


def KNN(testData, valData, testLabel, k):
    result = []
    for simple in valData:
        # 对验证数据中的每个值求距离
        distant = (np.sum((simple - testData) ** 2, axis=1)) ** 0.5
        index = np.argsort(distant)[0:k]
        neighbor = testLabel[index]
        c = Counter(neighbor)
        getclass = c.most_common(1)[0][0]
        result.append(getclass)
    return result


avg = []
# 选择k值
factor = list(range(1, 40))
for k in factor:
    accList = []
    for train, validation in skf.split(tvData, tvLabel):
        labelPredict = KNN(tvData[train], tvData[validation], tvLabel[train], k)
        accList.append(get_acc(labelPredict, tvLabel[validation]))
    averageACC = sum(accList) / len(accList)
    avg.append(averageACC)

k = np.argsort(avg)[-1] + 1
print("The proper k factor is %d" % (k))

testPredict = KNN(tvData, testData, tvLabel, k)
testACC = get_acc(testPredict, testLabel)
print("测试集准确率：" + str(testACC))

plt.plot(factor, avg, color="red")
plt.title("The Acc according to K factor", fontsize=18)
plt.xlabel("K", fontsize=15)
plt.ylabel("Precision", fontsize=15)
plt.show()

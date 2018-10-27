import random

import numpy as np
from sklearn import datasets
from sklearn.linear_model import LogisticRegression
from matplotlib import colors as c


def get_acc(predict,target):
    totle = len(predict)
    trueCount = 0
    for i in range(totle):
        if predict[i] == target[i]:
            trueCount += 1
    return  float(trueCount) / float(totle)


def draw_bonderay(x_max,x_min,y_max,y_min,model):
    cMap = c.ListedColormap(['#c7dcfd', '#b0ea96', '#ffffba'])
    h = .02
    x_max = x_max + .5
    x_min = x_min - .5
    y_max = y_max + .5
    y_min = y_min - .5

    # x_min, x_max = X[:, 0].min() - .5, X[:, 0].max() + .5
    # y_min, y_max = X[:, 1].min() - .5, X[:, 1].max() + .5

    print(x_min)
    print(x_max)

    xx, yy = np.meshgrid(np.arange(x_min, x_max, h), np.arange(y_min, y_max, h))
    # print(xx)
    # print(yy)
    # Z = logreg.predict(np.c_[xx.ravel(), yy.ravel()])
    Z = model.predict(np.c_[xx.ravel(), yy.ravel()])
    Z = Z.reshape(xx.shape)
    plt.pcolormesh(xx, yy, Z, cmap=cMap)



# 加载数据集
iris =datasets.load_iris()
print(iris.target_names)
print(iris.feature_names)
print(iris.data)
print(iris.target)
#
# print(iris.data.shape)  #150个样本   4个特征

#分割训练集，测试集
# 前120个训练集，后30个测试集

index = list(range(len(iris.data)))
# print(index)
random.seed(1)
random.shuffle(index)
# print(index)
trainIndex =index[:120]
testIndex = index[120:]




x_train = iris.data[trainIndex,:]
y_train = iris.target[trainIndex]

x_test = iris.data[testIndex,:]
y_test = iris.target[testIndex]


import matplotlib.pyplot as plt
data = iris.data


from sklearn import tree
clf = tree.DecisionTreeClassifier()




#决策树模型第0个特正
feature = [0,1]
clf.fit(x_train[:,feature],y_train)
draw_bonderay(data[:,0].max(),data[:,0].min(),data[:,1].max(),data[:,1].min(),clf)
plt.scatter(data[:50,0],data[:50,1],color="red",marker="o",label="setosa")
plt.scatter(data[50:100,0],data[50:100,1],color="blue",marker="x",label="versicolor")
plt.scatter(data[100:,0],data[100:,1],color="green",marker="+",label="virginica")
plt.xlabel("sepal length(cm)")
plt.ylabel("sepal width(cm)")
plt.legend()
answer = clf.predict(x_test[:,feature])
acc = get_acc(answer,y_test)
plt.title("TestSet Acc:" + str(int(acc *100)) + "%")
plt.show()


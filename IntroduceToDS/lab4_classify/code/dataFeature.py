import random

import numpy as np
from sklearn import datasets
from matplotlib import colors as c

# 加载数据集
iris =datasets.load_iris()
# print(iris.target_names)
# print(iris.feature_names)
# print(iris.data)
# print(iris.target)
#
# print(iris.data.shape)  #150个样本   4个特征

#分割训练集，测试集
# 前120个训练集，后30个测试集

index = list(range(len(iris.data)))
# print(index)
# 将其随机打乱
random.seed(1)
random.shuffle(index)
# print(index)
# 取前120个作为训练集，剩下的作为测试集
trainIndex =index[:120]
testIndex = index[120:]
#
#
#
x_train = iris.data[trainIndex,:]
y_train = iris.target[trainIndex]
#
x_test = iris.data[testIndex,:]
y_test = iris.target[testIndex]
#
#
import matplotlib.pyplot as plt
data = iris.data


#0,1
plt.subplot(2,3,1)
plt.scatter(data[:50,0],data[:50,1],color="red",marker="o",label="setosa")
plt.scatter(data[50:100,0],data[50:100,1],color="blue",marker="x",label="versicolor")
plt.scatter(data[100:,0],data[100:,1],color="green",marker="+",label="virginica")
plt.xlabel("sepal length(cm)")
plt.ylabel("sepal width(cm)")
plt.legend()
# plt.show()




#0,2
plt.subplot(2,3,2)
plt.scatter(data[:50,0],data[:50,2],color="red",marker="o",label="setosa")
plt.scatter(data[50:100,0],data[50:100,2],color="blue",marker="x",label="versicolor")
plt.scatter(data[100:,0],data[100:,2],color="green",marker="+",label="virginica")
plt.xlabel("sepal length(cm)")
plt.ylabel("petal length (cm)")
plt.legend()
# plt.show()




#0,3
plt.subplot(2,3,3)
plt.scatter(data[:50,0],data[:50,3],color="red",marker="o",label="setosa")
plt.scatter(data[50:100,0],data[50:100,3],color="blue",marker="x",label="versicolor")
plt.scatter(data[100:,0],data[100:,3],color="green",marker="+",label="virginica")
plt.xlabel("sepal length(cm)")
plt.ylabel("petal width (cm)")
plt.legend()
# plt.show()


#1,2
plt.subplot(2,3,4)
plt.scatter(data[:50,1],data[:50,2],color="red",marker="o",label="setosa")
plt.scatter(data[50:100,1],data[50:100,2],color="blue",marker="x",label="versicolor")
plt.scatter(data[100:,1],data[100:,2],color="green",marker="+",label="virginica")
plt.xlabel("sepal width (cm)")
plt.ylabel("petal length (cm)")
plt.legend()
# plt.show()




#1,3
plt.subplot(2,3,5)
plt.scatter(data[:50,1],data[:50,3],color="red",marker="o",label="setosa")
plt.scatter(data[50:100,1],data[50:100,3],color="blue",marker="x",label="versicolor")
plt.scatter(data[100:,1],data[100:,3],color="green",marker="+",label="virginica")
plt.xlabel("sepal width (cm)")
plt.ylabel("petal width (cm)")
plt.legend()
# plt.show()




#2,3
plt.subplot(2,3,6)
plt.scatter(data[:50,2],data[:50,3],color="red",marker="o",label="setosa")
plt.scatter(data[50:100,2],data[50:100,3],color="blue",marker="x",label="versicolor")
plt.scatter(data[100:,2],data[100:,3],color="green",marker="+",label="virginica")
plt.xlabel("petal length (cm)")
plt.ylabel("petal width (cm)")
plt.legend()
plt.show()

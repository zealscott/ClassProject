import random
import numpy as np
import matplotlib.pyplot as plt
from sklearn import datasets
from sklearn.linear_model.logistic import LogisticRegression
from matplotlib import colors as c


def get_acc(predict, target):
    totle = len(predict)
    trueCount = 0
    for i in range(totle):
        if predict[i] == target[i]:
            trueCount += 1
    return float(trueCount) / float(totle)


def draw_bonderay(x_max, x_min, y_max, y_min, model):
    cMap = c.ListedColormap(['#c7dcfd', '#b0ea96', '#ffffba'])
    h = .01
    x_max = x_max + .5
    x_min = x_min - .5
    y_max = y_max + .5
    y_min = y_min - .5

    # x_min, x_max = X[:, 0].min() - .5, X[:, 0].max() + .5
    # y_min, y_max = X[:, 1].min() - .5, X[:, 1].max() + .5

    print(x_min)
    print(x_max)
    # 使用arange创建等差数组(h)，meshgrid生成二维网格(像素点）
    # xx：特征a
    # yy：特征b
    xx, yy = np.meshgrid(np.arange(x_min, x_max, h), np.arange(y_min, y_max, h))
    # print(xx)
    # print(yy)
    # np.c_:  Translates slice objects to concatenation along the second axis.
    Z = model.predict(np.c_[xx.ravel(), yy.ravel()])
    Z = Z.reshape(xx.shape)
    # 使用预测结果Z来进行选择像素点该填什么颜色
    plt.pcolormesh(xx, yy, Z, cmap=cMap)


# 加载数据集
iris = datasets.load_iris()
data = iris.data
# 分割训练集，测试集
# 前120个训练集，后30个测试集
index = list(range(len(iris.data)))
random.seed(1)
random.shuffle(index)
trainIndex = index[:120]
testIndex = index[120:]

x_train = iris.data[trainIndex, :]
y_train = iris.target[trainIndex]

x_test = iris.data[testIndex, :]
y_test = iris.target[testIndex]

clf = LogisticRegression()

# 大图的标题
plt.suptitle('LogisticRegression Training', fontsize=30)
plt.subplots_adjust(wspace=0.2, hspace=0.4)
# 0,1
plt.subplot(2, 3, 1)
feature = [0, 1]
clf.fit(x_train[:, feature], y_train)
draw_bonderay(data[:, 0].max(), data[:, 0].min(), data[:, 1].max(), data[:, 1].min(), clf)
plt.scatter(data[:50, 0], data[:50, 1], color="red", marker="o", label="setosa")
plt.scatter(data[50:100, 0], data[50:100, 1], color="blue", marker="x", label="versicolor")
plt.scatter(data[100:, 0], data[100:, 1], color="green", marker="+", label="virginica")
plt.xlabel("sepal length (cm)")
plt.ylabel("sepal width (cm)")
plt.legend()
answer = clf.predict(x_test[:, feature])
acc = get_acc(answer, y_test)
plt.title("10153903105 Acc:" + str(int(acc * 100)) + "%")

# 0,2
plt.subplot(2, 3, 2)
feature = [0, 2]
clf.fit(x_train[:, feature], y_train)
draw_bonderay(data[:, 0].max(), data[:, 0].min(), data[:, 2].max(), data[:, 2].min(), clf)
plt.scatter(data[:50, 0], data[:50, 2], color="red", marker="o", label="setosa")
plt.scatter(data[50:100, 0], data[50:100, 2], color="blue", marker="x", label="versicolor")
plt.scatter(data[100:, 0], data[100:, 2], color="green", marker="+", label="virginica")
plt.xlabel("sepal length (cm)")
plt.ylabel("petal length (cm)")
plt.legend()
answer = clf.predict(x_test[:, feature])
acc = get_acc(answer, y_test)
plt.title("10153903105 Acc:" + str(int(acc * 100)) + "%")

# 0,3
plt.subplot(2, 3, 3)
feature = [0, 3]
clf.fit(x_train[:, feature], y_train)
draw_bonderay(data[:, 0].max(), data[:, 0].min(), data[:, 3].max(), data[:, 3].min(), clf)
plt.scatter(data[:50, 0], data[:50, 3], color="red", marker="o", label="setosa")
plt.scatter(data[50:100, 0], data[50:100, 3], color="blue", marker="x", label="versicolor")
plt.scatter(data[100:, 0], data[100:, 3], color="green", marker="+", label="virginica")
plt.xlabel("sepal length (cm)")
plt.ylabel("petal width (cm)")
plt.legend()
answer = clf.predict(x_test[:, feature])
acc = get_acc(answer, y_test)
plt.title("10153903105 Acc:" + str(int(acc * 100)) + "%")

# 1,2
plt.subplot(2, 3, 4)
feature = [1, 2]
clf.fit(x_train[:, feature], y_train)
draw_bonderay(data[:, 1].max(), data[:, 1].min(), data[:, 2].max(), data[:, 2].min(), clf)
plt.scatter(data[:50, 1], data[:50, 2], color="red", marker="o", label="setosa")
plt.scatter(data[50:100, 1], data[50:100, 2], color="blue", marker="x", label="versicolor")
plt.scatter(data[100:, 1], data[100:, 2], color="green", marker="+", label="virginica")
plt.xlabel("sepal width (cm)")
plt.ylabel("petal length (cm)")
plt.legend()
answer = clf.predict(x_test[:, feature])
acc = get_acc(answer, y_test)
plt.title("10153903105 Acc:" + str(int(acc * 100)) + "%")

# 1,3
plt.subplot(2, 3, 5)
feature = [1, 3]
clf.fit(x_train[:, feature], y_train)
draw_bonderay(data[:, 1].max(), data[:, 1].min(), data[:, 3].max(), data[:, 3].min(), clf)
plt.scatter(data[:50, 1], data[:50, 3], color="red", marker="o", label="setosa")
plt.scatter(data[50:100, 1], data[50:100, 3], color="blue", marker="x", label="versicolor")
plt.scatter(data[100:, 1], data[100:, 3], color="green", marker="+", label="virginica")
plt.xlabel("sepal width (cm)")
plt.ylabel("petal width (cm)")
plt.legend()
answer = clf.predict(x_test[:, feature])
acc = get_acc(answer, y_test)
plt.title("10153903105 Acc:" + str(int(acc * 100)) + "%")

# 2,3
plt.subplot(2, 3, 6)
feature = [2, 3]
clf.fit(x_train[:, feature], y_train)
draw_bonderay(data[:, 2].max(), data[:, 2].min(), data[:, 3].max(), data[:, 3].min(), clf)
plt.scatter(data[:50, 2], data[:50, 3], color="red", marker="o", label="setosa")
plt.scatter(data[50:100, 2], data[50:100, 3], color="blue", marker="x", label="versicolor")
plt.scatter(data[100:, 2], data[100:, 3], color="green", marker="+", label="virginica")
plt.xlabel("petal length (cm)")
plt.ylabel("petal width (cm)")
plt.legend()
answer = clf.predict(x_test[:, feature])
acc = get_acc(answer, y_test)
plt.title("10153903105 Acc:" + str(int(acc * 100)) + "%")
plt.show()

# encoding: utf-8
import numpy as np
import pandas as pd
from sklearn.preprocessing import StandardScaler
import matplotlib.pyplot as plt


def getR_2(y_hat, y):  # 拟合优度函数
    a = np.sum((y_hat - y) ** 2)
    b = np.sum((y - np.mean(y)) ** 2)
    return 1 - a / b

def calculate(inputX, inputY, step, w, b,m):
    y_hat = np.dot(w.T, inputX) + b
    dw = np.dot(inputX, (y_hat - inputY).T) * (1 / float(m))
    db = np.sum(y_hat - inputY) * (1 / float(m))
    w = w - step * dw
    b = b - step * db
    return w, b, y_hat

def getStep(inputX, inputY):
    # 初始步长100，逐渐减小步长，若损失函数开始减小，则返回该步长
    j = []
    step = 100.0
    while (len(j) == 2 and j[1] < j[0] ) == False:
        n = inputX.shape[0]
        m = inputX.shape[1]
        w = np.zeros((n, 1))
        b = 0
        j = []
        revl = []
        for i in range(2):
            revl = calculate(inputX, inputY, step, w, b,m)
            w = revl[0]
            b = revl[1]
            y_hat = revl[2]
            jj=np.sum((y_hat - inputY) ** 2) / float(2 * m)
            j.append(jj)
        step = step / 2
    return step


def gradiengDesent(inputX, inputY, step, iteration):
    n = inputX.shape[0]
    m = inputY.shape[1]
    w = np.zeros((n, 1))
    b = 0
    j = []
    revl = []
    for i in range(iteration):
        revl = calculate(inputX, inputY, step, w, b,m)
        w = revl[0]
        b = revl[1]
        y_hat = revl[2]
        j.append(np.sum((y_hat - inputY) ** 2) / (2 * m))

    return w, b, j


def predict(w, b, testX):
    y_hat = np.dot(w.T, testX) + b
    return y_hat


trainData = pd.read_csv('trainingData2.csv', header=None)
testData = pd.read_csv('testData2.csv', header=None)

trainX = trainData.values[:, :-1]  # 行不变，列取到 倒数第二列

# 特征标准化
scaler = StandardScaler().fit(trainX)
Xtrain_scaled = scaler.transform(trainX)

trainY = trainData.values[:, -1].reshape((1, trainX.shape[0]))



testX = testData.values[:, :-1]
testY = testData.values[:, -1].reshape((1, testX.shape[0]))

# 步长选定函数
step = getStep(Xtrain_scaled.T, trainY)
print("当前步长：" + str(step))

# 训练参数 注意X有转置
w, b, j = gradiengDesent(Xtrain_scaled.T, trainY, step, 100)

print(j)

index_j = []
for i in range(len(j)):
    index_j.append(i)

plt.figure("costFunction")
plt.title("costFunction")
plt.xlabel("times")
plt.ylabel("cost")
plt.plot(index_j, j, "r-")
plt.savefig("costFunction.jpg")
plt.show()

# 测试数据标准化
Xtest_scale = scaler.transform(testX)
y = predict(w, b, Xtest_scale.T)
index_y = []
for i in range(y.shape[1]):
    index_y.append(i)

plt.figure("result analysis")
plt.title("result analysis")
plt.xlabel("times")
plt.ylabel("y")
plt.plot(index_y, y.tolist()[0], "y-")
plt.plot(index_y, testY.tolist()[0], "g-")
plt.savefig("result.jpg")
plt.show()


print("拟合优度（R平方）：" + str(getR_2(y, testY)))

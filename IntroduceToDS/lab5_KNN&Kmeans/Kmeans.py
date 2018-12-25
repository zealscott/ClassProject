import numpy as np
import matplotlib.pyplot as plt

inputs = [[-35, 3], [-50, 18], [3, 0], [5, 8],
          [-14, -5], [-55, 10], [-50, 5], [-45, -10], [-55, 0],
          [-40, 8], [15, 8], [15, 11], [-40, -2], [13, 13],
          [20, 23], [-19, -11], [-9, -16], [21, 27], [-49, 15],
          [26, 13], [-46, 5], [-34, -1], [11, 15], [-22, -16],
          [19, 28], [-12, -8], [-13, -19], [-41, 11], [-11, -6], [-25, -9], [-18, -3]]

inputs = np.array(inputs)


def Distance(vector1, vector2):
    return (np.sum((vector1 - vector2) ** 2, axis=1)) ** 0.5


# init centroids with random samples
def initCentroids(dataSet, k):
    numSamples, dimension = dataSet.shape
    centroids = np.zeros((k, dimension))
    for i in range(k):
        index = int(np.random.uniform(0, numSamples))
        centroids[i, :] = dataSet[index, :]
    return centroids


# init with Kmeans++
def initKpp(dataSet, k):
    numSamples, dimension = dataSet.shape
    centroids = np.zeros((k, dimension))
    # first select one point randomly
    index = int(np.random.uniform(0, numSamples))
    centroids[0, :] = dataSet[index, :]
    # select others
    for i in range(1, k):
        dis = np.zeros(numSamples)
        dis = Distance(centroids[i - 1, :], dataSet)
        dis2 = dis ** 2
        dis2 = dis2 / (dis2.sum())
        for j in range(1, numSamples):
            dis2[j] = dis2[j] + dis2[j - 1]
        # choose a random
        index = np.random.uniform(0, 1)
        # select the index to get centroid
        index = np.argwhere(dis2 > index)[0][0]
        centroids[i, :] = dataSet[index, :]
    print("select init centroids:")
    print(centroids)
    return centroids


def showCluster(dataSet, k, centroids, cluster,count):
    plt.figure()
    numSamples = dataSet.shape[0]

    colorChoose = ['blue', 'black', 'red', 'yellow', 'pink', 'grey', 'purple']
    # draw points
    for i in range(numSamples):
        markIndex = int(cluster[i])
        plt.scatter(dataSet[i, 0], dataSet[i, 1], color=colorChoose[markIndex])
    # draw centroids
    for i in range(k):
        plt.scatter(centroids[i, 0], centroids[i, 1], color=colorChoose[i], marker="^")
    plt.xlabel("X")
    plt.ylabel("Y")
    plt.title("step "+str(count+1))
    plt.show()


# k-means cluster
def kmeans(dataSet, k):
    count = 0
    numSamples = dataSet.shape[0]
    clusterCheck = np.zeros(numSamples)
    clusterChanged = True

    # centroids = initCentroids(dataSet, k)
    centroids = initKpp(dataSet, k)

    while clusterChanged:
        clusterChanged = False
        tempcluster = []
        # calculate distance
        for each in dataSet:
            getDis = Distance(each, centroids)
            getcluster = np.argsort(getDis)[0]
            tempcluster.append(getcluster)

        tempcluster = np.array(tempcluster)

        showCluster(dataSet, k, centroids, clusterCheck,count)

        judge = np.argwhere((tempcluster == clusterCheck) == False)
        if judge.size:
            clusterCheck = tempcluster
            clusterChanged = True
            count += 1
            for j in range(k):
                pointclass = np.argwhere(tempcluster == j)
                pointsInCluster = dataSet[pointclass]
                centroids[j, :] = np.mean(pointsInCluster, axis=0)
    print("after cluster, the centroids:")
    print(centroids)
    print("cluster times = " + str(count))
    return centroids, clusterCheck


kmeans(inputs, 3)

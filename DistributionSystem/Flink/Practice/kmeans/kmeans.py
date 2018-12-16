import numpy as np
from sklearn.cluster import KMeans
import pandas as pd


df = pd.read_csv(r"C:\Users\scott\OneDrive\GitHub\ClassProject\DistributionSystem\Hadoop\Kmeans\k-means.dat",sep=",",header=None)

arr = np.array(df)

kmeans = KMeans(n_clusters=4).fit(arr)

print(kmeans.cluster_centers_)




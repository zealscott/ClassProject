import numpy as np
import math

"""(x,y,z)，x,y只是代表R中元素的下标，z表示分值。R为用户评分矩阵。
由于缺失值的存在[如(0,4),(4,3),(5,5)]，所以列表不是那么整齐。
"""
pu = [
    [(0, 0, 1), (0, 1, 22), (0, 2, 1), (0, 3, 1), (0, 5, 0)],
    [(1, 0, 1), (1, 1, 32), (1, 2, 0), (1, 3, 0), (1, 4, 1), (1, 5, 0)],
    [(2, 0, 0), (2, 1, 18), (2, 2, 1), (2, 3, 1), (2, 4, 0), (2, 5, 1)],
    [(3, 0, 1), (3, 1, 40), (3, 2, 1), (3, 3, 0), (3, 4, 0), (3, 5, 1)],
    [(4, 0, 0), (4, 1, 40), (4, 2, 0), (4, 4, 1), (4, 5, 0)],
    [(5, 0, 0), (5, 1, 25), (5, 2, 1), (5, 3, 1), (5, 4, 1)]
]

"""pv只是pu的转置，(x,y)互相换位置，这样做是为了后面更新V方便而已。
"""
pv = [
    [(0, 0, 1), (0, 1, 1), (0, 2, 0), (0, 3, 1), (0, 4, 0), (0, 5, 0)],
    [(1, 0, 22), (1, 1, 32), (1, 2, 18), (1, 3, 40), (1, 4, 40), (1, 5, 25)],
    [(2, 0, 1), (2, 1, 0), (2, 2, 1), (2, 3, 1), (2, 4, 0), (2, 5, 1)],
    [(3, 0, 1), (3, 1, 0), (3, 2, 1), (3, 3, 0), (3, 5, 1)],
    [(4, 1, 1), (4, 2, 0), (4, 3, 0), (4, 4, 1), (4, 5, 1)],
    [(5, 0, 0), (5, 1, 0), (5, 2, 1), (5, 3, 1), (5, 4, 0)]
]

"""V为物品-特性矩阵  的初始化"""
V = np.mat(
    [
        [0.15968384, 0.9441198, 0.83651085],
        [0.73573009, 0.24906915, 0.85338239],
        [0.25605814, 0.6990532, 0.50900407],
        [0.2405843, 0.31848888, 0.60233653],
        [0.24237479, 0.15293281, 0.22240255],
        [0.03943766, 0.19287528, 0.95094265]
    ]
)

U = np.mat(np.eye(6))
L = 0.03

#
# """写出迭代更新U,V的代码,这里我们只进行5次迭代更新U,V。当然也可以大于5次"""
for iter in range(5):
    # 固定V，求U#
    urs = []
    for uset in pu:
        user_V = []
        user_score = []
        for i, j, score in uset:
            vor = []
            for k in range(3):
                vor.append(V[j, k])
            user_V.append(vor)
            user_score.append(score)
        user_V = np.mat(user_V)
        ur = np.linalg.inv(user_V.T * user_V + L * np.mat(np.eye(3))) * user_V.T * np.mat(user_score).T
        urs.append(ur.T)
    U = np.vstack(urs)
    # 固定U，求V#
    vrs = []
    for vset in pv:
        user_V = []
        user_score = []
        for i, j, score in vset:
            vor = []
            for k in range(3):
                vor.append(U[j, k])
            user_V.append(vor)
            user_score.append(score)
        user_V = np.mat(user_V)
        vr = np.linalg.inv(user_V.T * user_V + L * np.mat(np.eye(3))) * user_V.T * np.mat(user_score).T
        vrs.append(vr.T)
    V = np.vstack(vrs)

user_final_score = U * (V.T)
# print(user_final_score)
cout = 1
for user in user_final_score:
    print("the user %d choose " % (cout))
    cout+=1
    user = np.array(user.tolist()[0])
    index = np.argmax(user) + 1
    print(str(index) + " movie")

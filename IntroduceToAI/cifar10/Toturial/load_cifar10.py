# -*- encoding: utf8 -*-
import pickle
import numpy
import random
import matplotlib.pyplot as plt
import platform
import cv2


class Load_data:
    
    def __init__(self):
        self.load_cifar10('../dir')
    

"""依次获取各个batch数据，每个batch包含一个字典，每个字典包含有：

Data
　　一个10000*3072的numpy数组，数据类型是无符号整形uint8。这个数组的每一行存储了32*32大小的彩色图像（32*32*3通道=3072）。前1024个数是red通道，然后分别是green,blue。另外，图像是以行的顺序存储的，也就是说前32个数就是这幅图的像素矩阵的第一行。

labels
　　一个范围在0-9的含有10000个数的列表（一维的数组）。第i个数就是第i个图像的类标。

　　数据集除了6个batch之外，还有一个文件batches.meta。它包含一个python字典对象，内容有：
一个包含10个元素的列表，每一个描述了labels array中每个数字对应类标的名字。比如：label_names[0] == "airplane", label_names[1] == "automobile"
"""
    def load_cifar10(self, directory):
        # 读取训练集
        images, labels = [], []
        for filename in ['%s/data_batch_%d' % (directory, j) for j in range(1, 6)]:
            with open(filename, 'rb') as fo:
                if 'Windows' in platform.platform():
                    cifar10 = pickle.load(fo, encoding='bytes')
                elif 'Linux' in platform.platform():
                    cifar10 = pickle.load(fo)
            for i in range(len(cifar10[b"labels"])):         #按标签依次读取数据
                image = numpy.reshape(cifar10[b"data"][i], (3, 32, 32))
                image = numpy.transpose(image, (1, 2, 0))
                image = image.astype(float)
                images.append(image)
            labels += cifar10[b"labels"]
        images = numpy.array(images, dtype='float')
        labels = numpy.array(labels, dtype='int')
        self.train_images, self.train_labels = images, labels
        # 读取测试集
        images, labels = [], []
        for filename in ['%s/test_batch' % (directory)]:
            with open(filename, 'rb') as fo:
                if 'Windows' in platform.platform():
                    cifar10 = pickle.load(fo, encoding='bytes')
                elif 'Linux' in platform.platform():
                    cifar10 = pickle.load(fo)
            for i in range(len(cifar10[b"labels"])):
                image = numpy.reshape(cifar10[b"data"][i], (3, 32, 32))
                image = numpy.transpose(image, (1, 2, 0))
                image = image.astype(float)
                images.append(image)
            labels += cifar10[b"labels"]
        images = numpy.array(images, dtype='float')
        labels = numpy.array(labels, dtype='int')
        self.test_images, self.test_labels = images, labels


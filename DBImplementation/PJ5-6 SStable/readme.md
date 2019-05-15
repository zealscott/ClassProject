---
title: SStable练习一
categories:
  - DBImplementation
  - Project
date: 2019-03-23 14:36:03
---

了解LevelDB block实现源码，并读取footer。

<!-- more --> 

# LevelDB

## Block类

在`table/block.h`中定义，较为重要的成员变量有：

![55366560660](C:\Users\scott\AppData\Local\Temp\1553665606604.png)

### 构造函数

其中，对于`block`的构造函数，使用了`explicit`关键字，主要用于避免类型转换，[参考这里](https://www.cnblogs.com/rednodel/p/9299251.html)。

构造函数用于插入数据并判断大小：

![AahSzj.png](https://s2.ax1x.com/2019/03/27/AahSzj.png)

### Iterator

在`block`类中，还存在一个`iter`类，是nested classes，[参考官网](https://en.cppreference.com/w/cpp/language/nested_types)。

主要看`ParseNextKey`函数，迭代器重要任务就是解析出记录：

1. 当为第一条记录时，共享部分为0， `key_.append(p, non_shared)`;这行代码就是将第一条完整记录加在`key_`后面。
2. 当不是第一条记录时，此时`key_.resize(shared)`；这行代码获取当前记录和上一条记录共享部分， `key_.append(p, non_shared)`；这行代码就获得非共享部分，凑成完整的记录。
3. 最后，如果下一个`key`是在另一个group中，就将`restart_index`往下指。

此时这个迭代器的`key_`指向第一条记录的key，`value_`指向第一条记录的value。

![Aa4i1H.png](https://s2.ax1x.com/2019/03/27/Aa4i1H.png)

## Two Level Iterator类

如果要读取一个`SStable`文件的某个键值对，首先要读取`data block`，然后才是从这个`data block`中读取键值对。所以有两次迭代，即两个迭代器。

### 构造函数

![Aa5wIf.png](https://s2.ax1x.com/2019/03/27/Aa5wIf.png)

### 读取第一个样本

经过`SeekToFirst` 、`InitDataBlock` 这两个函数之后，`index_iter_`指向`index_block`第一条记录，`data_iter_`指向第一个块的首位置

![AaTBi6.png](https://s2.ax1x.com/2019/03/27/AaTBi6.png)

### 获取数据

先构造一个sst文件的Table类，然后构造出Table的迭代器，从迭代器就可以获取这个文件的任何键值（`seek`函数）

![AaTcsH.png](https://s2.ax1x.com/2019/03/27/AaTcsH.png)

# 读取footer

## 结构

![55427144685](http://wx3.sinaimg.cn/large/0060lm7Tly1g1pet64zwxj30jb08841l.jpg)

可以发现，footer一共有4个结构+magic number，每个结构最多10个字节，需要使用`GetVarint64`得到。

## 代码

首先需要创建一个SStable，然后通过指针指到footer，再依次读出里面的结构。

```c++

#include <cstdio>
#include <iostream>

using namespace std;

int getFileSize(const string &filename) {
    int size = 0;
    FILE* fp = nullptr;

    fp = fopen(filename.c_str(), "r");
    if (nullptr == fp) {
        return size;
    }
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);
    fclose(fp);

    return size;

}

char* GetVarint64Ptr(char* p, const char* limit, uint64_t* value) {
    uint64_t result = 0;
    for (uint32_t shift = 0; shift <= 63 && p < limit; shift += 7) {
        uint64_t byte = *(reinterpret_cast<const unsigned char*>(p));
        p++;
        if (byte & 128) {
            // More bytes are present
            result |= ((byte & 127) << shift);
        } else {
            result |= (byte << shift);
            *value = result;
            return p;
        }
    }
    return nullptr;
}

char* GetVarint64(char* input, int size, uint64_t* value) {
    char* limit = input + size;
    char* q = GetVarint64Ptr(input, limit, value);
    return q;
}

int main() {
    string fileName = "testdb.ldb";
    int fileSize = getFileSize(fileName);
    //文件总大小
    cout<<"File Size:"<<getFileSize(fileName)<<endl;

    FILE* fp = fopen(fileName.c_str(), "r");
    fseek(fp, -48L, SEEK_END);
    char* footer = (char *)malloc(48 * sizeof(char));
    fgets(footer, 48, fp);
    uint64_t metaOffset, metaSize, indexOffset, indexSize;
    footer = GetVarint64(footer, 10, &metaOffset);
    footer = GetVarint64(footer, 10, &metaSize);
    footer = GetVarint64(footer, 10, &indexOffset);
    footer = GetVarint64(footer, 10, &indexSize);
    cout<<"Meta offset:"<<metaOffset<<endl
        <<"Meta size:"<<metaSize<<endl
        <<"Index offset:"<<indexOffset<<endl
        <<"Index size:"<<indexSize<<endl;

    char* index = (char *) malloc((indexSize + 1) * sizeof(char));
    fseek(fp, 0L, static_cast<int>(indexOffset));
    fgets(index, static_cast<int>(indexSize + 1), fp);
//    cout<<index[indexSize]<<endl;
    return 0;
}

```
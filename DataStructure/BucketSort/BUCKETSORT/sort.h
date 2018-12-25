#ifndef SORT_H_INCLUDED
#define SORT_H_INCLUDED
#include <iostream>
#include <vector>
#include "class_int.h"

using namespace std;
int MaxBit(vector<integers> data) //求数据的最大位数
{
    int p = 1;
    int k;
    for(size_t i = 0; i < data.size(); i++)
    {
        k=data[i].number.size();
        if(k >= p)
        {
            p = k;
        }
    }
    return p;
}

void BucketSort(vector<integers> &data,  int begin1, int end1,int radix,int threshold,int signal)
{
// radix 表示进行比较的基数
// threshold  表示最大位数
    int cunt[10];
    integers *tmp = new integers[end1-begin1+1];
    int i, k,j,p1,p2;
    for(i = 0; i < 10; i++)  //初始化
        cunt[i] = 0;
    for(i = begin1; i <= end1; i++)
    {
        k=0;
        if (int(data[i].number.size()) >= radix)
        {
            k=data[i].number[radix-1];
        }
        cunt[k]++;//统计每个桶的数量
    }
    for(i = 1; i < 10; i++)
    {
        cunt[i] = cunt[i - 1] + cunt[i]; //加上总数
    }
    for(i = end1 ; i >= begin1 ; i--) //将所有桶中记录依次收集到tmp中
    {
        k=0;
        if (int(data[i].number.size()) >= radix)
        {
            k=data[i].number[radix-1];
        }
        tmp[cunt[k] - 1] = data[i];
        cunt[k]--;
    }
    for(i = begin1,j=0; i <= end1; i++,j++) //将临时数组的内容复制到data中
    {
        data[i] = tmp[j];
    }
    delete[] tmp;
    switch (signal)
    {
    case 0:
        if (radix<threshold)
        {
            cout<<"radix = "<<radix<<endl;
            BucketSort(data, begin1, end1,radix+1,threshold,signal);
        }
        break;
    case 1:
        for (i=0; i<10; i++)
        {
            if (i == 9)                //处理最后一位
            {
                p1=begin1+cunt[i];
                p2=end1-begin1;
            }
            else
            {
                p1=begin1+cunt[i];
                p2=begin1+cunt[i+1]-1;
            }
            if (p1 < p2 && radix>1)
            {
                BucketSort(data, p1, p2,radix-1,threshold,signal);
            }
        }
        break;
    default:
        cout<<"wrong enter!"<<endl;
        return;
    }
    return;
}




#endif // SORT_H_INCLUDED

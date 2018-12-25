#ifndef ONEPACK_H_INCLUDED
#define ONEPACK_H_INCLUDED

#include<iostream>
using namespace std;

void OnePack(int N,int M,int weight[],int value[])
{
    int f[100]={0};
    int index[100]={0};
    for (int i=1; i<=N; i++)   //第i个放不放进去
        for (int j=M; j>=1; j--)
        {
            if (weight[i]<=j)
            {
                if (f[j]<f[j-weight[i]]+value[i])
                {
                    f[j]=f[j-weight[i]]+value[i];
                    index[f[j]]=i;
                }
            }
        }
    for (int w = f[M];w>0;)
    {
        cout<<index[w]<<" ";
        w-=value[index[w]];
    }
    cout<<endl;
    cout<<f[M]<<endl;//输出最优解
}

void Completeknapsack(int N,int M,int weight[],int value[])
{
    int f[100]={0};
    int index[100]={0};
    for (int i=1; i<=N; i++)   //第i个放不放进去
        for (int j=1; j<=M; j++)
        {
            if (weight[i]<=j)
            {
                if (f[j]<f[j-weight[i]]+value[i])
                {
                    f[j]=f[j-weight[i]]+value[i];
                    index[f[j]]=i;
                }
            }
        }
    for (int w = f[M];w>0;)
    {
        cout<<index[w]<<" ";
        w-=value[index[w]];
    }
    cout<<endl;
    cout<<f[M]<<endl;//输出最优解
}
#endif // ONEPACK_H_INCLUDED

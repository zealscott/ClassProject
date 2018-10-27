#include <iostream>
#include <algorithm>
#include <cstdio>
#include <ctime>
#include <cstring>
using namespace std;
#define n 6
typedef struct{
    int id;
    int d;
    int w;
}task;
bool cmpW(task t1,task t2){
    return t1.w>t2.w;
}
bool cmpD(task t1,task t2){
    return t1.d<t2.d;
}
void random_init(task t[]){
    srand((unsigned)time(NULL));
    for(int i=0;i<n;i++){
        t[i].id=i+1;
        t[i].d=rand()%n+1;
        t[i].w=rand()%30;
    }
}
void copy(task &t1,task &t2){
    t1.id=t2.id;
    t1.d=t2.d;
    t1.w=t2.w;
}
int greedy(task a[],task ta[],int &ans){
    int num=0,i,j;//当前已经完成的任务数量
    sort(ta,ta+n,cmpW);
    int fla[9999];
    memset(fla,0,sizeof(fla));
    for(i=0;i<n;i++){
        for(j=ta[i].d;j>0;j--){
            if(fla[j]==0){
                fla[j]=1;
                break;
            }
        }
        if(j>0){
            copy(a[num++],ta[i]);
            ans-=ta[i].w;
        }
    }
    return num;
}
int main()
{
    task ta[n],a[n];
    printf("正在随机生成n(%d)组数据...\n",n);
    random_init(ta);
    printf("生成的数据为:\n");
    int ans=0,maxx=-1;
    for(int i=0;i<n;i++){
        printf("ID：%d，期限：%d，惩罚：%d\n",ta[i].id,ta[i].d,ta[i].w);
        ans+=ta[i].w;
        if(ta[i].w>maxx)
            maxx=ta[i].w;
    }
    int k=greedy(a,ta,ans);
    printf("任务惩罚为：%d\n",ans);
    sort(a,a+k,cmpD);
    printf("将完成执行的任务按照时间递增顺序排列输出：\n");
    for(int i=0;i<k;i++){
        printf("ID：%d，期限：%d，惩罚：%d\n",a[i].id,a[i].d,a[i].w);
    }

    return 0;
}

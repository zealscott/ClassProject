#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "lifegame.h"

using namespace std;

int main()
{
    char ans;   //接受输入
    life lifeTest;    //声明一个类lifeTest
    lifeTest.initialize();   //初始化矩阵
    do
    {
        lifeTest.print();    //打印矩阵
        lifeTest.update();   //更新矩阵
        cout<<"please enter a char to decide whether continue(y/n):"<<endl; //由用户输入是否继续
        cin>>ans; //存入ans中
    }
    while (ans != 'n');
    return 0;
}



void life::initialize()
{
    int i,j;
    srand(time(NULL)); //使用时间设置随机种子
    for(i=0; i<=maxrow+1; i++)
    {
        for (j=0; j<=maxcol+1; j++)
        {
            life::grif[i][j]=0;  //将矩阵初始化为0
        }
    }
    for(i=1; i<=maxrow; i++)
    {
        for (j=1; j<=maxcol; j++)
        {
            life::grif[i][j]=rand()%2;   //使用rand（）函数随机生成生命矩阵
        }
    }
    cout<<"finish initialize!"<<endl;
}

int life::neighbor_count(int row,int col)
{
    int cunt=0,i,j;
    int MoveRow[3]= {-1,0,1};   //设置行移动数组
    int MoveCol[3]= {-1,0,1};   //设置列移动数组
    for (i=0; i<3; i++)
    {
        for (j=0; j<3; j++)
        {
            if (life::grif[row+MoveRow[i]][col+MoveCol[j]]==1) cunt++;
        }
    }
    if (life::grif[row][col]==1) return cunt-1;  //若自己是活的，则需要减去自己
    else return cunt;
}

void life::update()
{
    int number,i,j,tempMatrix[maxrow+2][maxcol+2];
    memcpy(tempMatrix,grif,sizeof(grif));  //使用tempMatrix暂时存储矩阵
    for(i=1; i<=maxrow; i++)
    {
        for (j=1; j<=maxcol; j++)
        {
            number=life::neighbor_count(i,j);  //计算每个位置的周围活人的个数
            if (life::grif[i][j]==1)
            {
                if (number<=1 || number>=4) tempMatrix[i][j]=0;  //更新tempMatrix
            }
            else
            {
                if (number==3) tempMatrix[i][j]=1;
            }
        }
    }
    memcpy(grif,tempMatrix,sizeof(tempMatrix));  //将更新好的矩阵复制给grif
}

void life::print()   //打印矩阵
{
    int i,j;
    for(i=1; i<=maxrow; i++)
    {
        for (j=1; j<=maxcol; j++)
        {
            cout<<life::grif[i][j]<<" ";
        }
        cout<<endl;
    }
}

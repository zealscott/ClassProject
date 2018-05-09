#include <iostream>
#include "skiing.h"
using namespace std;


skining::skining()
{
    cout<<"please enter the R/C"<<endl;
    cin>>Row>>Column;
    for (int i=1; i<=Row; i++)
    {
        for (int j=1; j<=Column; j++)
        {
            cin>>field[i][j];
            dis_sk[i][j]=0;
        }
    }
    cout<<"finish init!"<<endl;
    len=0;  //初始化高度
}

int skining::test_bound(int R,int C)
{
    int flag=0;
    int new_R,new_C;
    for (int i=0; i<4; i++)
    {
        //得出下一步的点
        new_R=R+dir[0][i];
        new_C=C+dir[1][i];
        if (new_C>=1 && new_C<=Column &&new_R>=1 && new_R<=Row)
        {
            //如果下一步的点不是边界
            if (field[new_R][new_C]!=10001 && field[new_R][new_C]<=field[R][C])
            {
                //如果下一步的点没有走过并且高度更低
                flag=1;
                break;
            }
        }
    }
    return !flag;
}

void skining::calculate(int R,int C,int milage)
{
    int temp_high;
    int new_R,new_C;
    if (test_bound(R,C))
    {
        len=milage>len?milage:len;
        return;
    }
    else
    {
        for (int i=0; i<4; i++)
        {
            //得出下一步的点
            new_R=R+dir[0][i];
            new_C=C+dir[1][i];
            if (new_C>=1 && new_C<=Column &&new_R>=1 && new_R<=Row)
            {
                //如果下一步的点不是边界
                if (field[new_R][new_C]!=10001 && field[new_R][new_C]<field[R][C])
                {
                    milage+=1;
                    temp_high=field[R][C];
                    field[R][C]=10001;
                    //               cout<<"next step "<<new_R<<" * "<<new_C<<endl;
                    //               cout<<"step =="<<milage<<endl;
                    calculate(new_R,new_C,milage);
                    field[R][C]=temp_high;
                    milage-=1;
                }
            }
        }
    }
}

int skining::dis(int C,int R)
{
    int temp;
    int new_R,new_C;
    if (dis_sk[C][R]) return dis_sk[C][R];//如果已经存在，则直接返回
    for (int i=0; i<4; i++)
    {
        new_C=C+dir[0][i];
        new_R=R+dir[1][i];
        if (in_bound(new_C,new_R))//如果在界内
        {
            if (field[C][R]>field[new_C][new_R])//如果可以顺着滑下来
            {
                temp=dis(new_C,new_R);
                dis_sk[C][R]=dis_sk[C][R]>temp?dis_sk[C][R]:temp+1;
            }
        }
    }
    return dis_sk[C][R];
}

int skining::in_bound(int R,int C)
{
    return R>=1 && R<=Row && C>=1 && C<=Column;
}




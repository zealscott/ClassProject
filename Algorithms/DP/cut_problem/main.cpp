#include <iostream>
#include <vector>

using namespace std;
int cut_up_down(int n);
int cut_down_up(int n);
int cut_down_up_decision(int n);
int price[]={0,1,5,8,9,10,17,17,20,24,30};

int r[11]={0};

int select[11]={0};  //decision
//P205
int main()
{

    cout<<cut_down_up_decision(10)<<endl;
    for (int i=0;i<=10;i++)
        cout<<r[i]<<" ";
    cout<<endl;
    for (int i=0;i<=10;i++)
        cout<<select[i]<<" ";
    return 1;
}


int cut_down_up(int n)
{
    int q;  //income
    int temp;
    for (int j=1;j<=n;j++)
    {
        q=-1;
        for (int i=1;i<=j;i++)
        {
            temp=price[i]+r[j-i];
            if (temp>q)
                q=temp;
        }
        r[j]=q;
    }
    return r[n];
}

int cut_down_up_decision(int n)
{
    int temp;
    int q;
    for (int j=1;j<=n;j++)
    {
        q=-1;
        for (int i=1;i<=j;i++)
        {
            temp=price[i]+r[j-i];
            if (temp>q)
            {
                q=temp;
                select[j]=i;         //store the decision
            }
        }
        r[j]=q;
    }
    return r[n];
}
int cut_up_down(int n)
{
    int q;
    int temp;
    if (r[n]>0)
        return r[n];
    if (n==0)
        q=0;
    else
    {
        q=-1;
        for (int i=1;i<=n;i++)
        {
            temp=price[i]+cut_up_down(n-i);
            if (temp>q)
                q=temp;
        }
    }
    r[n]=q;
    return q;
}





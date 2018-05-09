#include <iostream>
#include "limits.h"

using namespace std;

void Matrix_Chain(int p[],int n);
void print_m(int i,int j);
int m[10][10]={{100},{100}};

int s[10][10];


int main()
{
    int p[10];
    int n;
    cin>>n;
    for (int i=0;i<=n;i++)
        cin>>p[i];
    Matrix_Chain(p,n);
    for (int i=1;i<10;i++)
    {
        for (int j=1;j<10;j++)
            cout<<s[i][j]<<" ";
        cout<<endl;
    }
    print_m(1,n);
    return 0;
}

void Matrix_Chain(int p[],int n)
{
    for (int i=1;i<=n;i++)
        m[i][i]=0;
    for (int l=2;l<=n;l++)
    {
        for (int i=1;i<=n-l+1;i++)
        {
            int j=i+l-1;
            m[i][j]=INT_MAX;
            for (int k=i;k<=j-1;k++)
            {
                int q=m[i][k]+m[k+1][j]+p[i-1]*p[k]*p[j];
                if (q<m[i][j])
                {
                    m[i][j]=q;
                    s[i][j]=k;
                }
            }
        }
    }
}

void print_m(int i,int j)
{
    if (i==j)
        cout<<"A"<<i;
    else
    {
        cout<<"(";
        print_m(i,s[i][j]);
        print_m(s[i][j]+1,j);
        cout<<")";
    }
}

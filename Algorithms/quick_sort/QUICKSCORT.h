#ifndef QUICKSCORT_H_INCLUDED
#define QUICKSCORT_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;
void swep(int *p,int *q)
{
    int temp;
    temp=*p;
    *p=*q;
    *q=temp;
}


int random_patition(int a[],int p,int r)
{
    srand(time(NULL));
    int x,i,j;
    swep(&a[rand()%(r-p)+p],&a[r]);
    x=a[r];
    i=p-1;
    for (j=p;j<=r-1;j++)
    {
        if (a[j]<=x)
        {
            i++;
            swep(&a[i],&a[j]);
        }
    }
    swep(&a[i+1],&a[r]);
    return i+1;
}

void Quick_Sort(int a[],int p,int r)
{
    if (p<r)
    {
        int q;
        q=random_patition(a,p,r);
        Quick_Sort(a,p,q-1);
        Quick_Sort(a,q+1,r);
    }
}


#endif // QUICKSCORT_H_INCLUDED

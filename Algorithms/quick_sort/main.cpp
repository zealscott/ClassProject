#include <iostream>
#include "QUICKSCORT.h"
using namespace std;

int main()
{
    int a[100],i,j,n;
    cout<<"please enter the number of list"<<endl;
    cin>>n;
    for (i=0;i<n;i++)
    {
        cin>>a[i];
    }
    Quick_Sort(a,0,n-1);
    for (i=0;i<n;i++)
    {
        cout<<a[i]<<" ";
    }
    return 0;
}

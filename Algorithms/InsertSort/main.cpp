#include <iostream>

using namespace std;
void insertSort(int a[],int n);
int main()
{
    int a[100],n,i;
    cout << "enter the number:" << endl;
    cin>>n;
    for (i=1;i<=n;i++)
    {
        cin>>a[i];
    }
    insertSort(a,n);
    for (i=1;i<=n;i++)
    {
        cout<<a[i]<<" ";
    }
    return 0;
}


void insertSort(int a[],int n)
{
    int i,j,key;
    for (j=2;j<=n;j++)
    {
        key=a[j];
        i=j-1;
        while (i>0 && a[i]>key)
        {
            a[i+1]=a[i];
            i--;
        }
        a[i+1]=key;
    }
}

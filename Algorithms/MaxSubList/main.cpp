#include <iostream>

using namespace std;
//扫描法
int maxSum(int* a, int n)
{
    int sum=0;
    //其实要处理全是负数的情况，很简单，如稍后下面第3点所见，直接把这句改成："int sum=a[0]"即可
    //也可以不改，当全是负数的情况，直接返回0，也不见得不行。
    int b=0;

    for(int i=0; i<n; i++)
    {
        if(b<0)           //...
            b=a[i];
        else
            b+=a[i];
        if(sum<b)
            sum=b;
    }
    return sum;
}

int main()
{
    int a[10]= {7,9,-8,3,-7,6,-13,9};
    //int a[]={-1,-2,-3,-4};  //测试全是负数的用例
    cout<<maxSum(a,8)<<endl;
    return 0;
}

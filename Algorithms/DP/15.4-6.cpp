#include<iostream>
#include<algorithm>
#include<cstdio>
using namespace std;
const int INF=1e9;
int c[105];
int a[105];
int b[105];
int main()
{
    int i,j,n,m;
    //freopen("d:\\test.txt","r",stdin);
    cin>>m;
    while(m--)
    {
        cin>>n;
        for(i=0; i<n; i++)
        {
            cin>>a[i];
        }
        int ans=0;
        for(i=1;i<=n;i++) b[i]=INF; //初始化g[i]
        for(i=0;i<n;i++)
        {
            int k=lower_bound(b+1,b+1+n,a[i])-b;    //返回第一个大于或者等于a[i]的index
            c[i]=k;
            b[k]=a[i];        //更新g[k],使g数组保持最小递增序列(第1~n个元素均为当前可取最小值),不会丢失最优解
            ans=max(c[i],ans);
        }
        cout<<ans<<endl;
        for (int i=1;i<=ans;i++)
            cout<<b[i]<<" ";
    }
    //fclose(stdin);
    return 0;
}

#include<cstdlib>
#include<cstdio>
#include<cstring>
#include<iostream>
using namespace std;
const int maxn=2000005;
const int mod=20071027;

int ch[maxn][26]= {0},cnt=0,n,root=0,m;
bool vis[maxn]= {0};
char str[1000005],word[105];
int d[1000005];

void in()//建trie树
{
    int location=root,i=0;
    while(1)
    {
        if(word[i]=='\0')
        {
            vis[location]=1;   //将该节点记录，表示该节点为子节点
            return;
        }
        int k=word[i]-'a';
        if(!ch[location][k]) ch[location][k]=++cnt;// 表示该节点新建在哪一排，根据节点个数判断
        location=ch[location][k];
        i++;
    }
}
void work(int i)
{
    int p=root,j=i;
    int sum =0;

    while (i<=m-1)
    {

        int k=str[i]-'a';
        p=ch[p][k];
        if(!p)
        {
            break;  //一定要记得break，没有找的就跳出来
        }
        if(vis[p])   //找到了对应的单词
        {
            cout<<"index = "<<j<<"found "<<endl;
            if (m-i-1 ==0)
                d[j]++;
            else
            {
                if (d[i+1])
                    d[j] += d[i+1];
            }
        }
        i++;
    }

    cout<<"index = "<<j<<"has "<<d[j]<<endl;


}
int main()
{
//    freopen("in.txt","r",stdin);
//    freopen("ou.txt","w",stdout);
    scanf("%s",str);
    scanf("%d",&n);
    for(int i=1; i<=n; i++)
    {
        scanf("%s",word);
        in();
    }
//    d[0]=1;
    m=strlen(str);
    for(int i=m-1; i>=0; i--) //动规
    {
        work(i);
    }
    cout<<d[0];
    return 0;
}

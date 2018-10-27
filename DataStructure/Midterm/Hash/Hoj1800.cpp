#include <iostream>
#include <cstdio>
#include <map>
#include <string>
#include <vector>
using namespace std;


//hash application
//http://acm.hdu.edu.cn/showproblem.php?pid=1800


int main7()
{
    multimap<int, int> mymap;
    int n;
    int temp;
    vector<int> level;
    scanf("%d",&n);
    while(n--)
    {
        scanf("%d",&temp);
//        cin>>temp;
        level.push_back(temp);
        mymap.insert(make_pair(temp,n));
    }
    int cunt=1;
    for (vector<int>::iterator it=level.begin();it!=level.end();it++)
    {
        int c = mymap.count(* it);
        if (cunt<c)
            cunt=c;
    }
    printf("%d\n",cunt);

    return 0;
}

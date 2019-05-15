#include <iostream>
#include <map>
#include <vector>

using namespace std;

int main()
{
    int times=0,i=0,flag=0;
    multimap<string,int> mp;
    cin>>times;
    vector<string> n;
    string secret;
    while (i<times)
    {
        string temp;
        cin>>temp;
        n.push_back(temp);
        mp.insert(make_pair(temp,i));
        i++;
    }
    cin>>secret;
    for (i=0; i<times; i++)
    {
        if (mp.count(n[i])==1)
        {
            cout<<n[i];
            break;
        }
    }
    if (i==times)
    {
        cout<<"-1"<<endl;
        return 0;
    }
    for (size_t i=0; i<secret.size(); i++)
    {
        if (flag==1)
            break;
        for (size_t j=secret.size()-1; j>i; j--)
        {
            string temp;
            temp=secret.substr(i,i);
            if (mp.count(temp)>0)
            {
                cout<<i;
                flag =1;
                break;
            }
        }
    }
    if (flag==0)
        cout<<"-1";
    return 0;
}

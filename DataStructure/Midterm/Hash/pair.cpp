#include <iostream>
#include <map>
using namespace std;

int main4()
{
    map<int, int> mp;
    for (int i = 0; i < 10; i ++)
    {
        mp[i] = i;
    }
    for (int i = 10; i < 20; i++)
    {
        mp.insert(make_pair(i, i));
    }
    map<int, int>::iterator it;
    for (it = mp.begin(); it != mp.end(); it++)
    {
//        cout("%d-->%d\n", it->first, it->second);
    }
    return 0;
}

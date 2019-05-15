#include <iostream>
#include "TrieNode.h"
using namespace std;

int main()
{
    TrieNode *root = NULL;
    char temp[keynum];

    while (1)
    {
        cout<<"enter the str,n for stop"<<endl;
        cin>>temp;
        if (!strcasecmp(temp,"n"))
            break;
        Key a(temp);
        TrieInsert(a,root);
    }

    while (1)
    {
        cout<<"enter the str,n for stop"<<endl;
        cin>>temp;
        if (!strcasecmp(temp,"n"))
            break;
        Key a(temp);
        TrieSearch(a,root);
    }

    while (1)
    {
        cout<<"enter the str,n for stop"<<endl;
        cin>>temp;
        if (!strcasecmp(temp,"n"))
            break;
        Key a(temp);
        TrieDelete(a,root);
    }
    return 0;
}

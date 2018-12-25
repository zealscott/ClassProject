#include <iostream>
#include <string>

using namespace std;

//book2 P83 KMP algorithm
int* getNext(string ps);
int KMP(string T,string p);
int main()
{
    string T="abcabd";
    string p="abcabd";
    cout<<KMP(T,p)<<endl;
    return 0;
}


int* getNext(string p)
{
    int* next = new int[p.length()];
    next[0] = -1;           //while the first char not match, i++,j++
    int j = 0;
    int k = -1;
    while (j < (int)p.length() - 1)
    {
        if (k == -1 || p[j] == p[k])
        {
            j++;
            k++;
            next[j] = k;
        }
        else
        {
            k = next[k];
        }
    }
    for (int i=0;i<(int)p.length();i++)
        cout<<next[i]<<" ";
    return next;
}


int KMP(string T,string p)
{
    int i=0;
    int j=0;
    int* next=getNext(T);
    while (i<(int)T.length() && j<(int)p.length())
    {
        if (j==-1 || T[i]==p[j])
        {
            i++;
            j++;
        }
        else
        {
            j=next[j];
        }
    }
    if (j==(int)p.length())
    {
        return i-j;
    }
    return -1;
}


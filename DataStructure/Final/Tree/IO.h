#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED
#include <iostream>
#include <stdlib.h>
using namespace std;
// 都是先读取字符串再操作

//前序建树，不存在的树用#表示
bool ReadStream_DFS(int &number)
{
    char buffer[32];
    if (cin>>buffer)
    {
        if (buffer[0] != '#')
        {
            number = atoi(buffer);
            return true;
        }
        return false;
    }
    cout<<"end!"<<endl;
    return false;
}

//层序建树,不存在的用NULL表示
int ReadStream_BFS(int &number)
{
    char buffer[32];
    if (cin>>buffer)
    {
        if (buffer[0] != 'n')
        {
            number = atoi(buffer);
            return 1;
        }
        return -1;
    }
    cout<<"end"<<endl;
    return 0;
}



#endif // IO_H_INCLUDED

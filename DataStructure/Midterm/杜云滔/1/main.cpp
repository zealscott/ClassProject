#include <iostream>
#include <stdlib.h>
#include "stdio.h"
#include "BinaryTree.h"
#include <typeinfo>
using namespace std;

int main()
{
    BinaryTree MyTree;
    char data[22];

    while(1)
    {
        scanf("%s",data);
        int temp =0;
        if (data[0]>='0' && data[0]<='9')
        {
            temp=atoi(data);
        }
        MyTree.Insert(temp);
        char  ch = getchar();
        if (ch=='\n')
            break;
    }
//    MyTree.printpreorder();
    int sum;
    cin>>sum;
    MyTree.search_all(sum);

    return 0;
}

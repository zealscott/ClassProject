#include <iostream>
#include <string>
#include "IO.h"
#include "tree.h"
using namespace std;

int main()
{
    string s = "6 4 1 # 2 # # 5 # # 8 # 9 # #";
    string temp = "10 5 12 4 7 null null";
    string t;
    getline(cin,t);
    BinaryTreeNode* T;
//    BfsCreateTree(T);
    DfsCreateTree(T);
    cout<<t<<endl;
//    PreOrder(T);
    return 0;
}



int main2()
{
    BinaryTreeNode* T;
// Ç°ÐòÖÐÐò½¨Ê÷
    string PreArray = "ABCDEGF";
    string InArray = "CBEGDFA";
    string PostArray = "CGEFDBA";
//    PreInCreateTree(T,0,0,InArray.length(),PreArray,InArray);
    PostInCreateTree(T,PostArray.length()-1,0,InArray.length(),PostArray,InArray);
    PreOrder(T);
    return 0;
}


#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED
#include <string>
#include <queue>
#include "stdio.h"
struct BinaryTreeNode
{
    char data;
    int number;
    BinaryTreeNode* left;
    BinaryTreeNode* right;

    BinaryTreeNode():left(NULL),right(NULL) {number =0;};
    BinaryTreeNode(char d):data(d),left(NULL),right(NULL) {};
    BinaryTreeNode(int d):number(d),left(NULL),right(NULL) {};
};

int FindChar(string &a,char target)
{
    unsigned i;
    for (i = 0; i < a.length(); i++)
    {
        if (a[i] == target)
            break;
    }
    return i;
}


void PostOrder(BinaryTreeNode* &T)
{
    if (T == NULL)
        return;
    PostOrder(T->left);
    PostOrder(T->right);
//    cout<<T->data;
    cout<<T->number;
}

void PreOrder(BinaryTreeNode* &T)
{
    if (T == NULL)
        return;
//    cout<<T->data;
    cout<<T->number;
    PreOrder(T->left);
    PreOrder(T->right);
}

void PreInCreateTree(BinaryTreeNode* &T,int PreIndex,int InIndex,int subTreelen,string &PreArray,string &InArray)
{
    if (subTreelen <= 0)
    {
        T = NULL;
        return;
    }
    T = new BinaryTreeNode(PreArray[PreIndex]);

    int index = FindChar(InArray,PreArray[PreIndex]);
    int LeftLen = index - InIndex;
    PreInCreateTree(T->left,PreIndex+1,InIndex,LeftLen,PreArray,InArray);

    int RightLen = subTreelen - 1- LeftLen;
    PreInCreateTree(T->right,PreIndex + LeftLen +1,index+1,RightLen,PreArray,InArray);
}

void PostInCreateTree(BinaryTreeNode* &T,int PostIndex,int InIndex,int subTreelen,string &PostArray,string &InArray)
{
    if (subTreelen <= 0)
    {
        T = NULL;
        return;
    }
    T = new BinaryTreeNode(PostArray[PostIndex]);

    int index = FindChar(InArray,PostArray[PostIndex]);
    int LeftLen = index - InIndex;
    PostInCreateTree(T->left,PostIndex-(subTreelen-1-LeftLen)-1,InIndex,LeftLen,PostArray,InArray);

    int RightLen = subTreelen - 1- LeftLen;
    PostInCreateTree(T->right,PostIndex-1,index+1,RightLen,PostArray,InArray);
}


void DfsCreateTree(BinaryTreeNode *&T)
{
    int number;
    if (ReadStream_DFS(number))
    {
        T = new BinaryTreeNode(number);
        cout<<"create node "<<number<<endl;
        DfsCreateTree(T->left);
        DfsCreateTree(T->right);
    }
}

void BfsCreateTree(BinaryTreeNode *&T)
{
    int number;
    char temp[32];
    char ch;
    T = new BinaryTreeNode();
    queue<BinaryTreeNode*> nodequeue;
    nodequeue.push(T);
    while (!nodequeue.empty())
    {
        BinaryTreeNode* currentNode = nodequeue.front();
        nodequeue.pop();
        cin>>temp;
        ch = getchar();
        if (ch == '\n')
            break;
        else if (temp[0] == 'n')
            continue;
        else
        {
            number = atoi(temp);
        }
        cout<<"number = "<<number;
        currentNode->number = number;
        currentNode->left =new BinaryTreeNode();
        currentNode->right =new BinaryTreeNode();
        nodequeue.push(currentNode->left);
        nodequeue.push(currentNode->right);
    }
}





#endif // TREE_H_INCLUDED

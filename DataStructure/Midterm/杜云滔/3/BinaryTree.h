#ifndef BINARYTREE_H_INCLUDED
#define BINARYTREE_H_INCLUDED
#include "BinaryNode.h"
#include "stdio.h"
#include <queue>
#include <stack>

using namespace std;
class BinaryTree
{
public:
    bool Empty();
    int Size();
    void Insert();
    void height();
    BinaryTree();
    void print();
    void get_root();
protected:
    BinaryNode *root;
    queue<int> qu;
    stack<int> rount;
    int RountGet[2][10];
    void recursive_insert(BinaryNode* &subroot);
    void recursive_preorder(BinaryNode* subroot);
    int recursive_height(BinaryNode* subroot);
    void inorder_insert(BinaryNode* &subroot,int level);
    void DFS_Recursive(BinaryNode* subroot,int target);
    void writeIn(int target);
    void get();
    int Count;
    int a;
    int b;
    int flag;
};


BinaryTree::BinaryTree()
{
    root = NULL;
    Count = 0;
    flag=0;
    for (int i=0; i<2; i++)
    {
        for (int j=0; j<10; j++)
            RountGet[i][j]=0;
    }
}

bool BinaryTree::Empty()
{
    return (Count == 0);
}

int BinaryTree::Size()
{
    return Count;
}

void BinaryTree::height()
{
    recursive_height(this->root);
    if (this->flag==1)
        cout<<"No"<<endl;
    else
        cout<<"Yes"<<endl;
}

int BinaryTree::recursive_height(BinaryNode* subroot)
{
    int heightTree = 0;
    if (subroot != NULL)
    {
        int right = recursive_height(subroot->right);
        int left = recursive_height(subroot->left);
        if (right-left>1 || right-left<-1)
        {
            flag =1;
        }
        heightTree = right>left?right+1:left+1;
        return heightTree;
    }
    return 0;
}

void BinaryTree::print()
{
    recursive_preorder(this->root);
}

void BinaryTree::recursive_preorder(BinaryNode* subroot)
{
    if ( subroot!=NULL &&subroot->data != -13)
    {
        cout<<subroot->data<<endl;
        recursive_preorder(subroot->left);
        recursive_preorder(subroot->right);
    }
    return;
}

void BinaryTree::Insert()
{
    char i;
    i=getchar();
    while (i != '\n')
    {
        if (i!=' ')
        {
            int j;
            j=i-'0';
            qu.push(j);
        }
        i=getchar();
    }
    cin>>a;
    cin>>b;
    inorder_insert(this->root,1);
}


void BinaryTree::recursive_insert(BinaryNode* &subroot)
{
    if (qu.size() <= 0)
        return;
    if (qu.front() <= 0)
    {
        qu.pop();
        return;
    }
    if (subroot == NULL)
    {
        subroot = new BinaryNode(qu.front());
//        cout<<"insert "<<qu.front()<<endl;
        qu.pop();
        recursive_insert(subroot->left);
        recursive_insert(subroot->right);
    }
}

void BinaryTree::inorder_insert(BinaryNode* &subroot,int level)
{
    if (qu.size() <= 0)
        return;
    if (level == 4)
    {
        subroot = new BinaryNode(qu.front());
        qu.pop();
        return;
    }
    if (subroot == NULL)
    {
        subroot = new BinaryNode();

        inorder_insert(subroot->left,level+1);

        subroot->data = qu.front();

        cout<<"insert "<<qu.front();
        cout<<"  level =  "<<level<<endl;

        qu.pop();

        inorder_insert(subroot->right,level+1);
    }
}






void BinaryTree::get_root()
{
    DFS_Recursive(this->root,a);

    while(!rount.empty())
        rount.pop();
    DFS_Recursive(this->root,b);

    get();
}

void BinaryTree::DFS_Recursive(BinaryNode* pRoot,int target)
{
    if (pRoot == NULL)
    {
        return;
    }
    if (pRoot->data == target)
    {
        writeIn(target);
        return;
    }
    rount.push(pRoot->data);
    DFS_Recursive(pRoot->left,target);
    DFS_Recursive(pRoot->right,target);
    rount.pop();
}

void BinaryTree::writeIn(int target)
{
    stack<int> temp = rount;
    int j =0;
    int i =0;
    if (target==b)
    {
        j=1;
    }
    for (; i<10 ; i++)
    {

        if (temp.empty())
            break;
        RountGet[j][i]=temp.top();
//        cout<<rount.top()<<endl;
        temp.pop();
    }
}

void  BinaryTree::get()
{
    int r;
    int m,n;

    for (m=0; m<10; m++)
    {

        if (RountGet[0][m]==0)
            break;
    }


    for (n=0; n<10; n++)
    {

        if (RountGet[1][n]==0)
            break;
    }

    int i=m-1;
    int j=n-1;
    for (; i>=0 && j>=0; i--,j--)
    {
        if (RountGet[0][i]==RountGet[1][j])
        {

            r=RountGet[0][i];
        }
        else
        {
            break;
        }

    }
    cout<<r;
}

#endif // BINARYTREE_H_INCLUDED

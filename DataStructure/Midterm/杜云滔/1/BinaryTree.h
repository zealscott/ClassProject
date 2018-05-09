#ifndef BINARYTREE_H_INCLUDED
#define BINARYTREE_H_INCLUDED
#include "BinaryNode.h"
#include <stack>

using namespace std;
class BinaryTree
{
public:
    bool Empty();
    int Size();
    bool Insert(int data);
    BinaryTree();
    void search_all(int sum);
    void printpreorder();
protected:
    BinaryNode *root;
    stack<BinaryNode *> InRount;
    int Count;
    void recursive_preorder(BinaryNode* subroot);
    void DFS_Recursive(BinaryNode* pRoot,int sum,int now);
    void print();
};


BinaryTree::BinaryTree()
{
    root = NULL;
    Count = 0;
}

bool BinaryTree::Empty()
{
    return (Count == 0);
}

int BinaryTree::Size()
{
    return Count;
}

bool BinaryTree::Insert(int data)
{

    this->Count++;
    if (this->root==NULL)
    {
        root = new BinaryNode(data);
        return true;
    }
    if (data==0)
        return true;
    int tempnum = Count;
    BinaryNode *subroot = root;
    stack<int> rount;
    while (tempnum>1)
    {
        if (tempnum %2 == 0)
            rount.push(2);
        else
            rount.push(1);
        tempnum/=2;
    }
    while (rount.size()>1)
    {
        tempnum = rount.top();
        if (tempnum == 2)
            subroot = subroot->left;
        else
            subroot = subroot->right;
        rount.pop();
    }
    tempnum = rount.top();
    if (tempnum == 2)
        subroot->left = new BinaryNode(data);
    else
        subroot->right = new BinaryNode(data);
    return true;
}

void BinaryTree::print()
{
    stack<BinaryNode*> temp = InRount;
    stack<int> num;
    while (temp.size()!=0)
    {
        num.push(temp.top()->data);
        temp.pop();
    }
    cout<<"[";
    while (num.size()!=0)
    {
        cout<< num.top() <<",";
        num.pop();
    }
    cout<<"]"<<endl;;
}


void BinaryTree::search_all(int sum)
{
    DFS_Recursive(this->root,sum,0);
}

void BinaryTree::DFS_Recursive(BinaryNode* pRoot,int sum,int now)
{
    now += pRoot->data;
    InRount.push(pRoot);

    if (pRoot->left==NULL && pRoot->right==NULL)
    {
        if (now == sum)
        {
            cout<<"found!"<<endl;
            print();
        }
        InRount.pop();
        return;
    }

    if(pRoot->left)
    {
        DFS_Recursive(pRoot->left,sum,now);
    }
    if (pRoot->right)
    {
        DFS_Recursive(pRoot->right,sum,now);
    }

    InRount.pop();
}

void BinaryTree::printpreorder()
{
    recursive_preorder(this->root);
}

void BinaryTree::recursive_preorder(BinaryNode* subroot)
{
    if (subroot != NULL)
    {
        cout<<subroot->data<<endl;
        recursive_preorder(subroot->left);
        recursive_preorder(subroot->right);
    }
}
#endif // BINARYTREE_H_INCLUDED

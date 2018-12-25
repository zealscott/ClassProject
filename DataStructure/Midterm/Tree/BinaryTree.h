#ifndef BINARYTREE_H_INCLUDED
#define BINARYTREE_H_INCLUDED
#include "BinaryNode.h"
#include <stack>
#include <iostream>

template<class Record>
class BinaryTree
{
public:
    BinaryTree();
    bool Empty()const;
    int Size()const;
    int Height();
    void Insert(Record &x);
    void InOrder();
    void PreOrder();
    void PostOrder();
    bool operator == (BinaryTree<Record> &tree);
    bool isEqual(BinaryNode<Record>* &root,BinaryNode<Record>* &other_root);
protected:
    int Count;
    BinaryNode<Record> *root;
    void recursive_inorder(BinaryNode<Record>* subroot)const;
    void recursive_preorder(BinaryNode<Record>* subroot)const;
    void recursive_postorder(BinaryNode<Record>* subroot)const;
    int recursive_height(BinaryNode<Record>* subroot);
};


template<class Record>
bool BinaryTree<Record>::operator == (BinaryTree<Record> &tree)
{
    return isEqual(root,tree.root);
}


template<class Record>
bool BinaryTree<Record>::isEqual (BinaryNode<Record>* &root,BinaryNode<Record>* &other_root)
{
    if (root ==NULL && other_root == NULL)
        return true;
    if (!(root && other_root))
        //at least one is NULL
        return false;
    if (root->data ==other_root->data)
        return isEqual(root->left,other_root->left) && isEqual(root->right,other_root->right);
    else
        return false;
}


template<class Record>
BinaryTree<Record>::BinaryTree()
{
    root = NULL;
    Count = 0;
}

template<class Record>
bool BinaryTree<Record>::Empty()const
{
   return Count == 0;
}

template<class Record>
BinaryTree<Record>::Size()const
{
    return Count;
}


template<class Record>
int BinaryTree<Record>::Height()
{
    return recursive_height(root);
}

template<class Record>
int BinaryTree<Record>::recursive_height(BinaryNode<Record> *sub_root)
{
   int treeHeight = 0;
   if (sub_root != NULL)
   {
        int leftHeight = recursive_height(sub_root->left);
        int rightHeight = recursive_height(sub_root->right);
        treeHeight = leftHeight > rightHeight ? leftHeight+1:rightHeight+1;
   }
   return treeHeight;
}

template<class Record>
void BinaryTree<Record>::Insert(Record &x)
{
    if (Empty())
    {
        root = new BinaryNode<Record>(x);
        Count++;
        return;
    }
    Count++;
    std::stack<int> rount;
    int tempcount = Size();
    while (tempcount > 1)
    {
        if (tempcount % 2 ==1)
            rount.push(1);
        else
            rount.push(2);
        tempcount/=2;
    }
    BinaryNode<Record> *current = root;
    while (rount.size()>1)
    {
        int number = rount.top();
        if (number ==1 )
            current = current->right;
        if (number == 2)
            current = current->left;
        rount.pop();
    }
    int number = rount.top();
    if (number ==1 )
        current->right = new BinaryNode<Record>(x);
    else
        current->left = new BinaryNode<Record>(x);
}

template<class Record>
void BinaryTree<Record>::InOrder()
{
    recursive_inorder(root);
}

template<class Record>
void BinaryTree<Record>::recursive_inorder(BinaryNode<Record>* subroot)const
{
    if (subroot != NULL)
    {
        recursive_inorder(subroot->left);
        std::cout<<subroot->data<<" ";
        recursive_inorder(subroot->right);
    }
}

template<class Record>
void BinaryTree<Record>::PostOrder()
{
    recursive_postorder(root);
}

template<class Record>
void BinaryTree<Record>::recursive_postorder(BinaryNode<Record>* subroot)const
{
    if (subroot != NULL)
    {
        recursive_postorder(subroot->left);
        recursive_postorder(subroot->right);
        std::cout<<subroot->data<<" ";
    }
}

template<class Record>
void BinaryTree<Record>::PreOrder()
{
    recursive_preorder(root);
}

template<class Record>
void BinaryTree<Record>::recursive_preorder(BinaryNode<Record>* subroot)const
{
    if (subroot != NULL)
    {
        std::cout<<subroot->data<<" ";
        recursive_preorder(subroot->left);
        recursive_preorder(subroot->right);

    }
}



#endif // BINARYTREE_H_INCLUDED

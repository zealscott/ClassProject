#ifndef BINARYTREE_H_INCLUDED
#define BINARYTREE_H_INCLUDED
#include <stack>
#include "BinaryNode.h"

using namespace std;

template <class Entry>
class Binary_tree{
public:
    Binary_tree();
    bool Empty()const;
    int Size()const;
    int Height();
    void Insert(Entry &x);
    void InOrder(void (*visit)(Entry &));
    void PreOrder(void (*visit)(Entry &));
    void PostOrder(void (*visit)(Entry &));

protected:
    int Count;
    Binary_node<Entry> *root;
    void recursive_inorder(Binary_node<Entry>*sub_root, void (*visit)(Entry &));
    void recursive_preorder(Binary_node<Entry>*sub_root, void (*visit)(Entry &));
    void recursive_postorder(Binary_node<Entry>*sub_root, void (*visit)(Entry &));
    int recursive_height(Binary_node<Entry>*node)const;
};


template <class Entry>
Binary_tree<Entry>::Binary_tree()
{
    root = NULL;
    Count = 0;
}

template<class Entry>
bool Binary_tree<Entry>::Empty()const
{
    return root==NULL;
}

template<class Entry>
int Binary_tree<Entry>::Size()const
{
    return Count;
}

template<class Entry>
int Binary_tree<Entry>::Height()
{
    return recursive_height(root);
}



template<class Entry>
int Binary_tree<Entry>::recursive_height(Binary_node<Entry>*node)const
{
    int treeHeight = 0;
    if (node != NULL)
    {
        int leftHeight = recursive_height(node->left);
        int rightHeight = recursive_height(node->right);
        treeHeight = leftHeight >= rightHeight? leftHeight + 1:rightHeight + 1;
    }
    return treeHeight;
}

//preorder
template<class Entry>
void Binary_tree<Entry>::PreOrder(void (*visit)(Entry &))
{
    recursive_preorder(root,visit);
}

template<class Entry>
void Binary_tree<Entry>::recursive_preorder(Binary_node<Entry>*sub_root,void (*visit)(Entry &))
{
    if (sub_root!=NULL)
    {
        (*visit)(sub_root->data);
        recursive_preorder(sub_root->left,visit);
        recursive_preorder(sub_root->right,visit);
    }
}

//inorder
template<class Entry>
void Binary_tree<Entry>::InOrder(void (*visit)(Entry &))
{
    recursive_inorder(root,visit);
}

template<class Entry>
void Binary_tree<Entry>::recursive_inorder(Binary_node<Entry>*sub_root,void (*visit)(Entry &))
{
    if (sub_root!=NULL)
    {
        recursive_inorder(sub_root->left,visit);
        (*visit)(sub_root->data);
        recursive_inorder(sub_root->right,visit);
    }
}

//postorder
template<class Entry>
void Binary_tree<Entry>::PostOrder(void (*visit)(Entry &))
{
    recursive_postorder(root,visit);
}

template<class Entry>
void Binary_tree<Entry>::recursive_postorder(Binary_node<Entry>*sub_root,void (*visit)(Entry &))
{
    if (sub_root!=NULL)
    {
        recursive_postorder(sub_root->left,visit);
        recursive_postorder(sub_root->right,visit);
        (*visit)(sub_root->data);
    }
}



template<class Entry>
void Binary_tree<Entry>::Insert(Entry &x)
{
    Count++;
    if (Empty())
    {
        root = new Binary_node<Entry>(x);
        return;
    }
    stack<int> rount;
    int tempcount = Size();
    while (tempcount > 1)
    {
        if (tempcount%2 == 1)
            rount.push(1); //the node is right node
        else
            rount.push(2); //the node is left node
        tempcount = tempcount/2;
    }
    Binary_node<Entry>*current = root;
    while (rount.size()>1) //pop until left one node
    {
        int number = rount.top();
        if (number==1)
            current = current->right;
        if (number==2)
            current = current->left;
        rount.pop();
    }
    int number = rount.top();
    if (number == 1)
        current->right = new Binary_node<Entry>(x);
    if (number == 2)
        current->left = new Binary_node<Entry>(x);
}



#endif // BINARYTREE_H_INCLUDED

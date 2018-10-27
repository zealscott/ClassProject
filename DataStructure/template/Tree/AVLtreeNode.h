#ifndef AVLTREENODE_H_INCLUDED
#define AVLTREENODE_H_INCLUDED
#include "BinaryNode.h"
using namespace std;

template <class Record>
struct AVL_node:public Binary_node<Record>
{
//  additional data member
    Balance_factor balance;
//  constructors
    AVL_node();
    AVL_node(const Record &x);
//  overridden virtual functions
    void set_balance(Balance_factor b);
    Balance_factor get_balance()const;
//    ~AVL_node();
};




template<class Record>
AVL_node<Record>::AVL_node():Binary_node<Record>()
{
    balance=equal_height;
}

template<class Record>
AVL_node<Record>::AVL_node(const Record &x):Binary_node<Record>(x)
{
    balance = equal_height;
}

template<class Record>
void AVL_node<Record>::set_balance(Balance_factor b)
{
    balance = b;
}

template<class Record>
Balance_factor AVL_node<Record>::get_balance()const
{
    return balance;
}
#endif // AVLTREENODE_H_INCLUDED

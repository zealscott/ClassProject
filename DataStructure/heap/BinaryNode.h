#ifndef BINARYNODE_H_INCLUDED
#define BINARYNODE_H_INCLUDED
#include <iostream>
using namespace std;

template <class Entry>
struct Binary_node
{
// data members
    Entry data;

    Binary_node <Entry> *left;
    Binary_node <Entry> *right;
// constructors
    Binary_node();
    Binary_node(const Entry &x);
};


template<class Entry>
void print(Entry &x)
{
    cout<<x<<" ";
}

template<class Entry>
Binary_node<Entry>::Binary_node()
{
    left = NULL;
    right = NULL;
}

template<class Entry>
Binary_node<Entry>::Binary_node(const Entry &x)
{
    data = x;
    left = NULL;
    right = NULL;
}


#endif // BINARYNODE_H_INCLUDED

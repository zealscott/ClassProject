#ifndef HEAP_H_INCLUDED
#define HEAP_H_INCLUDED
#include "BinaryNode.h"
#include <vector>
#include <iostream>

using namespace std;

enum Error_code {success,fail};

template<class Entry>
class heap
{
protected:
    /* the compare function to decide min or max */
//    bool (*compare)(Entry, Entry);
    bool (*compare)(Binary_node<Entry> *, Binary_node<Entry> *);
    /* the size of vector */
    int Count;
    vector<Binary_node<Entry> *> TreeNode;
    /* find element */
    int find_pos(Entry &x);
    /* heapfy */
    void AdjustUp(int index);

    void AdjustDown(int index);
public:
//    heap(bool (*p)(Entry, Entry));
    heap(bool (*p)(Binary_node<Entry> *, Binary_node<Entry> *));
    Error_code Push(Entry x);
    Error_code Delete(Entry x);
    Entry Pop();
    bool Empty();
    void print();
};

template<class Entry>
//heap<Entry>::heap(bool (*p)(Entry, Entry))
heap<Entry>::heap(bool (*p)(Binary_node<Entry> *, Binary_node<Entry> *))
{
    Count = 0;
    compare = p;
}

template<class Entry>
Error_code heap<Entry>::Push(Entry x)
{
    Binary_node<Entry> *p = new Binary_node<Entry>(x);
    if (!p)
    {
        return fail;
    }
    TreeNode.push_back(p);
    Count++;
    AdjustUp(Count-1);
    return success;
}


template<class Entry>
Entry heap<Entry>::Pop()
{
    Entry data = TreeNode[0]->data;
    TreeNode[0] = TreeNode[Count-1];
    TreeNode.pop_back();
    Count--;
    AdjustDown(0);
    return data;
}

template<class Entry>
Error_code heap<Entry>::Delete(Entry x)
{
    int pos = find_pos(x);
    if (pos >= Count)
    {
        return fail;
    }
    TreeNode[pos] = TreeNode[Count-1];
    TreeNode.pop_back();
    Count--;
    AdjustDown(pos);
    return success;
}


template<class Entry>
void heap<Entry>::AdjustUp(int index)
{
    Binary_node<Entry> *temp = TreeNode[index];
//    while (index > 0 && compare(temp->data,TreeNode[(index - 1) / 2]->data))
    while (index > 0 && compare(temp,TreeNode[(index - 1) / 2]))
    {
//        cout<<"compare "<<temp->data<<" "<<TreeNode[(index - 1) / 2]->data<<endl;
        TreeNode[index] = TreeNode[(index - 1) / 2];
        index = (index - 1) / 2;
    }
    TreeNode[index] = temp;
}


template<class Entry>
void heap<Entry>::AdjustDown(int index)
{
    Binary_node<Entry> *temp = TreeNode[index];
    index = index * 2 + 1;
    while (index < Count)
    {
        if (index + 1 < Count && compare(TreeNode[index+1],TreeNode[index]))
//        if (index + 1 < Count && compare(TreeNode[index+1]->data,TreeNode[index]->data))
            index++;
//        if (compare(temp->data,TreeNode[index]->data))
        if (compare(temp,TreeNode[index]))
            break;
        else
        {
            TreeNode[(index - 1) / 2] = TreeNode[index];
            index = index * 2 + 1;
        }
    }
    TreeNode[(index - 1) / 2] = temp;
}

template<class Entry>
int heap<Entry>::find_pos(Entry &x)
{
    int i;
    for (i=0; i<Count; i++)
    {
        if (TreeNode[i]->data == x)
            break;
    }
    return i;
}

template<class Entry>
void heap<Entry>::print()
{
    for (int i=0;i < Count;i++)
        cout<<TreeNode[i]->data<<" ";
}

template<class Entry>
bool heap<Entry>::Empty()
{
    return Count == 0;
}
#endif // HEAP_H_INCLUDED

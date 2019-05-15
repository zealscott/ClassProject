#include <iostream>
#include "heap.h"

using namespace std;



template<class Entry>
bool max_node (Binary_node<Entry> *a, Binary_node<Entry> *b)
{
    return (a->data) > (b->data);
}


int main()
{
//    bool (*p)(int,int);
    bool (*p)(Binary_node<int> *,Binary_node<int> *);
//    p = Min<int>;
    p = max_node<int>;
    heap<int> MyHeap(p);
    for (int i=100;i>=0;i--)
        MyHeap.Push(i);

//    MyHeap.Delete(5);
//    while (!MyHeap.Empty())
        cout<<MyHeap.Pop()<<" ";
        cout<<MyHeap.Pop()<<" ";
        cout<<MyHeap.Pop()<<" ";
//    MyHeap.print();
    cout<<endl;
////////////////////////////

//    p = Max<int>;
//    heap<int> MyHeap2(p);
//    for (int i=0;i<=9;i++)
//        MyHeap2.Push(i);
//
//    MyHeap2.Delete(5);
//    while (!MyHeap2.Empty())
//        cout<<MyHeap2.Pop()<<" ";
//    MyHeap2.print();
    return 0;
}



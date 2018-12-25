#include <iostream>
#include "List.h"
using namespace std;
template<class List_entry>
List<List_entry>::List()
{
    Count=0;
}

template<class List_entry>
int List<List_entry>::Size()const
{
    return Count;
}

template<class List_entry>
bool List<List_entry>::full()const
{
    return (Count==max_list);
}

template<class List_entry>
bool List<List_entry>::Empty()const
{
    return (Count==0);
}

template<class List_entry>
void List<List_entry>::Clear()
{
    Count=0;
}

template<class List_entry>
Error_code List<List_entry>::Insert(int position, const List_entry &x)
{
    if (full()) return overflow;
    if (position<0 || position > Count) return range_error_new;
    for (int i=Count-1;i>=position;i--)
        entry[i+1]=entry[i];
    entry[position]=x;
    Count++;
    return success;
}

template<class List_entry>
Error_code List<List_entry>::Remove(int position,List_entry &x)
{
    if (Empty()) return overflow;
    if (position<0 || position >= Count) return range_error_new;
    x=entry[position];
    for (int i=position;i<Count-1;i++)
        entry[i]=entry[i+1];
    Count--;
    return success;
}

template<class List_entry>
Error_code List<List_entry>::Replace(int position, const List_entry &x)
{
    if (position<0 || position >= Count) return range_error_new;
    entry[position]=x;
    return success;
}

template<class List_entry>
Error_code List<List_entry>::Retrieve(int position,List_entry &x)const
{
    if (position<0 || position >= Count) return range_error_new;
    x=entry[position];
    return success;
}

template<class List_entry>
void List<List_entry>::Traverse(void (*visit)(List_entry &))
{
    /* the action specified by function (*visit) */
    for (int i=0;i<Count;i++)
        (*visit)(entry[i]);
}

template<class List_entry>
void update(List_entry &x)
{
    x*=2;
}

template<class List_entry>
void print(List_entry &x)
{
    cout<<x<<endl;
}

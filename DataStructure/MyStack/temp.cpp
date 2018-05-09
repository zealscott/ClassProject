#include <iostream>
#include "MyIntStack_template.h"



template <class T>
MyStack<T>::MyStack()   //the stack is initialize to be empty
{
    cunt=0;
}

template <class T>
bool MyStack<T>::Empty() const
{
    bool outcome=true;
    if (cunt>0) outcome=false;
    return outcome;
}

template <class T>
Error_code MyStack<T>::push(const T & item)
{
    Error_code outcome=success;
    if (cunt>maxstack)
        outcome=overflow;
    else
        entry[cunt++]=item;
    return outcome;
}

template <class T>
Error_code MyStack<T>::pop()
{
    Error_code outcome=success;
    if (cunt==0)
        outcome=underflow;
    else --cunt;
    return outcome;
}

template <class T>
Error_code MyStack<T>::top(T& item)
{
    Error_code outcome=success;
    if (cunt==0)
        outcome=underflow;
    else
        item=entry[cunt-1];
    return outcome;
}

template <class T>
void  MyStack<T>::fun1()
{
    int a;
    MyStack<T>::fun2();
}

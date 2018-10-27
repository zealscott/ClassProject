#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED
#include <iostream>
#include <string>

using namespace std;

enum Error_code{underflow,overflow,success};
const int maxstack=10;

typedef struct data
{
    string data;
    float score;
}d;


template <class T>
class MyStack{
public:
    MyStack();
    bool Empty() const;
    void push(const T &item);
    void top( T &item) ;
    void pop();
private:
    int cunt;
    T entry[maxstack];
};

template <class T>
void MyStack<T>::push(const T &item)
{

    entry[++cunt]=item;
}

template <class T>
MyStack<T>::MyStack()
{
    int n;
    T item;
    cunt=-1;
    cout<<"how many?"<<endl;
    cin>>n;
    while (n--)
    {
        cin>>item;
        push(item);
    }
}


template <class T>
void MyStack<T>::top( T &item)
{
    cout<<"the top item is "<<entry[cunt]<<endl;
    item=entry[cunt];
}

template <class T>
void MyStack<T>::pop()
{
    cunt--;
}
#endif // STACK_H_INCLUDED

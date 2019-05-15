#include <iostream>
#include "MyIntStack.h"

using namespace std;

int main2()
{
    int n;
    Stack item;
    MyStack numbers;
    cout<<"please enter a number:"<<endl;
    cin>>n;
    for (int i=0;i<n;i++)
    {
        cin>>item;
        numbers.push(item);
    }
    cout<<endl<<endl;
    while (!numbers.Empty())
    {
        numbers.top(item);
        cout<<item<<" ";
        numbers.pop();
    }
    cout<<endl;
    return 0;
}


MyStack::MyStack()   //the stack is initialize to be empty
{
    cunt=0;
}

bool MyStack::Empty() const
{
    bool outcome=true;
    if (cunt>0) outcome=false;
    return outcome;
}

Error_code MyStack::push(const Stack & item)
{
    Error_code outcome=success;
    if (cunt>maxstack)
        outcome=overflow;
    else
        entry[cunt++]=item;
    return outcome;
}

Error_code MyStack::pop()
{
    Error_code outcome=success;
    if (cunt==0)
        outcome=underflow;
    else --cunt;
    return outcome;
}

Error_code MyStack::top(Stack& item)
{
    Error_code outcome=success;
    if (cunt==0)
        outcome=underflow;
    else
        item=entry[cunt-1];
    return outcome;
}

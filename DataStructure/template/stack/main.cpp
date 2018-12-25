#include <iostream>
#include "stack.h"
using namespace std;

int main()
{
    double item;
    MyStack<double> s;
    s.top(item);

    s.pop();
        s.top(item);
}

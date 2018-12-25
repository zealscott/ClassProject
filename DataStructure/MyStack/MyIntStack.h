#ifndef MYINTSTACK_H_INCLUDED
#define MYINTSTACK_H_INCLUDED
typedef double Stack;
enum Error_code{underflow,overflow,success};
const int maxstack=10;

class MyStack{
public:
    MyStack();
    bool Empty() const;
    Error_code push(const Stack &item);
    Error_code top( Stack &item) ;
    Error_code pop();
private:
    int cunt;
    Stack entry[maxstack];
};


#endif // MYINTSTACK_H_INCLUDED

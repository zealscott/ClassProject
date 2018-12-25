#include <iostream>
#include <cctype>
#include <stack>
#include <stdlib.h>
#include <math.h>
#include "fun.h"
//cannot express negative;

using namespace std;
void EvaluateExpression();
int main()
{
    EvaluateExpression();
    return 0;
}


void EvaluateExpression()
{
    char c;
    string temp="";
    stack<double> number;
    stack<char> op;
    op.push('#');
    cin>>c;
    do
    {
        if (isdigit(c) || c=='.')
        {
            temp+=c;
            cin>>c;
        }
        else
        {
            if (temp!="")   //如果数字字符串不为空，且下个char不为数字，则push
            {
                number.push(ToDouble(temp));
                temp="";   //将其置空，以便存储下一个数字
            }
            switch (precede(op.top(),c))
            {
            case '<':
                op.push(c);
                cin>>c;
                break;
            case '=':
                op.pop();
                cin>>c;
                break;
            case '>':
                double a,b;
                char theta;
                theta=op.top();    //弹出运算符进行运算
                op.pop();
                b=number.top();    //弹出两个数字
                number.pop();
                a=number.top();
                number.pop();
                number.push(operate(a,theta,b));   //将运算出的值push
                break;
            }
        }
    }
    while (c!='#' || op.top()!='#');
    cout<<number.top()<<endl;
}



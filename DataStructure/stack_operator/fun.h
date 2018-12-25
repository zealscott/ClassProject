#ifndef FUN_H_INCLUDED
#define FUN_H_INCLUDED

using namespace std;
char relation[8][8]={{'0','+','-','*','/','(',')','#'},
                    {'+','>','>','<','<','<','>','>'},
                    {'-','>','>','<','<','<','>','>'},
                    {'*','>','>','>','>','<','>','>'},
                    {'/','>','>','>','>','<','>','>'},
                    {'(','<','<','<','<','<','=','0'},
                    {')','>','>','>','>','0','>','>'},
                    {'#','<','<','<','<','<','0','='}};

char precede(char top,char written)
{
    int i,j;
    for (i=0;i<8;i++)
    {
        if (relation[i][0]==top) break;
    }
    for (j=0;j<8;j++)
    {
        if (relation[0][j]==written) break;
    }
    return relation[i][j];
}

double operate(double a,char theta,double b)
{
    if (theta=='+') return a+b;
    else if (theta=='-') return a-b;
    else if (theta=='*') return a*b;
    else if (theta=='/') return a/b;
    return 0;
}

double ToDouble(string a)
{
    double sum=0;
    int i=0,n;
    while (a[i]!='.' && a[i]!='\0') i++;
    n=i;
    for (i=0; i<n; i++)
    {
        sum+=(a[i]-'0')*pow(10,n-i-1);
    }
    for (i=n+1; a[i]!='\0'; i++)
    {
        sum+=(a[i]-'0')*pow(0.1,i-n);
    }
    return sum;
}


#endif // FUN_H_INCLUDED

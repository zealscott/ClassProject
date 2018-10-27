#include <iostream>
#include "skiing.h"

using namespace std;

int main()
{
    int max_dis=0,temp;
    skining new_skiing;
    for (int i=1;i<=new_skiing.Row;i++)
    {
        for (int j=1;j<=new_skiing.Column;j++)
        {
            new_skiing.calculate(i,j,0);
            temp=new_skiing.dis(i,j);
            max_dis=max_dis>temp?max_dis:temp;
        }
    }
    cout<<"the farthest length is "<<new_skiing.len<<endl;
    cout<<"the farthest length is "<<max_dis<<endl;
    return 0;
}

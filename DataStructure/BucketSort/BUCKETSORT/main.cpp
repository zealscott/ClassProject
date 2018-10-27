#include <iostream>
#include "sort.h"
#include "class_int.h"

using namespace std;

int main()
{
//    integers a;
//    a.init();
//    integers b;
//    b.init();
//    integers c;
//    c=a+b;
//    cout<<c;
    while (1)
    {
        int n,m,choice,k;
        char iter;
        //////////
        cout<<"enter the n:"<<endl;
        cin>>n;
        m=n;
        vector<integers> Int;
        while (n--)
        {
            integers temp;
            temp.init();
            Int.push_back(temp);
        }
        k=MaxBit(Int);
        ////////
        cout<<"LSD(0) or MSD(1):"<<endl;
        cin>>choice;
        switch (choice)
        {
        case 0:     //LSD
            BucketSort(Int,0,m-1,1,k,0);
            break;
        case 1:
            BucketSort(Int,0,m-1,k,k,1);
            break;
        default:
            break;
        }
        for(size_t i = 0; i < Int.size(); i++)
        {
            cout<<Int[i];
            cout<<" ";
        }
        cout<<endl<<"continue?y or n  ";
        cin>>iter;
        if (iter!='y' && iter!='Y')
            break;
    }
}



#include <iostream>


#include "circular.h"

using namespace std;

int main()
{
    circular_struct link1;
    link1.print(link1.head);
    cout<<"-------"<<endl;
    circular_struct link2;
    link2.print(link2.head);
    cout<<"-------"<<endl;
    link1.Merge(link1.head,link2.head);
    link1.print(link1.head);
//    link1.print();
//    cout<<"-------"<<endl;
//    link.sort_link(link.head);
//    link.print();
}

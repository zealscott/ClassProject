#include <iostream>
#include "List.cpp"

using namespace std;

int main2()
{
    List <int>mylist;
    for (int i=0; i<5; i++)
        mylist.Insert(i,i);
    cout<<"Your list have "<<mylist.Size()<<" element:"<<endl;
    int i;
    void (*fp)(int &x);
    /*定义函数指针，必须指定类型*/
    fp=print;
    mylist.Traverse(fp);
    //mylist.Traverse(print);
    mylist.Remove(0,i);
    cout<<"After remove(0)"<<endl;
    mylist.Traverse(print);
    cout<<"After update"<<endl;
    mylist.Traverse(update);
    mylist.Traverse(print);
    return 0;
}





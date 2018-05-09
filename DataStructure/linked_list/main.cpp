#include <iostream>
#include <cstring>
#include "struct.h"


//使用两种方法进行合并
//递归和非递归
using namespace std;
void merge_recursion(Node p1,Node p2,Node head,Node now,Node another_head);
int main()
{
    LIST student_node1;
    LIST student_node2;
    student_node1.Reverse();
    cout<<"reverse :"<<endl;
    student_node1.print();
    student_node1.Delete_ID();
    cout<<"delete after :"<<endl;
    student_node1.print();
    cout<<"another node :"<<endl;
    student_node2.print();
    student_node1.bubblesort_score(student_node1.head);
    student_node1.bubblesort_score(student_node2.head);
    cout<<"after sort stu1:"<<endl;
    student_node1.print();
    cout<<"after sort stu2:"<<endl;
    student_node2.print();
//    student_node1.merge_link(student_node2.head);
    merge_recursion(student_node1.head->next,student_node2.head->next,student_node1.head,student_node1.head,student_node2.head);
    cout<<"after merge"<<endl;
    student_node1.print();
    return 0;
}

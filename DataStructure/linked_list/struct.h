#ifndef STRUCT_H_INCLUDED
#define STRUCT_H_INCLUDED
#include <iostream>
#include <cstring>


using namespace std;

typedef struct STU* Node;
struct STU
{
    int ID;
    string name;
    float score;
    Node next;
};

class LIST
{
public:
    int SIZE;
    Node head;  //指向第一个指针
    Node tail;  //尾指针，指向NULL
    LIST();
    void Delete_ID();
    void print();
    void Reverse();
    void bubblesort_score(Node head);
    void insert_stu();
    void insert_Node(Node p1,Node p2);
    void merge_link(Node another_head);
    void append_Node(Node p2);
};



#endif // STRUCT_H_INCLUDED

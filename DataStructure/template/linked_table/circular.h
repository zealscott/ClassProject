#ifndef CIRCULAR_H_INCLUDED
#define CIRCULAR_H_INCLUDED
#include <string>
using namespace std;


typedef struct Node_circular* Node;

typedef struct Node_circular{
    string data;
    float score;
    Node next;
}cir;


class circular_struct
{
public:
    int SIZE;
    Node head;
    circular_struct();
    ~circular_struct();
    void print(Node head);
    void append();  //insert to pos+1
    Node find_Node(int pos);
    void inverse(Node head);
    void sort_link(Node head);
    void Merge(Node head1,Node head2);
};

#endif // CIRCULAR_H_INCLUDED

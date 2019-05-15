#include <iostream>
#include "circular.h"


circular_struct::circular_struct()
{
    Node pre,p;
    int n;
    head=new cir;
    pre=head;
    cout<<"how many node"<<endl;
    cin>>n;
    SIZE=n;
    while (n--)
    {
        p=new cir;
        cout<<"enter the data and score"<<endl;
        cin>>p->data>>p->score;
        pre->next=p;
        pre=p;
    }
    p->next=head->next;  //将最后一个节点连接到第一个节点
}

circular_struct::~circular_struct()
{
    Node p,pre;
    pre=head->next->next;  //从第二个节点开始删除
    if (!pre)
    {
        if (head->next)
        {
            delete head->next;
        }
        delete head;
    }
    return;
    while (pre!=head->next)
    {
        p=pre->next;   //保存下一个节点
        cout<<"delete  "<<pre->data<<endl;
        delete pre;
        pre=p;
    }
    cout<<"delete  "<<pre->data<<endl;
    delete pre;
    delete head;
    cout<<"delete head!"<<endl;
}

void circular_struct::print(Node head)
{
    Node p;
    p=head->next;
    while (p)
    {
        cout<<"Node's data:"<<p->data<<"Node's score"<<p->score<<endl;
        p=p->next;
        if (p==head->next) break;
    }
}

Node circular_struct::find_Node(int pos)
{
    int n=1;
    Node p=head->next;
    while (n++!=pos)
    {
        p=p->next;
        if (p==head->next)
        {
            cout<<"there is no such node!"<<endl;
            return NULL;
        }
    }
    return p;
}

void circular_struct::append()  //insert to pos+1
{
    Node pre,p;
    int pos;
    cout<<"enter where you want to append to"<<endl;
    cin>>pos;
    pre=find_Node(pos);
    if (pre==NULL) return;
    p=new cir;
    cout<<"enter the data and score"<<endl;
    cin>>p->data>>p->score;
    p->next=pre->next;
    pre->next=p;
    cout<<"append success!"<<endl;
}

void circular_struct::inverse(Node head)
{
    Node p1,p2,temp;
    p1=head->next;  //第一个节点
    p2=p1->next;  //第二个节点
    while (p2->next!=head->next)  //当p2为尾节点时
    {
        temp=p2->next;
        p2->next=p1;
        p1=p2;
        p2=temp;
    }
    p2->next=p1;
    head->next->next=p2;
    head->next=p2;
}

void swap_Node(Node p1,Node p2)
{
    string data;
    float score;
    data=p1->data;
    score=p1->score;
    p1->data=p2->data;
    p1->score=p2->score;
    p2->data=data;
    p2->score=score;

}


void circular_struct::sort_link(Node h)
{
    Node x,y;
    for (x=h->next; x->next!=h->next; x=x->next)
    {
        for (y=x->next; y!=h->next; y=y->next)
        {
            if (y->score>x->score) swap_Node(x,y);
        }
    }
}

void circular_struct::Merge(Node head1,Node head2)
{
    Node x,y;
    x=head1->next;
    y=head2->next;
    do
    {
        x=x->next;
    }while (x->next!=head1->next);
        do
    {
        y=y->next;
    }while (y->next!=head2->next);
    x->next=head2->next;
    y->next=head1->next;
    sort_link(head1);
}

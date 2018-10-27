#ifndef FUC_H_INCLUDED
#define FUC_H_INCLUDED
#include <iostream>
#include <cstring>
#include "struct.h"
using namespace std;


void copy_node(Node p1,Node p2)
{
    p1->ID=p2->ID;
    p1->name=p2->name;
    p1->score=p2->score;
}


int bigger(Node p1,Node p2)   //比较p1，p2，若p2大，则返回0
{
    return (p1->score>p2->score)?1:0;
}

void swap_Node(Node p1,Node p2)  //交换p1，p2的值
{
    Node temp=new STU;
    temp->ID=p1->ID;
    temp->name=p1->name;
    temp->score=p1->score;
    p1->ID=p2->ID;
    p1->name=p2->name;
    p1->score=p2->score;
    p2->ID=temp->ID;
    p2->name=temp->name;
    p2->score=temp->score;
    delete temp;
}

void LIST::insert_Node(Node p1,Node p2)  //insert front p1
{
    Node p=new STU;
    Node temp=head;
    for (; temp->next!=NULL; temp=temp->next)
    {
        if (temp->next==p1) break;
    }
    copy_node(p,p2);
    p->next=temp->next;
    temp->next=p;
}

void LIST::append_Node(Node p2)  //append p2
{
    Node p=new STU;
    Node temp=head;
    for (temp=temp->next; temp->next->next!=NULL; temp=temp->next) {}
    copy_node(p,p2);
    p->next=temp->next;
    temp->next=p;
}

void LIST::print()
{
    Node p;
    p=head->next;
    while (p->next!=NULL)
    {
        cout<<"the ID is "<<p->ID<<" name : "<<p->name<<" score : "<<p->score<<endl;
        p=p->next;
    }
}

LIST::LIST()
{
    int n,i,k;
    Node pre,p;
    SIZE=0;
    cout<<"how many students?"<<endl;
    cin>>n;
    pre=new STU;
    head=pre;
    while (n--)
    {
        p=new STU;
        cout<<"enter the ID,name,score"<<endl;
        cin>>p->ID>>p->name>>p->score;
        if (p!=NULL)
        {
            pre->next=p;
            pre=p;
            SIZE++;
        }
        else
        {
            cout<<"cannot create list!"<<endl;
            break;
        }
    }
    tail=new STU;
    tail->next=NULL;
    pre->next=tail;
}

void LIST::Delete_ID()
{
    int id=0,flag=0;
    Node p,q;
    p=head;   //p为头节点
    cout<<"please enter the ID that would be deleted:"<<endl;
    cin>>id;
    while(p->next)
    {
        if (id==p->next->ID) break;
        p=p->next;
    }
    if (!p->next)
    {
        cout<<"there is no such ID!"<<endl;
        return ;
    }
    q=p->next;
    p->next=q->next;
    delete q;
    return ;
}

void LIST::Reverse()
{
    Node p1,p2,temp;
    p1=head->next; //p1指向第一个节点
    p2=p1->next;  //p2指向第二个节点
    head->next=NULL;  //将头结点指向NULL
    p1->next=head;   //将第一个节点指向头结点
    while (p2!=tail)
    {
        temp=p2->next;
        p2->next=p1;  //反向
        p1=p2;   //移向下一个
        p2=temp;
    }
    tail->next=p1;
    p2=tail;   //首尾交换
    tail=head;
    head=p2;
}

void LIST::bubblesort_score(Node head)
{
    Node x=head->next,y;
    for (; x->next->next!=NULL; x=x->next)
    {
        for (y=x->next; y->next!=NULL; y=y->next)
        {
            if (bigger(y,x))
                swap_Node(y,x);
        }
    }
}

void LIST::merge_link(Node another_head)
{
    //将两个链表排序，并合并链表到第一个链表
    int flag;
    Node pa=head->next,pb=another_head->next;
    Node root=head;
    LIST::bubblesort_score(head);
    LIST::bubblesort_score(another_head);
    while (pa->next!=NULL && pb->next!=NULL)
    {
        if (bigger(pa,pb)) //pa>=pb
        {
            root->next=pa;
            root=root->next;
            pa=pa->next;
        }
        else
        {
            root->next=pb;
            root=root->next;
            pb=pb->next;
        }
    }
    if (pa->next)
    {
        root->next=pa;
    }
    else
    {
        root->next=pb;
    }
    return ;
}

void merge_recursion(Node p1,Node p2,Node head,Node now,Node another_head)
{
    if (p1->next==NULL)
    {
        now->next=p2;
        another_head->next=NULL;
        return;
    }
    else if (p2->next==NULL)
    {
        now->next=p1;
        another_head->next=NULL;
        return;
    }
    else
    {
        if (bigger(p1,p2))
        {
            now->next=p1;
            now=p1;
            p1=p1->next;
            merge_recursion(p1,p2,head,now,another_head);
        }
        else
        {
            now->next=p2;
            now=p2;
            p2=p2->next;
            merge_recursion(p1,p2,head,now,another_head);
        }
    }
}

void LIST::insert_stu()
{
    //将新的学生信息插入到第一个
    Node p=new STU;
    cout<<"enter the ID,name,score"<<endl;
    cin>>p->ID>>p->name>>p->score;
    p->next=head->next;
    head->next=p;
}

#endif // FUC_H_INCLUDED

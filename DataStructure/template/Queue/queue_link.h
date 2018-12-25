#ifndef QUEUE_LINK_H_INCLUDED
#define QUEUE_LINK_H_INCLUDED
#include <iostream>

using namespace std;

typedef struct Queue2* qNode;
typedef struct Queue2
{
    string data;
    float score;
    qNode next;
} q2;

class link_q
{
public:
    qNode font;
    qNode rear;
    link_q();
    ~link_q();
    void q_append();
    bool q_empty();
    void q_delete();
    void print();
};


link_q::link_q()
{
    int n;
    qNode temp;
    cout<<"how many Node?"<<endl;
    cin>>n;
    font=rear=new q2;
    if (!rear)
    {
        cout<<"fail to create Node!"<<endl;
        return;
    }
    cout<<"enter the data and score"<<endl;
    cin>>font->data>>font->score;
    font->next=NULL;
    n--;
    while (n--)
    {
        temp=new q2;
        if (!temp)
        {
            cout<<"fail to create Node!"<<endl;
            return;
        }
        cout<<"enter the data and score"<<endl;
        cin>>temp->data>>temp->score;
        temp->next=NULL;
        rear->next=temp;
        rear=temp;
    }
}

link_q::~link_q()
{
    while (font)
    {
        rear=font->next;   //»ØËÝ
        cout<<"delete "<<font->data<<endl;
        delete font;
        font=rear;
    }
}

void link_q::q_append()
{
    qNode temp;
    temp=new q2;
    cout<<"enter the data and score"<<endl;
    cin>>temp->data>>temp->score;
    temp->next=NULL;
    rear->next=temp;
    rear=temp;
    cout<<"data: "<<rear->data<<endl;
}

bool link_q::q_empty()
{
    if (!font) return 1;
    else return 2;
}

void link_q::q_delete()
{
    qNode temp;
    temp=font;
    font=font->next;
    cout<<"delete "<<temp->data<<endl;
    delete temp;
}

void link_q::print()
{
    qNode temp;
    temp=font;
    while (temp)
    {
    cout<<"data: "<<temp->data<<endl;
    temp=temp->next;
    }

}
#endif // QUEUE_LINK_H_INCLUDED

#ifndef PRIORITY_QUEUE_H_INCLUDED
#define PRIORITY_QUEUE_H_INCLUDED
#include <iostream>
#include <stdio.h>
#include <queue>
using namespace std;


typedef pair<int,int>int2;

struct cmp{
    bool operator() ( int2 a, int2 b ){return a.first< b.first;}
};

int main2()
{
    //priority_queue<int2,vector<int2>,greater<int2> > p1;
    int2 a,b;
    priority_queue<int2,vector<int2>,cmp > p1;
    p1.push(int2(1,2));
    p1.push(int2(3,4));
    p1.push(int2(2,8));
    p1.push(int2(5,0));
    a=int2(1,2);
    b=int2(2,1);
    struct cmp c;
    c(a,b);
    for(int i=0;i<4;i++)
    {
        int2 temp=p1.top();p1.pop();
        printf("(%d,%d)\n",temp.first,temp.second);
    }


    return 0;

}

struct Edge{
    int start,end;
    double weight;
    Edge(int start,int end,int weight):start(start),end(end),weight(weight){};
};
typedef struct Edge Edge;


bool operator > (Edge a,Edge b)
{
    return a.weight > b.weight;
}



int main3()
{
    priority_queue<Edge,vector<Edge>,greater<Edge> > pqueue_Edge;
    pqueue_Edge.push(Edge(0,1,3));
    pqueue_Edge.push(Edge(0,2,4));
    pqueue_Edge.push(Edge(0,3,8));
    pqueue_Edge.push(Edge(0,4,5));


    while(!pqueue_Edge.empty()){
        Edge temp = pqueue_Edge.top();
        pqueue_Edge.pop();
        cout<<temp.start<<"\t"<<temp.end<<"\t"<<temp.weight<<endl;
    }
}


#endif // PRIORITY_QUEUE_H_INCLUDED

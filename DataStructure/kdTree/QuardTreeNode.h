#ifndef QUARDTREENODE_H_INCLUDED
#define QUARDTREENODE_H_INCLUDED
#include "Point.h"
#include <vector>
using namespace std;

/* 1 2
   3 4
*/

class QuadTreeNode
{
public:
    QuadTreeNode(double left,double right,double upper,double lower);
    double dis;
    int InBound(Point *p);
    int TotalPoint;
    vector<Point*> container;
    double Left_bound,Right_bound,Upper_bound,Lower_bound;
    QuadTreeNode* zone[5];
    ~QuadTreeNode(){};
};

QuadTreeNode::QuadTreeNode(double left,double right,double upper,double lower):
    Left_bound(left),Right_bound(right),Upper_bound(upper),Lower_bound(lower)
{
    for (int i=0; i<5; i++)
    {
        zone[i] = NULL;
    }
    dis = 0;
    TotalPoint = 0;
}

int QuadTreeNode::InBound(Point *p)
{
    double middle_x =(Left_bound +Right_bound)/2;
    double middle_y =(Upper_bound + Lower_bound)/2;
    if (p->X <= middle_x)
    {
        if (p->Y <= middle_y)
            return 3;
        else return 1;
    }
    else
    {
        if (p->Y <= middle_y)
            return 4;
        else return 2;
    }
}


#endif // QUARDTREENODE_H_INCLUDED

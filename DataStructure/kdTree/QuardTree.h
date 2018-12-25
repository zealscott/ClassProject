#ifndef QUARDTREE_H_INCLUDED
#define QUARDTREE_H_INCLUDED
#include "QuardTreeNode.h"

extern int M_limit;

class QuadTree
{
public:
    QuadTree(double right,double left,double upper,double lower);
    void Insert(double x,double y);
    ~QuadTree() {};
    void print();
//    void visit(Point *&p);
//    stack<QuadTreeNode *> Rount;
    double Right_bound,Left_bound,Upper_bound,Lower_bound;
    QuadTreeNode *root;
protected:
    void Insert_Recursive(Point *&p, QuadTreeNode* node);
    void print_Recursive(QuadTreeNode* node);
};

QuadTree::QuadTree(double right,double left,double upper,double lower)
    :Right_bound(right),Left_bound(left),Upper_bound(upper),Lower_bound(lower)
{
    root = new QuadTreeNode(left,right,upper,lower);
}


void QuadTree::Insert(double x,double y)
{
    Point *p = new Point(x,y);
    Insert_Recursive(p,root);
}

void QuadTree::print()
{
    print_Recursive(root);
}


void QuadTree::Insert_Recursive(Point *&p, QuadTreeNode *node)
{
    if (node->TotalPoint < M_limit)
    {
        if (node->zone[1] == NULL)
        {
            node->TotalPoint ++;
            node->container.push_back(p);
            return;
        }
        /* already have subroot*/
        else
        {
            int whichzone = node->InBound(p);
            Insert_Recursive(p,node->zone[whichzone]);
            return;
        }
    }
    //  constructor
    double middle_x = (node->Left_bound + node->Right_bound)/2;
    double middle_y = (node->Upper_bound + node->Lower_bound)/2;
    node->zone[1] = new QuadTreeNode(node->Left_bound, middle_x, node->Upper_bound, middle_y);
    node->zone[2] = new QuadTreeNode(middle_x, node->Right_bound, node->Upper_bound, middle_y);
    node->zone[3] = new QuadTreeNode(node->Left_bound, middle_x, middle_y, node->Lower_bound);
    node->zone[4] = new QuadTreeNode(middle_x, node->Right_bound, middle_y, node->Lower_bound);
    //
    int whichzone = node->InBound(p);
    Insert_Recursive(p,node->zone[whichzone]);
    vector<Point*>::iterator it;
    for (it = node->container.begin(); it < node->container.end(); it++)
    {
        Point * pt = *it;
        whichzone = node->InBound(pt);
        Insert_Recursive(pt,node->zone[whichzone]);
    }
    node->container.clear();
    node->TotalPoint = 0;
    return;
}

void QuadTree::print_Recursive(QuadTreeNode* node)
{
    if (node->zone[1] != NULL)
    {
        for (int i=1; i <= 4; i++)
        {
            cout<<"print zone"<<i<<endl;
            print_Recursive(node->zone[i]);
        }
    }
    else
    {
        vector<Point*>::iterator it;
        for (it = node->container.begin(); it < node->container.end(); it++)
        {
            (*it)->print();
        }
    }
}
#endif // QUARDTREE_H_INCLUDED

#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <stack>
#include <queue>
#include <math.h>
#include "QuardTree.h"

using namespace std;
void Get_Node_Distance(Point *&target,QuadTreeNode *&zone);
void KNN(QuadTree &MyTree,Point *target);
int M_limit;
size_t neighbor_limit;

struct cmp_point
{
    bool operator() ( Point *a, Point *b )
    {
        return a->Distance < b->Distance;
    }
};

struct cmp_zone
{
    bool operator() ( QuadTreeNode *a, QuadTreeNode *b )
    {
        return a->dis > b->dis;
    }
};

priority_queue<Point *,vector<Point *>,cmp_point > point_heap;
priority_queue<QuadTreeNode *,vector<QuadTreeNode *>,cmp_zone > node_heap;


int main()
{
    int factor[] = {1,-1};
    double right,left,upper,lower;
    double x,y;
    srand((unsigned)time(NULL));
    //--------------------------//
    cout<<"enter the limit number:"<<endl;
    cin>>M_limit;
    cout<<"enter the right,left,upper,lower bound:"<<endl;
    cin>>right>>left>>upper>>lower;
    cout<<"enter the target XY:"<<endl;
    cin>>x>>y;
    Point * target = new Point(x,y);
    cout<<"enter the neighbor number:"<<endl;
    cin>>neighbor_limit;
    //--------------------------//
    QuadTree MyTree(right,left,upper,lower);
    Get_Node_Distance(target,MyTree.root);
    node_heap.push(MyTree.root);
//    for (int i=0; i<20; i++)
//    {
////        double x = rand() / double(RAND_MAX)* factor[rand()%2]*right;
//        double x = rand() / double(RAND_MAX)* right;
////        double y = rand() / double(RAND_MAX)* factor[rand()%2]*upper;
//        double y = rand() / double(RAND_MAX)* upper;
//        cout<<x<<" "<<y<<endl;
//        MyTree.Insert(x,y);
//    }
    MyTree.Insert(1,1);
    MyTree.Insert(2,2);
    MyTree.Insert(3,3);
    MyTree.Insert(5,10);
    MyTree.Insert(-1,-1);
    MyTree.Insert(-2,-2);
//    MyTree.print();

    KNN(MyTree,target);

}


void Get_Node_Distance(Point *&target,QuadTreeNode *&zone)
{
    if (target->X <= zone->Right_bound && target->X >= zone->Left_bound)
    {
        /* if in the vertical */
        if (target->Y <= zone->Upper_bound && target->X >= zone->Lower_bound)
        {
            // in
            zone->dis = 0;
        }
        else
        {
            double temp1 = fabs(target->Y - zone->Lower_bound);
            double temp2 = fabs(target->Y - zone->Upper_bound);
            zone->dis = (temp1 > temp2 ? temp2:temp1);
        }
    }
    else if (target->Y <= zone->Upper_bound && target->X >= zone->Lower_bound)
    {
        /* if in the horizon */
        double temp1 = fabs(target->X - zone->Left_bound);
        double temp2 = fabs(target->X - zone->Right_bound);
        zone->dis = (temp1 > temp2 ? temp2:temp1);
    }
    else
    {
        double temp1 =sqrt(sqr(target->X - zone->Left_bound) + sqr(target->X - zone->Upper_bound));
        double temp2 =sqrt(sqr(target->X - zone->Left_bound) + sqr(target->X - zone->Lower_bound));
        double temp3 =sqrt(sqr(target->X - zone->Right_bound) + sqr(target->X - zone->Upper_bound));
        double temp4 =sqrt(sqr(target->X - zone->Right_bound) + sqr(target->X - zone->Lower_bound));
        temp1 = (temp1 > temp2 ? temp2:temp1);
        temp3 = (temp3 > temp4 ? temp4:temp3);
        zone->dis = (temp1 > temp3 ? temp3:temp1);
    }
}



void KNN(QuadTree &MyTree,Point *target)
{
    while (!node_heap.empty())
    {
        QuadTreeNode *top = node_heap.top();
        node_heap.pop();
        if (top->zone[1] != NULL)
        {
            for (int i =1; i<=4; i++)
            {
                Get_Node_Distance(target,top->zone[i]);
                node_heap.push(top->zone[i]);
            }
        }
        else
        {
            vector<Point*>::iterator it;
            for (it = top->container.begin(); it < top->container.end(); it++)
            {
                double dis_p = (*it)->GetPointDis(target);
                if (point_heap.size() < neighbor_limit)
                    point_heap.push((*it));
                else
                {
                    if (dis_p < point_heap.top()->Distance)
                    {
                        point_heap.pop();
                        point_heap.push((*it));
                    }
                }
            }
        }
        if (point_heap.size() == neighbor_limit)
        {
            if (node_heap.top()->dis > point_heap.top()->Distance)
                break;
            else
                continue;
        }
    }
    while (!point_heap.empty())
    {
        point_heap.top()->print();
        cout<<"the dis is "<<point_heap.top()->Distance<<endl;
        point_heap.pop();
    }
}



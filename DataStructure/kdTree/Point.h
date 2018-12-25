#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED
#include <iostream>
#include <math.h>
#define sqr(x) ((x)*(x))

class Point
{
public:
    double X,Y,Distance;
    int whichzone;
    Point(double x,double y):X(x),Y(y),Distance(0){};
    double GetPointDis(Point *other);
    void print();
};


double Point::GetPointDis(Point *other)
{
    this->Distance = sqrt(sqr(this->X - other->X) + sqr(this->Y - other->Y));
    return this->Distance;
}

void Point::print()
{
    std::cout<<" X = "<<X<<" Y = "<<Y<<std::endl;
}





#endif // POINT_H_INCLUDED

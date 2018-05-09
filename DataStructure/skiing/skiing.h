#ifndef SKIING_H_INCLUDED
#define SKIING_H_INCLUDED
static int dir[2][4]={1,0,-1,0,0,1,0,-1};

class skining
{
public:
    int field[50][50];
    int Row;
    int Column;
    int len;
    int dis_sk[50][50];
    skining();
    void calculate(int R,int C,int milage);
    int test_bound(int R,int C);
    /////
    int in_bound(int R,int C);
    int dis(int i,int j);
    /////
};

#endif // SKIING_H_INCLUDED

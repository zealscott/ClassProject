#ifndef LIFEGAME_H_INCLUDED
#define LIFEGAME_H_INCLUDED

const int maxrow=20,maxcol=20;

class life{
public :
    void initialize();
    void print();
    void update();
private:
    int grif[maxrow+2][maxcol+2];
    int neighbor_count(int row,int col);
};
#endif // LIFEGAME_H_INCLUDED

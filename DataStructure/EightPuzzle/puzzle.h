#ifndef PUZZLE_H_INCLUDED
#define PUZZLE_H_INCLUDED
#include <string>
#include <vector>
#include <set>
using namespace std;

typedef struct Node *node;
struct Node
{
    string status;
    int pre;     //上一个节点的位置
    int step;    //花费步数
};



class matrix
{
private:
    vector<node> select1;
    vector<node> select2;
    set<string> SetString1;
    set<string> SetString2;
    int cur;
public:
    matrix();
    void printResult(int ans);
    int judge(string start);   //每一次交换，逆序对奇偶性不变（0不算在内），因此逆序对的奇偶性不变
    void printString(string now);
    int BFS(vector<node> &select,int N,set<string> &st);
    int ExistIn(int n,string now,vector<node> &select);
    int findZreo(string now);
    void process();
    string swep(string now,int i,int j);
};






#endif // PUZZLE_H_INCLUDED

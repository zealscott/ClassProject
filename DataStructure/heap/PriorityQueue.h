#ifndef PRIORITYQUEUE_H_INCLUDED
#define PRIORITYQUEUE_H_INCLUDED
#include <queue>

std::priority_queue<int> mypq;


mypq.push(30);
mypq.push(100);
mypq.push(25);
mypq.push(40);

std::cout << "Popping out elements...";
while (!mypq.empty())
{
    std::cout << ' ' << mypq.top();
    mypq.pop();
}
std::cout << '\n';


#endif // PRIORITYQUEUE_H_INCLUDED

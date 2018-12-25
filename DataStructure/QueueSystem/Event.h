#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED
#include "allheader.h"
using namespace std;

struct Event {
    double next_occur_time;
    // 使用 -1 表示到达事件, >=0 表示离开事件, 同时数值表示所离开的服务窗口
    int event_type=-1;
    double during_time;
};





#endif // EVENT_H_INCLUDED

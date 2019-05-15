#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED
#include "allheader.h"

class QueueSystem
{
public:
    // 初始化队列系统
    QueueSystem(double total_service_time, int window_num);
    // 启动模拟
    void simulate(int simulate_num);

    double getAvgStayTime()
    {
        avg_stay_time=customer_stay_time/total_customer_num;
        return avg_stay_time;
    }
    double getAvgCustomers()
    {
        avg_customers=total_service_time/total_customer_num;
        return avg_customers;
    }

private:
    // 让队列系统运行一次
    double run();

    // 初始化各种参数
    void init();

    // 获得空闲窗口索引
    int getIdleServiceWindow();

    // 处理顾客到达事件
    void customerArrived();

    // 处理顾客离开事件
    void customerDeparture();

    // 服务窗口的总数
    int window_num;

    // 总的营业时间
    double total_service_time;

    // 顾客的逗留总时间
    double customer_stay_time;

    // 总顾客数
    int total_customer_num;

    // 核心成员
    queue<Customer> customer_queue;
    queue<Event>  event_list;
    queue<ServiceWindow> windows[5];

    // 给外部调用的结果
    double avg_customers;
    double avg_stay_time;

};

void QueueSystem::init()
{
    total_service_time=0;
    avg_customers=0;
    avg_stay_time=0;
    customer_stay_time=0;
    total_customer_num=0;
}

void QueueSystem::QueueSystem(double total_service_time, int window_num)
{
    init();
    run();


}
#endif // SYSTEM_H_INCLUDED

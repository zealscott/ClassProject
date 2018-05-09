#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED
#include "allheader.h"

using namespace std;

enum WindowStatus
{
    SERVICE,
    IDLE,
};

class ServiceWindow
{
public:
    ServiceWindow()
    {
        window_status = IDLE;
    };
    bool isIdle() const
    {
        if (window_status == IDLE)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    void serveCustomer(queue<Customer> customer_queue)
    {
        customer_queue.pop();
        customer=customer_queue.front();
    }
    void setBusy()
    {
        window_status = SERVICE;
    }
    void setIdle()
    {
        window_status = IDLE;
    }
    int getCustomerArriveTime() const
    {
        return customer.arrive_time;
    }
    int getCustomerDuration() const
    {
        return customer.duration;
    }
private:
    Customer customer;
    WindowStatus window_status;
};




#endif // WINDOW_H_INCLUDED

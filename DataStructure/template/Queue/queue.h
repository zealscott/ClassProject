#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED


/* First in First Out*/

#define max_ 100
using namespace std;

template<class M>
class Queue
{
    private:
         int font,rear,cunt;
         M *number;
    public:
        Queue();
        ~Queue();
        bool isempty();
        void inQueue(M &item);
        M outQueue();
};

template<class M>
Queue<M>::Queue()
{
    font=0;
    rear=max_-1;
    cunt=0;
    number=new M[max_];
}

template<class M>
Queue<M>::~Queue()
{
    delete number;
}

template<class M>
bool Queue<M>::isempty()
{
    return cunt==0;
}


template<class M>
void Queue<M>::inQueue(M &item)
{
    cunt++;
    rear=((rear+1==max_)?0:rear+1);
    number[rear]=item;
}

template<class M>
M Queue<M>::outQueue()
{
    M temp;
    cunt--;
    temp=number[font];
    font=((font+1==max_)?0:font+1);
    return temp;
}


#endif // QUEUE_H_INCLUDED

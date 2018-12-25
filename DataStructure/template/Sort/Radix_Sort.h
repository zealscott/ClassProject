#ifndef RADIX_SORT_H_INCLUDED
#define RADIX_SORT_H_INCLUDED
#include "List.cpp"
#include <queue>
#include <string>
using namespace std;

class Record
{
public:
    int key_size;
    string key;
    char key_letter(int position)const;
    Record(int i);
    Record();
    friend ostream& operator<<(ostream & os,const Record &In);
};

template <class Record>
class Sortable_list:public List<Record>
{
public:
    void radix_sort();
    void test(){};
private:
    void rethread(queue<Record>  queues[]);
};

const int max_chars = 28;

int alphabetic_order(char c)
{
    if (c == ' ') return 0;
    if (c >= 'a' && c <= 'z') return c-'a'+1;
    if (c >= 'A' && c <= 'Z') return c-'A'+1;
    return 27;
}

ostream& operator<<(ostream & os,const Record &In)
{
    os<<In.key;
    return os;
}

Record::Record(int i)
{
    cin>>key;
    key_size=int(key.size());
}

Record::Record(){}

char Record::key_letter(int position)const
{
    if (position > key_size -1) return ' ';
    else return key[position];
}


template<class Record>
void Sortable_list<Record>::rethread(queue<Record> queues[] )
{
    Record data;
    for (int i=0;i<max_chars;i++)
    {
        while (!queues[i].empty())
        {
            data=queues[i].front();
            this->Insert(this->Size(),data);
            queues[i].pop();
        }
    }
}

template<class Record>
void Sortable_list<Record>::radix_sort()
{
    Record data;
    int max_position=0;
    queue<Record> queues[max_chars];
    ///////////////////
    for (int i=0;i < this->Count;i++)
    {
        int temp=this->entry[i].key_size;
        if (temp>max_position)
            max_position=temp;
    }
    ////////////
    for (int position = max_position - 1;position >= 0;position--)
    {
        while (this->Remove(0,data) == success)
        {
            int queue_number = alphabetic_order(data.key_letter(position));
            queues[queue_number].push(data);
        }
        rethread(queues);
    }
}

#endif // RADIX_SORT_H_INCLUDED

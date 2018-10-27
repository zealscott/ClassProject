#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

enum Error_code{underflow,overflow,range_error_new,success};
const int max_list = 30;

template<class List_entry>
class List
{
public:
    List();
    int Size()const;
    bool full()const;
    bool Empty()const;
    void Clear();
    void Traverse(void (*visit)(List_entry &));
    Error_code Retrieve(int position, List_entry &x)const;
    Error_code Replace(int position, const List_entry &x);
    Error_code Remove(int position, List_entry &x);
    Error_code Insert(int position,const List_entry &x);
protected:
    int Count;
    List_entry entry[max_list];
};

#endif // LIST_H_INCLUDED

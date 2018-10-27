


enum Balance_factor{left_higher,equal_height,right_higher};

template <class Entry>
struct Binary_node{
// data members
    Entry data;
    Binary_node <Entry> *left;
    Binary_node <Entry> *right;
// constructors
    Binary_node();
    Binary_node(const Entry &x);
// virtual methods
    virtual void set_balance(Balance_factor b);
    virtual Balance_factor get_balance()const;
};

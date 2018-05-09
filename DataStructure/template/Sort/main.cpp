#include <iostream>
#include "Radix_Sort.h"
//#include "Radix_Sort.cpp"


using namespace std;
int main()
{
    Sortable_list<Record> ml;
    for (int i=0; i<9; i++)
    {
        Record temp(1);
        ml.Insert(i,temp);
    }
    ml.radix_sort();
    ml.Traverse(print);
    return 0;
}


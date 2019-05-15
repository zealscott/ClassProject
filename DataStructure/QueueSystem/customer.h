#ifndef CUSTOMER_H_INCLUDED
#define CUSTOMER_H_INCLUDED

#include "allheader.h"

struct Customer {
    double next_occur_time=Random::uniform();
    double arrive_time=Random::uniform();
    double duration=Random::uniform();
};



#endif // CUSTOMER_H_INCLUDED

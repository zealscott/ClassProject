#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED
#include "allheader.h"
class Random {
public:
    // [0, 30) 之间的服从均匀分布的随机值
    static double uniform(double max = 0.5) {
        return ((double)std::rand() / (RAND_MAX))*max;
    }
};


#endif // RANDOM_H_INCLUDED

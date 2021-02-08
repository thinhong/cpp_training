//
// Created by thinh on 08/02/2021.
//

#ifndef MAIN_CPP_BERNOULLIDISTRIBUTION_H
#define MAIN_CPP_BERNOULLIDISTRIBUTION_H

#include "Distribution.h"

class BernoulliDistribution: public Distribution {
private:
    double successRate;
    size_t maxDay;
    std::vector<double> cumulativeProb;
public:
    explicit BernoulliDistribution(double successRate);
    void calcCumulativeProb();
    double getCumulativeProb(size_t index);
    size_t getMaxDay();
};


#endif //MAIN_CPP_BERNOULLIDISTRIBUTION_H

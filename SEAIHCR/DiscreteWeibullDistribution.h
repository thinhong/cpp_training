//
// Created by thinh on 08/02/2021.
//

#ifndef MAIN_CPP_DISCRETEWEIBULLDISTRIBUTION_H
#define MAIN_CPP_DISCRETEWEIBULLDISTRIBUTION_H

#include "Distribution.h"

class DiscreteWeibullDistribution: public Distribution {
private:
    double scale;
    double shape;
    size_t maxDay;
    std::vector<double> cumulativeProb;
public:
    DiscreteWeibullDistribution(double scale, double shape);
    void calcCumulativeProb();
    double getCumulativeProb(size_t index);
    size_t getMaxDay();
};


#endif //MAIN_CPP_DISCRETEWEIBULLDISTRIBUTION_H

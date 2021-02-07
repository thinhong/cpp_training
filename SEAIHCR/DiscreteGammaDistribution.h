//
// Created by thinh on 05/02/2021.
//

#ifndef MAIN_CPP_DISCRETEGAMMADISTRIBUTION_H
#define MAIN_CPP_DISCRETEGAMMADISTRIBUTION_H

#include "Distribution.h"

class DiscreteGammaDistribution: public Distribution {
private:
    double scale;
    double shape;
    size_t maxDay;
    std::vector<double> cumulativeProb;
public:
    DiscreteGammaDistribution(double scale, double shape);
    void calcCumulativeProb();
    double getCumulativeProb(size_t index);
    size_t getMaxDay();
};


#endif //MAIN_CPP_DISCRETEGAMMADISTRIBUTION_H

//
// Created by thinh on 05/03/2021.
//

#ifndef MAIN_CPP_DISCRETEEXPONENTIALDISTRIBUTION_H
#define MAIN_CPP_DISCRETEEXPONENTIALDISTRIBUTION_H

#include "Distribution.h"

class DiscreteExponentialDistribution: public Distribution {
private:
    std::string distName {"exponential"};
    double rate;
    size_t maxDay;
    std::vector<double> transProb;
    void calcTransProb();
public:
    explicit DiscreteExponentialDistribution(double rate);
    std::string getDistName() override;
    double getTransProb(size_t index) override;
    size_t getMaxDay() override;
    double getRate();
};


#endif //MAIN_CPP_DISCRETEEXPONENTIALDISTRIBUTION_H

//
// Created by thinh on 08/02/2021.
//

#ifndef MAIN_CPP_BERNOULLIDISTRIBUTION_H
#define MAIN_CPP_BERNOULLIDISTRIBUTION_H

#include <memory>
#include "Distribution.h"

class BernoulliDistribution: public Distribution {
private:
    std::string distName {"bernoulli"};
    std::shared_ptr<double> successRate;
public:
    explicit BernoulliDistribution(std::shared_ptr<double> successRate);
    double getCumulativeProb(size_t index) override;
    size_t getMaxDay() override;
    std::string getDistName() override;
    void setForceInfection(double transRate, double popSize, double totalInfectious);
};


#endif //MAIN_CPP_BERNOULLIDISTRIBUTION_H

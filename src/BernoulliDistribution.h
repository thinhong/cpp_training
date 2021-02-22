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
    size_t maxDay;
    std::vector<std::shared_ptr<double>> cumulativeProb;
public:
    explicit BernoulliDistribution(std::shared_ptr<double> successRate);
    void calcCumulativeProb();
    double getCumulativeProb(size_t index);
    size_t getMaxDay();
    std::string getDistName() override;
};


#endif //MAIN_CPP_BERNOULLIDISTRIBUTION_H

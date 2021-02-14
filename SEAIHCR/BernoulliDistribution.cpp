//
// Created by thinh on 08/02/2021.
//

#include "BernoulliDistribution.h"

BernoulliDistribution::BernoulliDistribution(std::shared_ptr<double> successRate) {
    this->successRate = successRate;
    maxDay = 1;
}

void BernoulliDistribution::calcCumulativeProb() {
    cumulativeProb.push_back(successRate);
}

double BernoulliDistribution::getCumulativeProb(size_t index) {
    if (index > cumulativeProb.size()) {
        return 1;
    } else {
        return *cumulativeProb[index];
    }
}

size_t BernoulliDistribution::getMaxDay() {
    return maxDay;
}
//
// Created by thinh on 08/02/2021.
//

#include "BernoulliDistribution.h"

BernoulliDistribution::BernoulliDistribution(std::shared_ptr<double> successRate) {
    this->successRate = successRate;
}

double BernoulliDistribution::getTransitionProb(size_t index) {
    return *successRate;
}

size_t BernoulliDistribution::getMaxDay() {
    return 1;
}

std::string BernoulliDistribution::getDistName() {
    return distName;
}
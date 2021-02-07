//
// Created by thinh on 05/02/2021.
//

#include "prob.hpp"
#include "DiscreteGammaDistribution.h"

DiscreteGammaDistribution::DiscreteGammaDistribution(double scale, double shape) {
    this->scale = scale;
    this->shape = shape;
}

void DiscreteGammaDistribution::calcCumulativeProb() {
    double tempProb {0};
    size_t i {0};
    while (tempProb < (1 - Distribution::errorTolerance)) {
        tempProb = gamma_cdf(i, 0, scale, shape);
        cumulativeProb.push_back(tempProb);
        ++i;
    }
    maxDay = cumulativeProb.size();
}

double DiscreteGammaDistribution::getCumulativeProb(size_t index) {
    if (index > cumulativeProb.size()) {
        return 1;
    } else {
        return cumulativeProb[index];
    }
}

size_t DiscreteGammaDistribution::getMaxDay() {
    return maxDay;
}
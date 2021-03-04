//
// Created by thinh on 05/02/2021.
//

#include "../prob.h"
#include "DiscreteGammaDistribution.h"

DiscreteGammaDistribution::DiscreteGammaDistribution(double scale, double shape) {
    this->scale = scale;
    this->shape = shape;
    this->calcCumulativeProb();
}

DiscreteGammaDistribution::DiscreteGammaDistribution(std::vector<double> &cumulativeProb) {
    this->cumulativeProb = cumulativeProb;
    this->maxDay = cumulativeProb.size();
}

void DiscreteGammaDistribution::calcCumulativeProb() {
    double tempProb {0};
    size_t i {0};
    while (tempProb <= (1 - Distribution::errorTolerance)) {
        // https://people.sc.fsu.edu/~jburkardt/cpp_src/prob/prob.cpp
        // A controls the location of the peak;  A is often chosen to be 0.0.
        // B is the "scale" parameter; 0.0 < B, and is often 1.0.
        // C is the "shape" parameter; 0.0 < C, and is often 1.0.
        tempProb = gamma_cdf(i, 0, scale, shape);
        cumulativeProb.push_back(tempProb);
        ++i;
    }
    cumulativeProb.push_back(1);
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

std::string DiscreteGammaDistribution::getDistName() {
    return distName;
}

double DiscreteGammaDistribution::getScale() {
    return scale;
}

double DiscreteGammaDistribution::getShape() {
    return shape;
}
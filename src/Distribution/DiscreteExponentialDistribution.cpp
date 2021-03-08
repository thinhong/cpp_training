//
// Created by thinh on 05/03/2021.
//

#include <iostream>
#include "DiscreteExponentialDistribution.h"
#include "../prob.h"

DiscreteExponentialDistribution::DiscreteExponentialDistribution(double rate) {
    this->rate = rate;
    this->calcCumulativeProb();
}

void DiscreteExponentialDistribution::calcCumulativeProb() {
    double tempProb {0};
    size_t i {0};
    while (tempProb <= (1 - Distribution::errorTolerance)) {
        // https://people.sc.fsu.edu/~jburkardt/cpp_src/prob/prob.cpp
        // A controls the location of the peak;  A is often chosen to be 0.0.
        // B is the "scale" parameter; 0.0 < B, and is often 1.0.
        // C is the "shape" parameter; 0.0 < C, and is often 1.0.
        tempProb = exponential_cdf(i, 0, 1/rate);
        cumulativeProb.push_back(tempProb);
        ++i;
    }
    cumulativeProb.erase(cumulativeProb.begin());
    cumulativeProb.push_back(1);
    maxDay = cumulativeProb.size();
    for (auto i: cumulativeProb) {
        std::cout << i <<  ' ';
    }
    std::cout << "\n";
}

std::string DiscreteExponentialDistribution::getDistName() {
    return distName;
}

double DiscreteExponentialDistribution::getCumulativeProb(size_t index) {
    if (index > cumulativeProb.size()) {
        return 1;
    } else {
        return cumulativeProb[index];
    }
}

size_t DiscreteExponentialDistribution::getMaxDay() {
    return maxDay;
}

double DiscreteExponentialDistribution::getRate() {
    return rate;
}
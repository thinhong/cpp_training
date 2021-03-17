//
// Created by thinh on 05/02/2021.
//

#include "../prob.h"
#include "DiscreteGammaDistribution.h"
#include <iostream>

void DiscreteGammaDistribution::calcTransProb() {
    // First, generate cumulative probability
    double tempProb {0};
    std::vector<double> cumulativeProb;
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

    // Then compute P(0 < waiting time <= 1) by cdf(1) - cdf(0)
    std::vector<double> waitingTime;
    for (size_t j {0}; j < (cumulativeProb.size() - 1); ++j) {
        tempProb = cumulativeProb[j + 1] - cumulativeProb[j];
        waitingTime.push_back(tempProb);
    }

    // Finally, compute transProb using waiting time
    for (size_t k {0}; k < waitingTime.size(); ++k) {
        transProb.push_back(calcProbHelper(waitingTime, k));
    }

    for (auto b: transProb) {
        std::cout << b << "\n";
    }

    // Remember to calculate max day
    maxDay = transProb.size();
    std::cout << maxDay << "\n";
}

DiscreteGammaDistribution::DiscreteGammaDistribution(double scale, double shape) {
    this->scale = scale;
    this->shape = shape;
    this->calcTransProb();
}

DiscreteGammaDistribution::DiscreteGammaDistribution(std::vector<double> &cumulativeProb) {
    this->transProb = cumulativeProb;
    this->maxDay = cumulativeProb.size();
}

double DiscreteGammaDistribution::getCumulativeProb(size_t index) {
    if (index > transProb.size()) {
        return 1;
    } else {
        return transProb[index];
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
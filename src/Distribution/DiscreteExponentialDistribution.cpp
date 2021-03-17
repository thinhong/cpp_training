//
// Created by thinh on 05/03/2021.
//

#include <iostream>
#include "DiscreteExponentialDistribution.h"
#include "../prob.h"

void DiscreteExponentialDistribution::calcTransProb() {
    // First, generate cumulative probability
    double tempProb {0};
    std::vector<double> cumulativeProb;
    size_t i {0};
    while (tempProb <= (1 - Distribution::errorTolerance)) {
        // https://people.sc.fsu.edu/~jburkardt/cpp_src/prob/prob.cpp
        tempProb = exponential_cdf(i, 0, 1/rate);
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

DiscreteExponentialDistribution::DiscreteExponentialDistribution(double rate) {
    this->rate = rate;
    this->calcTransProb();
}

std::string DiscreteExponentialDistribution::getDistName() {
    return distName;
}

double DiscreteExponentialDistribution::getCumulativeProb(size_t index) {
    if (index > transProb.size()) {
        return 1;
    } else {
        return transProb[index];
    }
}

size_t DiscreteExponentialDistribution::getMaxDay() {
    return maxDay;
}

double DiscreteExponentialDistribution::getRate() {
    return rate;
}
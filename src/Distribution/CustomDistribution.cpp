//
// Created by thinh on 04/03/2021.
//

#include <iostream>
#include <algorithm>
#include <stdexcept>
#include "CustomDistribution.h"

void CustomDistribution::calcTransProb() {
    // Compute transProb using waiting time
    for (size_t k {0}; k < waitingTime.size(); ++k) {
        transProb.push_back(calcTransProbHelper(waitingTime, k));
    }

    for (auto b: transProb) {
        std::cout << b << "\n";
    }

    // Remember to calculate max day
    maxDay = transProb.size();
    std::cout << maxDay << "\n";
}

CustomDistribution::CustomDistribution(std::vector<double> waitingTime) {
    // Make sure that waiting time distribution is a probability distribution (sum = 1)
    double sumWaitingTime {0};
    for (auto& wt: waitingTime) {
        sumWaitingTime += wt;
    }
    if (sumWaitingTime != 1) {
        for (size_t i {0}; i < waitingTime.size(); ++i) {
            waitingTime[i] /= sumWaitingTime;
        }
    }
    this->waitingTime = waitingTime;
    this->calcTransProb();
}

std::string CustomDistribution::getDistName() {
    return distName;
}

double CustomDistribution::getTransProb(size_t index) {
    if (index > transProb.size()) {
        return 1;
    } else {
        return transProb[index];
    }
}

size_t CustomDistribution::getMaxDay() {
    return maxDay;
}
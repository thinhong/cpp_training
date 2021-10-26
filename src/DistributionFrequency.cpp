//
// Created by thinh on 22/09/2021.
//

#include "DistributionFrequency.h"

DistributionFrequency::DistributionFrequency(double frequency) {
    this->frequency = frequency;
}

std::string DistributionFrequency::getDistName() {
    return "frequency";
}

double DistributionFrequency::getTransitionProb(size_t index) {
    return frequency;
}

size_t DistributionFrequency::getMaxDay() {
    return 1;
}
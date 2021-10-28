//
// Created by thinh on 22/09/2021.
//

#include "DistributionConstant.h"

DistributionConstant::DistributionConstant(double frequency) {
    this->frequency = frequency;
}

std::string DistributionConstant::getDistName() {
    return "constant";
}

double DistributionConstant::getTransitionProb(size_t index) {
    return frequency;
}

size_t DistributionConstant::getMaxDay() {
    return 1;
}
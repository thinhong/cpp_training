//
// Created by thinh on 04/03/2021.
//

#include <iostream>
#include <algorithm>
#include <stdexcept>
#include "CustomDistribution.h"

CustomDistribution::CustomDistribution(std::vector<double> &cumulativeProb) {
    // First, check if this can be considered as a vector of cumulative probability
    try {
        if (!std::is_sorted(cumulativeProb.begin(), cumulativeProb.end())) {
            throw std::invalid_argument("Your input is not ascending");
        }
        else if (cumulativeProb[cumulativeProb.size() - 1] != 1) {
            throw std::domain_error("WARNING: Your input custom probability is ascending but not a cumulative "
                                    "distribution (the last value is not 1). Scaling it...");
        }
    }
    catch (std::invalid_argument& l) {
        std::terminate();
    }
    catch (std::domain_error& d) {
        std::vector<double> prob;
        for (auto i: cumulativeProb) {
            prob.push_back(i / cumulativeProb[cumulativeProb.size() - 1]);
        }
        std::cerr << d.what() << "\n";
        std::cerr << "Cumulative probability will be: ";
        for (auto j: prob) {
            std::cerr << j << ' ';
        }
        this->cumulativeProb = prob;
    }
    this->cumulativeProb = cumulativeProb;
    this->maxDay = cumulativeProb.size();
}

std::string CustomDistribution::getDistName() {
    return distName;
}

double CustomDistribution::getCumulativeProb(size_t index) {
    if (index > cumulativeProb.size()) {
        return 1;
    } else {
        return cumulativeProb[index];
    }
}

size_t CustomDistribution::getMaxDay() {
    return maxDay;
}
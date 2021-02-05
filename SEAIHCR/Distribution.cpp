//
// Created by thinh on 05/02/2021.
//

#include "Distribution.h"
#include "prob.hpp"

void Distribution::calcWeight() {
    double temp_weight {0};
    size_t i {0};
    while (temp_weight < threshold) {
        if (dist == "gamma") {
            // A controls the location of the peak;  A is often chosen to be 0.0.
            // B is the "scale" parameter; 0.0 < B, and is often 1.0.
            // C is the "shape" parameter; 0.0 < C, and is often 1.0.
            temp_weight = gamma_cdf(i, param1, param2, param3);
        } else if (dist == "weibull") {
            temp_weight = weibull_cdf(i, param1, param2, param3);
        }
        weight.push_back(temp_weight);
        ++i;
        max_day = weight.size();
    }
}
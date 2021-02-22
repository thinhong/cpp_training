//
// Created by thinh on 05/02/2021.
//

#ifndef MAIN_CPP_DISTRIBUTION_H
#define MAIN_CPP_DISTRIBUTION_H

#include <string>
#include <vector>

class Distribution {
public:
    static inline double errorTolerance;
    virtual std::string getDistName() = 0;
    virtual size_t getMaxDay() = 0;
    virtual double getCumulativeProb(size_t index) = 0;
};

#endif //MAIN_CPP_DISTRIBUTION_H

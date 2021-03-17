//
// Created by thinh on 05/02/2021.
//

#ifndef MAIN_CPP_DISTRIBUTION_H
#define MAIN_CPP_DISTRIBUTION_H

#include <string>
#include <vector>

class Distribution {
public:
    static inline double errorTolerance {0.01};
    virtual std::string getDistName() = 0;
    virtual size_t getMaxDay() = 0;
    virtual double getCumulativeProb(size_t index) = 0;
    /**
     * Helper function to calculate probability (x0, x1, x2...) from waiting time distribution (proportion p0, p1, p2
     * ...)
     * @param waitingTime vector of waiting time distribution
     * @param i index of the probability (e.g x0 is index 0, x1 is index 1...)
     * @return
     */
    double calcProbHelper(std::vector<double> waitingTime, size_t i);
};

#endif //MAIN_CPP_DISTRIBUTION_H

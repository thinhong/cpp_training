//
// Created by thinh on 04/03/2021.
//

#ifndef MAIN_CPP_CUSTOMDISTRIBUTION_H
#define MAIN_CPP_CUSTOMDISTRIBUTION_H

#include <string>
#include <vector>
#include "Distribution.h"

class CustomDistribution: public Distribution {
private:
    std::string distName {"custom"};
    size_t maxDay;
    std::vector<double> cumulativeProb;
public:
    explicit CustomDistribution(std::vector<double>& cumulativeProb);
    std::string getDistName() override;
    double getCumulativeProb(size_t index) override;
    size_t getMaxDay() override;
};


#endif //MAIN_CPP_CUSTOMDISTRIBUTION_H

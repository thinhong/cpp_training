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
    std::vector<double> waitingTime;
    size_t maxDay;
    std::vector<double> transProb;
    void calcTransProb();
public:
    explicit CustomDistribution(std::vector<double> waitingTime);
    double getTransProb(size_t index) override;
    std::string getDistName() override;
    size_t getMaxDay() override;
    std::vector<double> getWaitingTime();
};


#endif //MAIN_CPP_CUSTOMDISTRIBUTION_H

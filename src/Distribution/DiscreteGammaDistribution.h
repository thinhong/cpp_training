//
// Created by thinh on 05/02/2021.
//

#ifndef MAIN_CPP_DISCRETEGAMMADISTRIBUTION_H
#define MAIN_CPP_DISCRETEGAMMADISTRIBUTION_H

#include "Distribution.h"

class DiscreteGammaDistribution: public Distribution {
private:
    std::string distName {"gamma"};
    double scale;
    double shape;
    size_t maxDay;
    std::vector<double> cumulativeProb;
    void calcCumulativeProb();
public:
    DiscreteGammaDistribution(double scale, double shape);
    std::string getDistName() override;
    double getCumulativeProb(size_t index) override;
    size_t getMaxDay() override;
    double getScale();
    double getShape();
};


#endif //MAIN_CPP_DISCRETEGAMMADISTRIBUTION_H
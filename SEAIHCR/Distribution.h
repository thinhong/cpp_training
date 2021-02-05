//
// Created by thinh on 05/02/2021.
//

#ifndef MAIN_CPP_DISTRIBUTION_H
#define MAIN_CPP_DISTRIBUTION_H

#include <string>
#include <vector>

class Distribution {
private:
    std::string dist;
    double param1;
    double param2;
    double param3;
    double threshold;
    size_t max_day;
    std::vector<double> weight;
public:
    Distribution(std::string distName, double inpParam1, double inpParam2, double inpParam3, double inpThreshold):
            dist{distName}, param1{inpParam1}, param2{inpParam2}, param3{inpParam3}, threshold{inpThreshold} {};

    Distribution() = delete;
    Distribution(Distribution& dist) = delete;
    void calcWeight();

    size_t getMaxDay() {return max_day;};
    std::vector<double> getWeight() {return weight;};

    void setAccurate(double inpThreshold) {
        threshold = inpThreshold;
    }
};


#endif //MAIN_CPP_DISTRIBUTION_H

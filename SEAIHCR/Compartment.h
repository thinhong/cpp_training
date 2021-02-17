#ifndef SEAIHCR_COMPARTMENT_H
#define SEAIHCR_COMPARTMENT_H

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include "Distribution.h"

class Compartment {
private:
    std::string name;
    std::vector<double> total;
    std::vector<double> currentValues;
    double outValue {0};
    std::shared_ptr<Distribution> dist;
    std::vector<std::weak_ptr<Compartment>> linkedCompartment;
    double weight {1};
    std::vector<bool> isIn;
public:
    Compartment(std::string name, double initVal);

    Compartment() = delete;
    Compartment(const Compartment& comp) = delete;

    ~Compartment() {
//        std::cout << name << " destructor called." << std::endl;
    }
    static inline double daysFollowUp;
    // Getters
    std::vector<double> getTotal() {return total;};
    std::vector<double> getCurrentValues() {return currentValues;};
    std::string getName() {return name;};
    std::vector<bool> getIsIn() {return isIn;};
    std::vector<std::weak_ptr<Compartment>> getLinkedCompartment() {return linkedCompartment;};

    // Setters
    void setWeight(double weight);
    void setDistribution(std::shared_ptr<Distribution> dist);
    void addLinkedCompartment(std::shared_ptr<Compartment>& linkedCompartment);
    void addIsIn(bool isIn);

    // Method for update total in each iteration
    void updateValue(long iter);

};


#endif //SEAIHCR_COMPARTMENT_H

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
    double outValueNextIter {0};
    std::shared_ptr<Distribution> dist;
    std::vector<std::weak_ptr<Compartment>> linkedCompartment;
    double weight {1};
    std::vector<double> linkedWeight;
    std::vector<bool> isIn;
public:
    Compartment(std::string name, size_t size, double initVal);

    Compartment() = delete;
    Compartment(const Compartment& comp) = delete;

    ~Compartment() {
//        std::cout << name << " destructor called." << std::endl;
    }

    // Getters
    std::vector<double> getTotal() {return total;};
    std::vector<double> getCurrentValues() {return currentValues;};
    std::string getName() {return name;};
    double getWeight() {return weight;};
    std::shared_ptr<Distribution> getDistribution() {return dist;};
    double getOutValue() {return outValueNextIter;};
    std::vector<double> getlinkedWeight() {return linkedWeight;};

    std::vector<bool> getisIn() {return isIn;};
    std::vector<std::weak_ptr<Compartment>> getlinkedCompartment() {return linkedCompartment;};

    // Setters
    void addLinkedCompartment(std::shared_ptr<Compartment>& linkedCompartment);
    void addLinkedWeight(double linkedWeight);
    void addIsIn(bool isIn);
    void setWeight(double weight);
    void setDistribution(std::shared_ptr<Distribution> dist);

    // Method for update total in each iteration
    void updateValue(long iter);

};


#endif //SEAIHCR_COMPARTMENT_H

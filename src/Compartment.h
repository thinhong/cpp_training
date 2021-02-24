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
    std::shared_ptr<Distribution> dist;
    std::vector<std::weak_ptr<Compartment>> linkedCompartment;
    std::vector<double> linkedWeight;
    std::vector<bool> isIn;
    // Variables for computational analyses
    std::vector<double> total;
    std::vector<double> subCompartmentValues;
    double outValue {0};
public:
    Compartment(std::string name, double initVal, std::shared_ptr<Distribution> dist);

    Compartment() = delete;
    Compartment(const Compartment& comp) = delete;

    ~Compartment() {
//        std::cout << name << " destructor called." << std::endl;
    }
    static inline size_t daysFollowUp {200};
    // Getters
    std::vector<double> getTotal();
    std::vector<double> getSubCompartmentValues();
    std::string getName();
    std::vector<bool> getIsIn();
    std::vector<std::weak_ptr<Compartment>> getLinkedCompartment();
    std::shared_ptr<Distribution> getDist();
    std::vector<double> getLinkedWeight();

    // Setters
    void addLinkedWeight(double weight);
    void addLinkedCompartment(std::weak_ptr<Compartment> linkedCompartment);
    void addIsIn(bool isIn);

    // Method for update total in each iteration
    void updateValue(long iter);

};


#endif //SEAIHCR_COMPARTMENT_H

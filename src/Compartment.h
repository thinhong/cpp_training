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
    // Save sumIsIn and sumIsOut as member variables instead of computing each time running updateValue to save computational cost
    size_t sumIsIn;
    size_t sumIsOut;
    // Variables for computational analyses
    std::vector<double> total;
    std::vector<double> subCompartmentValues;
    double outValue {0};
public:
    static inline size_t daysFollowUp {200};
    Compartment(std::string name, double initVal, std::shared_ptr<Distribution> dist);

    Compartment() = delete;
    Compartment(const Compartment& comp) = delete;

    ~Compartment() {
//        std::cout << name << " destructor called." << std::endl;
    }
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

    // Helper functions to calculate sumIsIn and sumIsOut
    void calcSumIsIn();
    void calcSumIsOut();

    // Method for update total in each iteration
    void updateValue(long iter);
};


#endif //SEAIHCR_COMPARTMENT_H

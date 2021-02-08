#ifndef SEAIHCR_COMPARTMENT_H
#define SEAIHCR_COMPARTMENT_H

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include "Distribution.h"

class Compartment {
private:
    std::vector<double> total;
    std::vector<double> current_values;
    double out_value {0};
    std::shared_ptr<Distribution> dist;
    std::string name;
    std::vector<std::weak_ptr<Compartment>> linkedCompartment;
    std::vector<std::shared_ptr<double>> linkedWeight;
    std::vector<bool> isIn;
public:
    Compartment(std::string name, size_t size, double initVal) {
        total.resize(size);
        total[0] = initVal;
        // Note: just for test
        current_values.push_back(initVal);
        this->name = name;
    };

    Compartment() = delete;
    Compartment(const Compartment& comp) = delete;

    ~Compartment() {
//        std::cout << name << " destructor called." << std::endl;
    }

    // Getters
    std::vector<double> getTotal() {return total;};
    std::vector<double> getCurrentValues() {return current_values;};
    std::string getName() {return name;};
    std::shared_ptr<Distribution> getDistribution() {return dist;};
    double getOutValue() {return out_value;};

    // Setters
    void addLinkedCompartment(std::shared_ptr<Compartment>& newLinkCompartment);
    void addLinkedWeight(std::shared_ptr<double>& newLinkWeight);
    void addIsIn(bool isInputComp);
    void setDistribution(std::shared_ptr<Distribution> dist);

    // Method for update total in each iteration
    void updateValue(long iter);

};


#endif //SEAIHCR_COMPARTMENT_H

#ifndef SEAIHCR_COMPARTMENT_H
#define SEAIHCR_COMPARTMENT_H

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include "Distribution/Distribution.h"
#include "muParser/muParser.h"

class Compartment {
private:
    std::string name;
    std::shared_ptr<Distribution> dist;
    std::vector<std::weak_ptr<Compartment>> linkedCompartmentIn;
    std::vector<std::weak_ptr<Compartment>> linkedCompartmentOut;
    std::vector<double> linkedWeight;
    // Save nInNodes and nOutNodes as member variables instead of computing each time running updateValue to save computational cost
    size_t nInNodes {0};
    size_t nOutNodes {0};
    // Variables for computational analyses
    std::vector<double> total;
    std::vector<double> subCompartmentValues;
    double outValue {0};
public:
    static inline size_t timesFollowUp {200};
    Compartment(std::string name, double initVal);

    Compartment() = delete;
    Compartment(const Compartment& comp) = delete;

    ~Compartment() {
//        std::cout << name << " compartment destructor called." << std::endl;
    }
    // Getters
    std::vector<double> getTotal();
    std::vector<double> getSubCompartmentValues();
    std::string getName();
    std::vector<bool> getIsIn();
    std::vector<std::weak_ptr<Compartment>> getLinkedCompartmentIn();
    std::vector<std::weak_ptr<Compartment>> getLinkedCompartmentOut();
    std::shared_ptr<Distribution> getDist();
    std::vector<double> getLinkedWeight();
    size_t getNInNodes();

    // Setters
    void addDistribution(std::shared_ptr<Distribution> dist);
    void addLinkedWeight(double weight);
    void addLinkedCompartmentIn(std::weak_ptr<Compartment> linkedCompartmentIn);
    void addLinkedCompartmentOut(std::weak_ptr<Compartment> linkedCompartmentOut);

    /**
     * Update subCompartmentValues and total at each iteration
     * @param iter
     */
    void updateValue(long iter);

    void updateMath(long iter, std::vector<std::string> paramNames, std::vector<double> paramValues,
                    std::vector<std::string> allCompNames, std::vector<double> allCompValues);

    void updateCompartment(long iter, std::vector<std::string> paramNames, std::vector<double> paramValues,
                           std::vector<std::string> allCompNames, std::vector<double> allCompValues);
};


#endif //SEAIHCR_COMPARTMENT_H

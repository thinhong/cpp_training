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
    std::vector<std::weak_ptr<Compartment>> linkedCompartmentIn;
    // These four vectors (distribution, linkedCompartmentOut, subCompartmentValues, outValues)
    // have the same length and the index is corresponding to each other
    std::vector<std::shared_ptr<Distribution>> distribution;
    std::vector<std::weak_ptr<Compartment>> linkedCompartmentOut;
    std::vector<std::vector<double>> subCompartmentValues;
    std::vector<double> outValues;

    std::vector<double> linkedWeight;
    // Variables for computational analyses
    std::vector<double> total;

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
    std::string getName();
    std::vector<std::weak_ptr<Compartment>> getLinkedCompartmentIn();
    std::vector<std::weak_ptr<Compartment>> getLinkedCompartmentOut();
    std::vector<std::shared_ptr<Distribution>> getDist();
    std::vector<double> getLinkedWeight();

    std::vector<std::vector<double>> getSubCompartmentValues() {return subCompartmentValues;};
    std::vector<double> getOutValues() {return outValues;};

    // Setters
    void addDistribution(std::shared_ptr<Distribution>& dist);
    void addLinkedWeight(double weight);
    void addLinkedCompartmentIn(std::weak_ptr<Compartment>& linkedCompIn);
    void addLinkedCompartmentOut(std::weak_ptr<Compartment>& linkedCompOut);
    // subCompartmentValues and outValues are set after add all distributions
    void setSubCompartmentValues();
    void setOutValues();
    double sumSubCompartmentValues(size_t index);

    /**
     * Update subCompartmentValues and total at each iteration
     * @param iter
     */
    void updateSubComp(long iter, size_t index);

    void updateMath(long iter, size_t index, std::vector<std::string>& paramNames, std::vector<double>& paramValues,
                    std::vector<std::string>& allCompNames, std::vector<double>& allCompValues);

    void updateCompartment(long iter, std::vector<std::string>& paramNames, std::vector<double>& paramValues,
                           std::vector<std::string>& allCompNames, std::vector<double>& allCompValues);
};


#endif //SEAIHCR_COMPARTMENT_H

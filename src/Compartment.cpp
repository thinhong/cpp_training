#include "Compartment.h"

// Constructor
Compartment::Compartment(std::string name, double initVal, std::shared_ptr<Distribution> dist) {
    this->name = name;
    this->dist = dist;
    total.resize(daysFollowUp);
    total[0] = initVal;
    // Each total value is binned into many subCompartmentValues, subCompartmentValues[0] is the initVal
    subCompartmentValues.push_back(initVal);
    subCompartmentValues.resize(this->dist->getMaxDay(), 0);
}

// Define list of getters
std::vector<double> Compartment::getTotal() {
    return total;
}
std::vector<double> Compartment::getSubCompartmentValues() {
    return subCompartmentValues;
}
std::string Compartment::getName() {
    return name;
}
std::vector<bool> Compartment::getIsIn() {
    return isIn;
}
std::vector<std::weak_ptr<Compartment>> Compartment::getLinkedCompartment() {
    return linkedCompartment;
}

std::shared_ptr<Distribution> Compartment::getDist() {
    return dist;
}

std::vector<double> Compartment::getLinkedWeight() {
    return linkedWeight;
}

// Setters
void Compartment::addLinkedCompartment(std::shared_ptr<Compartment>& linkedCompartment) {
    this->linkedCompartment.push_back(linkedCompartment);
}

void Compartment::addLinkedWeight(double weight) {
    linkedWeight.push_back(weight);
}

void Compartment::addIsIn(bool isIn) {
    this->isIn.push_back(isIn);
}

void Compartment::updateValue(long iter) {
    int sumIsIn {0};
    for (auto value: isIn) {
        sumIsIn += value;
    }
    // For all compartments except S and R
    if (subCompartmentValues.size() > 1) {
        outValue = 0;
        // Going backward from subCompartmentValues[n] -> subCompartmentValues[1]
        for (size_t i {subCompartmentValues.size() - 1}; i > 0; --i) {
            outValue += subCompartmentValues[i] * dist->getCumulativeProb(i);
            subCompartmentValues[i] = subCompartmentValues[i - 1] * (1 - dist->getCumulativeProb(i - 1));
        }
        // All of subCompartmentValues[0] will go to subCompartmentValues[1] so initialize subCompartmentValues[0] = 0
        subCompartmentValues[0] = 0;
        // Loop over all linkedCompartment, find the linkedCompartment with isIn == true
        // Let subCompartmentValues[0] = outValue of that linkedCompartment
        // Multiply with linkedWeight for situations such as A -> Ar and I, I -> H_h, H_c and H_d
        for (size_t j {0}; j < linkedCompartment.size(); ++j) {
            if (isIn[j]) {
                subCompartmentValues[0] += linkedCompartment[j].lock()->outValue * linkedWeight[j];
            }
        }
    } else if (subCompartmentValues.size() == 1 && sumIsIn == 0) { // For S compartment (S only has 1 value)
        outValue = subCompartmentValues[0] * dist->getCumulativeProb(0);
        subCompartmentValues[0] -= outValue;
    } else if (subCompartmentValues.size() == 1 && sumIsIn > 0) { // For R compartment, only add people from its coming compartments
        for (size_t j {0}; j < linkedCompartment.size(); ++j) {
            if (isIn[j]) {
                subCompartmentValues[0] += linkedCompartment[j].lock()->outValue * linkedWeight[j];
            }
        }
    }

    // Finally sum up subCompartmentValues of this iteration to obtain total value
    for (auto value: subCompartmentValues) {
        total[iter] += value;
    }
}
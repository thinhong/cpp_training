#include "Compartment.h"

Compartment::Compartment(std::string name, size_t size, double initVal) {
    this->name = name;
    total.resize(size);
    total[0] = initVal;
// Note: just for test
    currentValues.push_back(initVal);
}

void Compartment::addLinkedCompartment(std::shared_ptr<Compartment>& linkedCompartment) {
    this->linkedCompartment.push_back(linkedCompartment);
}

void Compartment::setWeight(double weight) {
    this->weight = weight;
}

void Compartment::addLinkedWeight(double linkedWeight) {
    this->linkedWeight.push_back(linkedWeight);
}

void Compartment::addIsIn(bool isIn) {
    this->isIn.push_back(isIn);
}

void Compartment::setDistribution(std::shared_ptr<Distribution> dist) {
    this->dist = dist;
    currentValues.resize(this->dist->getMaxDay());
}

void Compartment::updateValue(long iter) {
    int sumIsIn {0};
    for (auto value: isIn) {
        sumIsIn += value;
    }
    // For all compartments except S and R
    if (currentValues.size() > 1) {
        outValueNextIter = 0;
        // Going backward from currentValues[n] -> currentValues[1]
        for (size_t i {currentValues.size() - 1}; i > 0; --i) {
            outValueNextIter += currentValues[i] * dist->getCumulativeProb(i);
            currentValues[i] = currentValues[i - 1] * (1 - dist->getCumulativeProb(i - 1));
        }
        // All of currentValues[0] will go to currentValues[1] so initialize currentValues[0] = 0
        currentValues[0] = 0;
        // Loop over all linkedCompartment, find the linkedCompartment with isIn == true
        // Let currentValues[0] = outValueNextIter of that linkedCompartment
        // Multiply with linkedWeight for situations such as A -> Ar and I, I -> H_h, H_c and H_d
        for (size_t j {0}; j < linkedCompartment.size(); ++j) {
            if (isIn[j]) {
                currentValues[0] += linkedCompartment[j].lock()->outValueNextIter * weight;
            }
        }
    } else if (currentValues.size() == 1 && sumIsIn == 0) { // For S compartment (S only has 1 value)
        outValueNextIter = currentValues[0] * dist->getCumulativeProb(0);
        currentValues[0] -= outValueNextIter;
    } else if (currentValues.size() == 1 && sumIsIn > 0) { // For R compartment, only add people from its coming compartments
        for (size_t j {0}; j < linkedCompartment.size(); ++j) {
            if (isIn[j]) {
                currentValues[0] += linkedCompartment[j].lock()->outValueNextIter * weight;
            }
        }
    }

    // Finally sum up the current values of this iteration to total
    for (auto value: currentValues) {
        total[iter] += value;
    }
}
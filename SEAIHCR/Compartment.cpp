#include "Compartment.h"

void Compartment::addLinkedCompartment(std::shared_ptr<Compartment>& newLinkCompartment) {
    linkedCompartment.push_back(newLinkCompartment);
}

void Compartment::addLinkedWeight(std::shared_ptr<double>& newLinkWeight) {
    linkedWeight.push_back(newLinkWeight);
}

void Compartment::addIsIn(bool isInputComp) {
    isIn.push_back(isInputComp);
}

void Compartment::setDistribution(std::shared_ptr<Distribution>& inpDist) {
    dist = inpDist;
}

void Compartment::updateValue(long iter) {
    total[iter] = total[iter - 1];
    int sign {-1};
    for (size_t i = 0; i < linkedCompartment.size(); ++i) {
        if (isIn[i]) {
            sign = 1;
        } else {
            sign = -1;
        }
        total[iter] += sign * (*linkedWeight[i]) * linkedCompartment[i].lock()->total[iter - 1];
    }
}
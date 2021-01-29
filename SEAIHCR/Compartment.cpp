#include "Compartment.h"

void Compartment::setLinkedCompartment(std::shared_ptr<Compartment>& newLinkCompartment) {
    linkedCompartment.push_back(newLinkCompartment);
}

void Compartment::setLinkedWeight(std::shared_ptr<double>& newLinkWeight) {
    linkedWeight.push_back(newLinkWeight);
}

void Compartment::setIsIn(bool isInVal) {
    isIn.push_back(isInVal);
}

void Compartment::setExtraParam(std::shared_ptr<Compartment> &newExtraParam) {
    extraParam.push_back(newExtraParam);
}

void Compartment::updateValue(long iter) {
    value[iter] = value[iter - 1];
    int sign {-1};
    for (size_t i {}; i < linkedCompartment.size(); ++i) {
        if (isIn[i]) {sign = 1;} else {sign = -1;}
        double weight {};
        if (!extraParam.empty()) {
            for (size_t j {}; j < extraParam.size(); ++j) {
                weight += (*linkedWeight[i]) * extraParam[j]->value[iter - 1];
            }
        } else {
            weight = *linkedWeight[i];
        }
        value[iter] += sign * weight * linkedCompartment[i]->value[iter - 1];
    }
}
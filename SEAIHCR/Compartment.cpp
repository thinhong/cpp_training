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

void Compartment::setExtraParam(bool newExtraParam) {
    extraParam.push_back(newExtraParam);
}

double Compartment::updateWeight(std::vector<std::shared_ptr<Compartment>>& otherCompartments, long iter) {
    double updateValue {};
    for (auto comp: otherCompartments) {
        updateValue += comp->value[iter - 1];
    }
    return updateValue;
}

void Compartment::updateValue(long iter, std::vector<std::shared_ptr<Compartment>>& listComps) {
    value[iter] = value[iter - 1];
    int sign {-1};
    for (size_t i {}; i < linkedCompartment.size(); ++i) {
        if (isIn[i]) {sign = 1;} else {sign = -1;}
        double weight {};
        if (extraParam[i]) {
            double updateValue = updateWeight(listComps, iter);
            weight = (*linkedWeight[i]) * updateValue;
        } else {
            weight = (*linkedWeight[i]);
        }
        value[iter] += sign * weight * linkedCompartment[i]->value[iter - 1];
    }
}
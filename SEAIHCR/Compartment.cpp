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

void Compartment::setDistribution(std::shared_ptr<Distribution> dist) {
    this->dist = dist;
    current_values.resize(this->dist->getMaxDay());
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

//void Compartment::updateValue(long iter) {
//    total[iter] = total[iter - 1];
//    out_value = current_values[current_values.size() - 1] * dist->getCumulativeProb(0);
//    int sign {-1};
//    for (size_t i = 0; i < linkedCompartment.size(); ++i) {
//        if (isIn[i]) {sign = 1;} else {sign = -1;}
//        for (size_t j {current_values.size() - 2}; j > 0; --j) {
//
//        }
//        total[iter] += sign * (*linkedWeight[i]) * linkedCompartment[i].lock()->total[iter - 1];
//    }
//}
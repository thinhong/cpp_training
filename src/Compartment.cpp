#include "Compartment.h"

// Constructor
Compartment::Compartment(std::string name, double initVal, std::shared_ptr<Distribution> dist) {
    this->name = name;
    this->dist = dist;
    total.resize(timesFollowUp);
    total[0] = initVal;
    // Each total value is binned into many subCompartmentValues, subCompartmentValues[0] is the initVal
    subCompartmentValues.resize(this->dist->getMaxDay(), 0);
    subCompartmentValues[0] = initVal;
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

size_t Compartment::getNInNodes() {
    return nInNodes;
}

// Setters
void Compartment::addLinkedCompartment(std::weak_ptr<Compartment> linkedCompartment) {
    this->linkedCompartment.push_back(linkedCompartment);
}

void Compartment::addLinkedWeight(double weight) {
    linkedWeight.push_back(weight);
}

void Compartment::addIsIn(bool isIn) {
    this->isIn.push_back(isIn);
    if (isIn) {
        nInNodes += 1;
    } else {
        nOutNodes += 1;
    }
}

void Compartment::updateValue(long iter) {
    outValue = 0;
    // Going backward from subCompartmentValues[n] -> subCompartmentValues[1]
    size_t startIndex {0};
    if (iter < (subCompartmentValues.size() - 1)) {
        startIndex = iter + 1;
    } else {
        startIndex = subCompartmentValues.size() - 1;
    }
    for (size_t i {startIndex}; i > 0; --i) {
        outValue += subCompartmentValues[i] * dist->getTransitionProb(i);
        subCompartmentValues[i] = subCompartmentValues[i - 1] * (1 - dist->getTransitionProb(i - 1));
    }
    outValue += subCompartmentValues[0] * dist->getTransitionProb(0);
}

void Compartment::updateMath(long iter, std::vector<std::string> paramNames, std::vector<double> paramValues,
                             std::vector<std::string> allCompNames, std::vector<double> allCompValues) {
    mu::Parser parser;
    parser.SetExpr(dist->getDistName());
    // Add parameter values
    for (size_t i {0}; i < paramNames.size(); ++i) {
        parser.DefineVar(paramNames[i], &paramValues[i]);
    }
    // Add compartment values
    for (size_t i {0}; i < allCompNames.size(); ++i) {
        parser.DefineVar(allCompNames[i], &allCompValues[i]);
    }
    outValue = parser.Eval();
}

void Compartment::updateCompartment(long iter, std::vector<std::string> paramNames, std::vector<double> paramValues,
                                    std::vector<std::string> allCompNames, std::vector<double> allCompValues) {
    double inValue {0};
    // Loop over all linkedCompartment, find the linkedCompartment with isIn == true
    // Let subCompartmentValues[0] += outValue of that linkedCompartment
    // Multiply with linkedWeight for situations such as A -> Ar and I, I -> H_h, H_c and H_d
    for (size_t m {0}; m < linkedCompartment.size(); ++m) {
        if (isIn[m]) {
            inValue += linkedCompartment[m].lock()->outValue * linkedWeight[m];
        }
    }
    if (dist->getDistName() == "gamma" || dist->getDistName() == "weibull" ||
        dist->getDistName() == "exponential" || dist->getDistName() == "transitionProb" ||
        dist->getDistName() == "custom") {
        updateValue(iter);
    } else {
        updateMath(iter, paramNames, paramValues, allCompNames, allCompValues);
    }
    subCompartmentValues[0] = inValue;
    total[iter] = total[iter - 1] + inValue - outValue;

    //    // Finally sum up subCompartmentValues of this iteration to obtain total value
    //    for (auto& value: subCompartmentValues) {
    //        total[iter] += value;
    //    }
}
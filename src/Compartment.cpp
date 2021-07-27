#include "Compartment.h"

// Constructor
Compartment::Compartment(std::string name, double initVal) {
    this->name = name;
    total.resize(timesFollowUp);
    total[0] = initVal;
}

void Compartment::addDistribution(std::shared_ptr<Distribution>& dist) {
    this->distribution.push_back(dist);
}

void Compartment::setSubCompartmentValues() {
    subCompartmentValues.resize(distribution.size());
    for (size_t i {0}; i < distribution.size(); ++i) {
        subCompartmentValues[i].resize(distribution[i]->getMaxDay(), 0);
        subCompartmentValues[i][0] = total[0];
    }
}

double Compartment::sumSubCompartmentValues(size_t index) {
    double sumSubComp {0};
    for (auto& value: subCompartmentValues[index]) {
        sumSubComp += value;
    }
    return sumSubComp;
}

void Compartment::setOutValues() {
    outValues.resize(subCompartmentValues.size(), 0);
}

// Define list of getters
std::vector<double> Compartment::getTotal() {
    return total;
}

std::string Compartment::getName() {
    return name;
}
std::vector<std::weak_ptr<Compartment>> Compartment::getLinkedCompartmentIn() {
    return linkedCompartmentIn;
}
std::vector<std::weak_ptr<Compartment>> Compartment::getLinkedCompartmentOut() {
    return linkedCompartmentOut;
}

std::vector<std::shared_ptr<Distribution>> Compartment::getDist() {
    return distribution;
}

std::vector<double> Compartment::getLinkedWeight() {
    return linkedWeight;
}

// Setters
void Compartment::addLinkedCompartmentIn(std::weak_ptr<Compartment>& linkedCompIn) {
    this->linkedCompartmentIn.push_back(linkedCompIn);
}

void Compartment::addLinkedCompartmentOut(std::weak_ptr<Compartment>& linkedCompOut) {
    this->linkedCompartmentOut.push_back(linkedCompOut);
}

void Compartment::addLinkedWeight(double weight) {
    linkedWeight.push_back(weight);
}

void Compartment::updateSubComp(long iter, size_t index) {
    outValues[index] = 0;
    // Going backward from subCompartmentValues[n] -> subCompartmentValues[1]
    // This startIndex is to reduce the number of calculations
    size_t startIndex {0};
    if (iter < (subCompartmentValues[index].size() - 1)) {
        startIndex = iter + 1;
    } else {
        startIndex = subCompartmentValues[index].size() - 1;
    }
    for (size_t i {startIndex}; i > 0; --i) {
        outValues[index] += subCompartmentValues[index][i] * distribution[index]->getTransitionProb(i);
        subCompartmentValues[index][i] = subCompartmentValues[index][i - 1] * (1 - distribution[index]->getTransitionProb(i - 1));
    }
    outValues[index] += subCompartmentValues[index][0] * distribution[index]->getTransitionProb(0);
    subCompartmentValues[index][0] = 0;
}

void Compartment::updateMath(long iter, size_t index, std::vector<std::string>& paramNames, std::vector<double>& paramValues,
                             std::vector<std::string>& allCompNames, std::vector<double>& allCompValues) {
    mu::Parser parser;
    parser.SetExpr(distribution[index]->getDistName());
    // Add parameter values
    for (size_t i {0}; i < paramNames.size(); ++i) {
        parser.DefineVar(paramNames[i], &paramValues[i]);
    }
    // Add compartment values
    for (size_t i {0}; i < allCompNames.size(); ++i) {
        parser.DefineVar(allCompNames[i], &allCompValues[i]);
    }
    outValues[index] = parser.Eval();
}

void Compartment::updateCompartment(long iter, std::vector<std::string>& paramNames, std::vector<double>& paramValues,
                                    std::vector<std::string>& allCompNames, std::vector<double>& allCompValues) {
    for (size_t index {0}; index < distribution.size(); ++index) {
        // If it is the first distribution, do nothing, from the second we have to subtract outValues of the previous
        if (index >= 1) {
            // Calculate sum of previous outValues
            double sumPreviousOutValues {0};
            for (size_t p {0}; p < index - 1; ++p) {
                sumPreviousOutValues += outValues[p];
            }
            // Compute the percentage of values that remain
            double remainPct = (total[iter - 1] - sumPreviousOutValues) / total[iter - 1];
            for (size_t v {0}; v < subCompartmentValues[index].size(); ++v) {
                subCompartmentValues[index][v] *= remainPct;
            }
        }
        if (distribution[index]->getDistName() == "gamma" || distribution[index]->getDistName() == "weibull" ||
            distribution[index]->getDistName() == "exponential" || distribution[index]->getDistName() == "transitionProb" ||
            distribution[index]->getDistName() == "custom") {
            updateSubComp(iter, index);
        } else {
            updateMath(iter, index, paramNames, paramValues,
                       allCompNames, allCompValues);
        }
    }
    // When finish the process with all distributions, calculate the total outValues
    double sumOutValues {0};
    for (auto& outVal: outValues) {
        sumOutValues += outVal;
    }
    // Again compute the percentage of values that remain
    double remainPct = (total[iter - 1] - sumOutValues) / total[iter - 1];
    if (total[iter - 1] == 0) {
        remainPct = 0;
    }
    for (size_t v {0}; v < subCompartmentValues[0].size(); ++v) {
        subCompartmentValues[0][v] *= remainPct;
    }

    // And compute the inValue
    double inValue {0};
    // Loop over all linkedCompartmentIn
    for (size_t m {0}; m < linkedCompartmentIn.size(); ++m) {
        // Find the linkedCompartmentOut of these linkedCompartmentIn
        for (size_t o {0}; o < linkedCompartmentIn[m].lock()->linkedCompartmentOut.size(); ++o) {
            // Based on name, find the outValue that correspond to this compartment and add to inValue
            if (name == linkedCompartmentIn[m].lock()->linkedCompartmentOut[o].lock()->getName()) {
                inValue += linkedCompartmentIn[m].lock()->outValues[o] * linkedWeight[m];
                // Note: Check linkedWeight[m] or [o]!!!!!!!!!!!!
            }
        }
    }
    for (size_t i {0}; i < subCompartmentValues.size(); ++i) {
        subCompartmentValues[i][0] = inValue;
    }
    total[iter] = total[iter - 1] + inValue - sumOutValues;
}
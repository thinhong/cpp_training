#include "Compartment.h"

// Constructor
Compartment::Compartment(std::string compName, double initVal) {
    this->compName = compName;
    compTotal.resize(timesFollowUp);
    compTotal[0] = initVal;
}

void Compartment::addOutDistribution(std::shared_ptr<Distribution> dist) {
    this->outDistributions.push_back(dist);
}

void Compartment::setOutValues() {
    outValues.resize(outDistributions.size(), 0);
}

void Compartment::setLengthSubCompartment() {
    size_t maxLength {0};
    for (auto& outDist: outDistributions) {
        if (outDist->getMaxDay() > maxLength) {
            maxLength = outDist->getMaxDay();
        }
    }
    subCompartmentValues.resize(maxLength);
    subCompartmentValues[0] = compTotal[0];
}

// Define list of getters
std::string Compartment::getCompName() {
    return compName;
}

std::vector<std::weak_ptr<Compartment>> Compartment::getInCompartments() {
    return inCompartments;
}

std::vector<std::weak_ptr<Compartment>> Compartment::getOutCompartments() {
    return outCompartments;
}

std::vector<std::shared_ptr<Distribution>> Compartment::getOutDistributions() {
    return outDistributions;
}

std::vector<double> Compartment::getOutWeights() {
    return outWeights;
}

std::vector<double> Compartment::getCompTotal() {
    return compTotal;
}

// Setters
void Compartment::addInCompartment(std::weak_ptr<Compartment>& linkedCompIn) {
    this->inCompartments.push_back(linkedCompIn);
}

void Compartment::addOutCompartment(std::weak_ptr<Compartment>& linkedCompOut) {
    this->outCompartments.push_back(linkedCompOut);
}

void Compartment::addOutWeight(double weight) {
    outWeights.push_back(weight);
}

void Compartment::updateSubCompByDist(long iter, size_t outIndex) {
    outValues[outIndex] = 0;
    // Going backward from subCompartmentValues[n] -> subCompartmentValues[1]
    // This startIndex is to reduce the number of calculations
    size_t startIndex {0};
    if (iter < (subCompartmentValues.size() - 1)) {
        startIndex = iter + 1;
    } else {
        startIndex = subCompartmentValues.size() - 1;
    }
    for (size_t i {startIndex}; i > 0; --i) {
        outValues[outIndex] += subCompartmentValues[i] * outDistributions[outIndex]->getTransitionProb(i);
        subCompartmentValues[i] = subCompartmentValues[i - 1] * (1 - outDistributions[outIndex]->getTransitionProb(i - 1));
    }
    outValues[outIndex] += subCompartmentValues[0] * outDistributions[outIndex]->getTransitionProb(0);
    subCompartmentValues[0] = 0;
}

void Compartment::updateSubCompByMath(size_t outIndex, std::vector<std::string>& paramNames, std::vector<double>& paramValues,
                                      std::vector<std::string>& allCompNames, std::vector<double>& allCompValues) {
    mu::Parser parser;
    parser.SetExpr(outDistributions[outIndex]->getDistName());
    // Add parameter values
    for (size_t i {0}; i < paramNames.size(); ++i) {
        parser.DefineVar(paramNames[i], &paramValues[i]);
    }
    // Add compartment values
    for (size_t i {0}; i < allCompNames.size(); ++i) {
        parser.DefineVar(allCompNames[i], &allCompValues[i]);
    }
    // The result of this math expression is the outValues of this outIndex
    outValues[outIndex] = parser.Eval();
    // Now calculate how many people remain in each subCompartment
    double sumSubComp {0};
    for (size_t i_subComp {0}; i_subComp < subCompartmentValues.size(); ++i_subComp) {
        sumSubComp += subCompartmentValues[i_subComp];
    }
    double remainPct = (sumSubComp - outValues[outIndex]) / sumSubComp;
    for (size_t i_subComp {0}; i_subComp < subCompartmentValues.size(); ++i_subComp) {
        subCompartmentValues[i_subComp] *= remainPct;
    }
}

void Compartment::updateCompartment(long iter, std::vector<std::string>& paramNames, std::vector<double>& paramValues,
                                    std::vector<std::string>& allCompNames, std::vector<double>& allCompValues) {
    for (size_t outIndex {0}; outIndex < outCompartments.size(); ++outIndex) {
        if (outDistributions[outIndex]->getDistName() == "gamma" ||
            outDistributions[outIndex]->getDistName() == "weibull" ||
            outDistributions[outIndex]->getDistName() == "exponential" ||
            outDistributions[outIndex]->getDistName() == "transitionProb" ||
            outDistributions[outIndex]->getDistName() == "custom") {
            updateSubCompByDist(iter, outIndex);
        } else {
            updateSubCompByMath(outIndex, paramNames, paramValues,
                                allCompNames, allCompValues);
        }
    }
    // Add the inValue to subCompartmentValues[0]
    double inValue {0};
    // Loop over all inCompartments
    for (size_t inIndex {0}; inIndex < inCompartments.size(); ++inIndex) {
        // Find the outCompartments of these inCompartments
        for (size_t outOfIn {0}; outOfIn < inCompartments[inIndex].lock()->outCompartments.size(); ++outOfIn) {
            // Based on name, find the outValue that correspond to this compartment and add to inValue
            if (compName == inCompartments[inIndex].lock()->outCompartments[outOfIn].lock()->getCompName()) {
                inValue += outWeights[inIndex] * inCompartments[inIndex].lock()->outValues[outOfIn];
            }
        }
    }
    subCompartmentValues[0] = inValue;
    // Calculate compTotal = compTotal of the previous iteration + inValue - sumOutValues
    double sumOutValues {0};
    for (size_t outIndex {0}; outIndex < outValues.size(); ++outIndex) {
        sumOutValues += outValues[outIndex];
    }
    compTotal[iter] = compTotal[iter - 1] + inValue - sumOutValues;
}
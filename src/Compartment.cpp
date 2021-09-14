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
    outValuesTotal.resize(outDistributions.size(), 0);
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
    // outSubCompartment is a clone of subCompartmentValues
    outSubCompValues.resize(maxLength);
    outSubCompValues[0] = compTotal[0];
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
    outValuesTotal[outIndex] = 0;
    // Going backward from subCompartmentValues[n] -> subCompartmentValues[1]
    // This startIndex is to reduce the number of calculations
    size_t startIndex {0};
    if (iter < (subCompartmentValues.size() - 1)) {
        startIndex = iter + 1;
    } else {
        startIndex = subCompartmentValues.size() - 1;
    }
    // Put if outside to check condition only once
    if (outWeights[outIndex] == 1) {
        for (size_t i {0}; i <= startIndex; ++i) {
            outValuesTotal[outIndex] += subCompartmentValues[startIndex - i] * outDistributions[outIndex]->getTransitionProb(startIndex - i);
            subCompartmentValues[startIndex - i] -= outSubCompValues[startIndex - i];
            subCompartmentValues[startIndex - i] *= (1 - outDistributions[outIndex]->getTransitionProb(startIndex - i));
        }
        // After finishing, clean the outSubCompValues vector
        std::fill(outSubCompValues.begin(), outSubCompValues.end(), 0);
    } else if (outWeights[outIndex] < 1) {
        for (size_t i {0}; i <= startIndex; ++i) {
            outValuesTotal[outIndex] += subCompartmentValues[startIndex - i] * outDistributions[outIndex]->getTransitionProb(startIndex - i);
            outSubCompValues[startIndex - i] += outWeights[outIndex] * subCompartmentValues[startIndex - i] * outDistributions[outIndex]->getTransitionProb(startIndex - i);
        }
    }

    // Update compTotal after finish this outSubComp
    compTotal[iter] -= outValuesTotal[outIndex];
}

void Compartment::updateSubCompByMath(long iter, size_t outIndex, std::vector<std::string>& paramNames, std::vector<double>& paramValues,
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
    // The result of this math expression is the outValuesTotal of this outIndex
    outValuesTotal[outIndex] = outWeights[outIndex] * parser.Eval();

    // If outWeight = 1 then calculate directly in the subCompartment
    if (outWeights[outIndex] == 1) {
        // Calculate how many people remain in each subCompartment
        double sumSubComp {0};
        for (size_t i_subComp {0}; i_subComp < subCompartmentValues.size(); ++i_subComp) {
            sumSubComp += subCompartmentValues[i_subComp];
        }
        double remainPct = (sumSubComp - outValuesTotal[outIndex]) / sumSubComp;
        for (size_t i_subComp {0}; i_subComp < subCompartmentValues.size(); ++i_subComp) {
            subCompartmentValues[i_subComp] *= remainPct;
        }
    } else if (outWeights[outIndex] < 1) {
        // If weight < 1 then perform it on the outSubCompValues
        double sumSubComp {0};
        for (size_t i_subComp {0}; i_subComp < subCompartmentValues.size(); ++i_subComp) {
            sumSubComp += subCompartmentValues[i_subComp];
        }
        double outPct = outValuesTotal[outIndex] / sumSubComp;
        for (size_t i_subComp {0}; i_subComp < subCompartmentValues.size(); ++i_subComp) {
            outSubCompValues[i_subComp] = outPct * subCompartmentValues[i_subComp];
        }
    }

    // Update compTotal after finish this outSubComp
    compTotal[iter] -= outValuesTotal[outIndex];
}

void Compartment::updateCompartment(long iter, std::vector<std::string>& paramNames, std::vector<double>& paramValues,
                                    std::vector<std::string>& allCompNames, std::vector<double>& allCompValues) {

    compTotal[iter] = compTotal[iter - 1];
    std::fill(outSubCompValues.begin(), outSubCompValues.end(), 0);

    for (size_t outIndex {0}; outIndex < outCompartments.size(); ++outIndex) {
        if (outDistributions[outIndex]->getDistName() == "gamma" ||
            outDistributions[outIndex]->getDistName() == "weibull" ||
            outDistributions[outIndex]->getDistName() == "exponential" ||
            outDistributions[outIndex]->getDistName() == "transitionProb" ||
            outDistributions[outIndex]->getDistName() == "custom") {
            updateSubCompByDist(iter, outIndex);
        } else {
            updateSubCompByMath(iter, outIndex, paramNames, paramValues,
                                allCompNames, allCompValues);
        }
    }

    size_t n_subComp = subCompartmentValues.size();
    if (n_subComp == 1) {
        subCompartmentValues[0] -= outSubCompValues[0];
    } else {
        for (size_t i_subComp {0}; i_subComp < n_subComp; ++i_subComp) {
            if (i_subComp == n_subComp) {
                subCompartmentValues[n_subComp - i_subComp] = 0;
            } else {
                subCompartmentValues[n_subComp - i_subComp] = subCompartmentValues[n_subComp - i_subComp - 1] - outSubCompValues[n_subComp - i_subComp - 1];
            }
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
                inValue += inCompartments[inIndex].lock()->outValuesTotal[outOfIn];
            }
        }
    }
    subCompartmentValues[0] += inValue;
    compTotal[iter] += inValue;
}
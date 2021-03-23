#include "Model.h"
#include <algorithm>
#include <stack>
#include <stdexcept>

Model::Model(std::string name, double transmissionRate, std::vector<std::string> infectiousComps) {
    this->name = name;
    this->transmissionRate = transmissionRate;
    this->infectiousComps = infectiousComps;
}

std::string Model::getName() {
    return name;
}

std::vector<std::shared_ptr<Compartment>> Model::getComps() {
    return comps;
}

double Model::getPopulationSize() {
    return populationSize;
}

std::vector<std::string> Model::getInfectiousComps() {
    return infectiousComps;
}

double Model::getTransmissionRate() {
    return transmissionRate;
}

std::vector<std::weak_ptr<Model>> Model::getLinkedLocation() {
    return linkedLocations;
}

std::vector<double> Model::getLocationInteraction() {
    return locationInteraction;
}

double Model::getSelfInteraction() {
    return selfInteraction;
}

void Model::addLocationInteraction(std::vector<double> locationInteraction) {
    this->locationInteraction = locationInteraction;
    // Calculate selfInteraction = 1 - sum(locationInteraction)
    double totalInteraction {0};
    for (auto& i: this->locationInteraction) {
        totalInteraction += i;
    }
    this->selfInteraction = 1 - totalInteraction;
}

void Model::addLinkedLocation(std::weak_ptr<Model> linkedLocation) {
    this->linkedLocations.push_back(linkedLocation);
}

void Model::addCompsFromConfig(std::vector<std::shared_ptr<Compartment>> &comps) {
    this->comps = comps;
}

void Model::addCompsAndConnect(std::shared_ptr<Compartment>& A, std::shared_ptr<Compartment>& B, double weight) {
    std::vector<std::string> compName;
    if (!comps.empty()) {
        for (auto& comp: comps) {
            compName.push_back(comp->getName());
        }
    }
    if (!(std::find(compName.begin(), compName.end(), A->getName()) != compName.end())) {
        comps.push_back(A);
    }
    if (!(std::find(compName.begin(), compName.end(), B->getName()) != compName.end())) {
        comps.push_back(B);
    }
    A->addLinkedCompartment(B);
    B->addLinkedCompartment(A);
    A->addIsIn(false);
    B->addIsIn(true);
    A->addLinkedWeight(1);
    B->addLinkedWeight(weight);
}

int Model::getIndex(std::shared_ptr<Compartment> comp) {
    auto it = find(comps.begin(), comps.end(), comp);
    int index {-1};
    if (it != comps.end()) {
        index = it - comps.begin();
    }
    return index;
}

bool Model::checkCycleHelper(size_t i, std::vector<bool> &visited, std::vector<bool> &recursiveStack) {
    if (!visited[i]) {
        visited[i] = true;
        recursiveStack[i] = true;
        for (size_t j {0}; j < comps[i]->getLinkedCompartment().size(); ++j) {
            int index = getIndex(comps[i]->getLinkedCompartment()[j].lock());
            if (!comps[i]->getIsIn()[j] && !visited[index] && checkCycleHelper(index, visited, recursiveStack)) {
                return true;
            }
            else if (!comps[i]->getIsIn()[j] && recursiveStack[index]) {
                return true;
            }
        }
    }
    recursiveStack[i] = false;
    return false;
}

void Model::checkCycle() {
    std::vector<bool> visited;
    visited.resize(comps.size(), false);
    std::vector<bool> recursiveStack;
    recursiveStack.resize(comps.size(), false);
    for (size_t i {0}; i < comps.size(); ++i) {
        try {
            if (checkCycleHelper(i, visited, recursiveStack)) {
                throw std::logic_error("A cycle exists in your model");
            }
        }
        catch (std::logic_error& e) {
            std::terminate();
        }
    }
}

void Model::sortCompsHelper(size_t i, std::vector<bool> &visited, std::stack<std::shared_ptr<Compartment>> &stack) {
    visited[i] = true;
    for (size_t j {0}; j < comps[i]->getLinkedCompartment().size(); ++j) {
        if (!comps[i]->getIsIn()[j]) {
            int index = getIndex(comps[i]->getLinkedCompartment()[j].lock());
            if (!visited[index]) {
                sortCompsHelper(index, visited, stack);
            }
        }
    }
    stack.push(comps[i]);
}

void Model::sortComps() {
    checkCycle();
    std::stack<std::shared_ptr<Compartment>> stack;
    std::vector<std::shared_ptr<Compartment>> sortedComps;
    std::vector<bool> visited;
    visited.resize(comps.size(), false);
    for (size_t i{0}; i < comps.size(); ++i) {
        if (!visited[i]) {
            sortCompsHelper(i, visited, stack);
        }
    }
    while (!stack.empty()) {
        sortedComps.push_back(stack.top());
        stack.pop();
    }
    comps = sortedComps;
}

void Model::calcPopulationSize() {
    for (auto& comp: comps) {
        populationSize += comp->getTotal()[0];
    }
}

double Model::calcForceInfection(size_t iter) {
    // Force of infection: lambda = beta * totalInfectious / N

    // Within this location
    double totalInfectiousWithin {0};
    // Scan through all compartments, find compartments have the same name as in the infectiousComps vector
    for (auto& comp: comps) {
        for (std::string &infectiousComp: infectiousComps) {
            if (comp->getName() == infectiousComp) {
                totalInfectiousWithin += comp->getTotal()[iter - 1];
            }
        }
    }
    double infectiousWithin = totalInfectiousWithin / populationSize;

    // Between locations
    double infectiousBetween {0};
    if (!linkedLocations.empty()) {
        // Start from the first linked location, perform the same process as within location
        for (size_t i {0}; i < linkedLocations.size(); ++i) {
            double totalInfectiousBetween {0};
            for (auto& linkedLocationComp: linkedLocations[i].lock()->getComps()) {
                for (std::string& infectiousComp: linkedLocations[i].lock()->getInfectiousComps()) {
                    if (linkedLocationComp->getName() == infectiousComp) {
                        totalInfectiousBetween += linkedLocationComp->getTotal()[iter - 1];
                    }
                }
            }
            // Remember to multiply the locationInteraction
            infectiousBetween += locationInteraction[i] * totalInfectiousBetween / linkedLocations[i].lock()->getPopulationSize() ;
        }
    }
    double forceInfection = transmissionRate * (infectiousWithin + infectiousBetween);
    return forceInfection;
}

void Model::update(long iter) {
    double forceInfection = calcForceInfection(iter);
    for (auto& comp: comps) {
        comp->updateValue(iter, forceInfection);
    }
}
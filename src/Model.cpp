#include "Model.h"
#include <algorithm>
#include <stack>

std::vector<std::shared_ptr<Compartment>> Model::getComps() {
    return comps;
}

void Model::addCompsFromConfig(std::vector<std::shared_ptr<Compartment>> &comps) {
    this->comps = comps;
}

// For example S -> I, after addCompsAndConnect(S, I):
// S has: linkedCompartment = S, linkedWeight = weight_StoI, isIn = false (S-> is moving out of S)
// I has: linkedCompartment = S, linkedWeight = weight_StoI, isIn = true (->I is moving to I)
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
        if (checkCycleHelper(i, visited, recursiveStack)) {
            isCycle = true;
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
    if (isCycle) {
        std::cout << "A cycle exists in your model." << "\n";
    } else {
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
    // Now we can assure that the model has been constructed well, calculate sumIsIn and sumIsOut here
    for (auto& comp: comps) {
        // We have to call calcSumIsIn before calling calcSumIsOut because calcSumIsOut = isIn.size() - sumIsIn
        comp->calcSumIsIn();
        comp->calcSumIsOut();
    }
}

void Model::update(long iter) {
    for (auto& comp: comps) {
        comp->updateValue(iter);
    }
}

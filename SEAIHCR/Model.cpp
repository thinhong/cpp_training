#include "Model.h"
#include <algorithm>

std::vector<std::shared_ptr<Compartment>> Model::getComps() {
    return comps;
}

// For example S -> I, after addCompsAndConnect(S, I):
// S has: linkedCompartment = S, linkedWeight = weight_StoI, isIn = false (S-> is moving out of S)
// I has: linkedCompartment = S, linkedWeight = weight_StoI, isIn = true (->I is moving to I)
void Model::addCompsAndConnect(std::shared_ptr<Compartment> &A, std::shared_ptr<Compartment> &B) {
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
}

int Model::getIndex(std::shared_ptr<Compartment>&& comp) {
    auto it = find(comps.begin(), comps.end(), comp);
    int index {-1};
    if (it != comps.end()) {
        index = it - comps.begin();
    }
    return index;
}

bool Model::DFS_helper(size_t i, std::vector<bool> &visited, std::vector<bool> &recursiveStack) {
    if (!visited[i]) {
        visited[i] = true;
        recursiveStack[i] = true;
        for (size_t j {0}; j < comps[i]->getLinkedCompartment().size(); ++j) {
            int index = getIndex(comps[i]->getLinkedCompartment()[j].lock());
            if (!comps[i]->getIsIn()[j] && !visited[index] && DFS_helper(index, visited, recursiveStack)) {
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

void Model::DFS() {
    std::vector<bool> visited;
    visited.resize(comps.size(), false);
    std::vector<bool> recursiveStack;
    recursiveStack.resize(comps.size(), false);
    for (size_t i {0}; i < comps.size(); ++i) {
        if (DFS_helper(i, visited, recursiveStack)) {
            isCycle = true;
        }
    }
}

void Model::sortComps() {
    if (isCycle) {
        std::cout << "A cycle exists in your model, please reconstruct it." << "\n";
    } else {
        // Find S and R
        for (size_t i {0}; i < comps.size(); ++i) {
            int sumIsIn {0};
            for (auto j: comps[i]->getIsIn()) {
                sumIsIn += j;
            }

            // Find S and shift S to the first position
            if (sumIsIn == 0) {
                auto tmp = comps[0];
                comps[0] = comps[i];
                comps[i] = tmp;
            }

            // Find R and shift R to the last position
            if (sumIsIn > 1) {
                auto tmp = comps[comps.size() - 1];
                comps[comps.size() - 1] = comps[i];
                comps[i] = tmp;
            }
        }

        // Sort the compartments between S and R
        // Start from the first compartment (S)
        for (size_t i {0}; i < comps.size() - 1; ++i) {
            // Get its linked compartment
            for (auto& linked: comps[i]->getLinkedCompartment()) {
                // Search from the next compartment, if there is a compartment with the same name
                // as linkedCompartment, switch it next to the current (i.e i + 1)
                for (size_t j {i + 1}; j < comps.size() - 1; ++j) {
                    if (comps[j]->getName() == linked.lock()->getName()) {
                        auto tmp = comps[i + 1];
                        comps[i + 1] = comps[j];
                        comps[j] = tmp;
                    }
                }
            }
        }
    }
}

void Model::update(long iter) {
    for (auto& comp: comps) {
        comp->updateValue(iter);
    }
}

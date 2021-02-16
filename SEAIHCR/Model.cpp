#include "Model.h"
#include <algorithm>

// For example S -> I, after connect(S, I):
// S has: linkedCompartment = S, linkedWeight = weight_StoI, isIn = false (S-> is moving out of S)
// I has: linkedCompartment = S, linkedWeight = weight_StoI, isIn = true (->I is moving to I)
void Model::connect(std::shared_ptr<Compartment> &A, std::shared_ptr<Compartment> &B) {
    std::vector<std::string> compName;
    if (!comps.empty()) {
        for (auto comp: comps) {
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

void Model::update(long iter) {
    for (auto comp: comps) {
        comp->updateValue(iter);
    }
}

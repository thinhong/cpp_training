#include "Model.h"

void Model::setComps(std::shared_ptr<Compartment> &pComp) {
    comps.push_back(pComp);
}

// For example S -> I, after connect(S, I):
// S has: linkedCompartment = S, linkedWeight = weight_StoI, isIn = false (S-> is moving out of S)
// I has: linkedCompartment = S, linkedWeight = weight_StoI, isIn = true (->I is moving to I)
void Model::connect(std::shared_ptr<Compartment> &A, std::shared_ptr<Compartment> &B,
                    std::shared_ptr<double> &weight) {
    A->setLinkedCompartment(A);
    B->setLinkedCompartment(A);
    A->setLinkedWeight(weight);
    B->setLinkedWeight(weight);
    A->setIsIn(false);
    B->setIsIn(true);
}

void Model::update() {
    for (size_t i {1}; i < comps[0]->getValue().size(); ++i) {
        for (auto comp: comps) {
            comp->updateValue(i);
        }
    }
}

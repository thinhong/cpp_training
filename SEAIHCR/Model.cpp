#include "Model.h"

void Model::setComps(std::shared_ptr<Compartment> &pComp) {
    comps.push_back(pComp);
}

// For example S -> I, after connect(S, I):
// S has: linkedCompartment = S, linkedWeight = weight_StoI, isIn = false (S-> is moving out of S)
// I has: linkedCompartment = S, linkedWeight = weight_StoI, isIn = true (->I is moving to I)
void Model::connect(std::shared_ptr<Compartment> &A, std::shared_ptr<Compartment> &B,
                    std::shared_ptr<double> &weight, bool newExtraParam) {
    A->addLinkedCompartment(A);
    B->addLinkedCompartment(A);
    A->addLinkedWeight(weight);
    B->addLinkedWeight(weight);
    A->addIsIn(false);
    B->addIsIn(true);
}

void Model::update(long iter) {
    for (auto comp: comps) {
        comp->updateValue(iter);
    }
}

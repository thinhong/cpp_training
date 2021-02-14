#include <iostream>
#include <array>
#include "Compartment.h"
#include "Model.h"
#include "Distribution.h"
#include "BernoulliDistribution.h"
#include "DiscreteGammaDistribution.h"
#include "DiscreteWeibullDistribution.h"
#include "FileCSV.h"

int main() {
    // Set error tolerance to all distribution
    Distribution::errorTolerance = 0.01;

    auto transRate = std::make_shared<double>(2.0 / 500000);
    auto forceInfection = std::make_shared<double>();
    auto bernoulli = std::make_shared<BernoulliDistribution>(forceInfection);
    bernoulli->calcCumulativeProb();

    auto removed = std::make_shared<double>(0.0);
    auto bernoulli_removed = std::make_shared<BernoulliDistribution>(removed);
    bernoulli_removed->calcCumulativeProb();

    auto gamma1 = std::make_shared<DiscreteGammaDistribution>(1, 2);
    gamma1->calcCumulativeProb();

    auto weibull1 = std::make_shared<DiscreteWeibullDistribution>(1, 1.5);
    weibull1->calcCumulativeProb();

    // Setup compartments
    auto S = std::make_shared<Compartment>("S", 1000, 100000);
    auto E = std::make_shared<Compartment>("E", 1000, 0);
    auto A = std::make_shared<Compartment>("A", 1000, 0);
    auto A_r = std::make_shared<Compartment>("A_r", 1000, 0);
    auto I = std::make_shared<Compartment>("I", 1000, 1);
    auto H_h = std::make_shared<Compartment>("H_h", 1000, 0);
    auto H_c = std::make_shared<Compartment>("H_c", 1000, 0);
    auto H_d = std::make_shared<Compartment>("H_d", 1000, 0);
    auto C_c = std::make_shared<Compartment>("C_c", 1000, 0);
    auto C_d = std::make_shared<Compartment>("C_d", 1000, 0);
    auto D = std::make_shared<Compartment>("D", 1000, 0);
    auto R = std::make_shared<Compartment>("R", 1000, 0);

    A_r->setWeight(0.2);
    I->setWeight(0.8);
    H_h->setWeight(0.5);
    H_c->setWeight(0.35);
    H_d->setWeight(0.15);

    S->setDistribution(bernoulli);
    E->setDistribution(gamma1);
    A->setDistribution(weibull1);
    A_r->setDistribution(gamma1);
    I->setDistribution(gamma1);
    H_h->setDistribution(weibull1);
    H_c->setDistribution(gamma1);
    H_d->setDistribution(gamma1);
    C_c->setDistribution(weibull1);
    C_d->setDistribution(gamma1);
    D->setDistribution(bernoulli_removed);
    R->setDistribution(bernoulli_removed);

    // Model myModel consists of S, I and R
    Model myModel;
    myModel.setComps(S);
    myModel.setComps(E);
    myModel.setComps(A);
    myModel.setComps(A_r);
    myModel.setComps(I);
    myModel.setComps(H_h);
    myModel.setComps(H_c);
    myModel.setComps(H_d);
    myModel.setComps(C_c);
    myModel.setComps(C_d);
    myModel.setComps(D);
    myModel.setComps(R);

    // Connect S -> E -> A
    myModel.connect(S, E);
    myModel.connect(E, A);
    // A -> A_r and I
    myModel.connect(A, A_r);
    myModel.connect(A, I);
    // A_r -> R
    myModel.connect(A_r, R);
    // I -> H_h, H_c and H_d
    myModel.connect(I, H_h);
    myModel.connect(I, H_c);
    myModel.connect(I, H_d);
    // H_h -> R
    myModel.connect(H_h, R);
    // H_c -> C_c -> R
    myModel.connect(H_c, C_c);
    myModel.connect(C_c, R);
    // H_d -> C_d -> D
    myModel.connect(H_d, C_d);
    myModel.connect(C_d, D);

    // Update model
    for (size_t i {1}; i < 1000; i++) {
        *forceInfection = (*transRate) * ((*A).getTotal()[i - 1] + (*A_r).getTotal()[i - 1] + (*I).getTotal()[i - 1]);
        myModel.update(i);
        std::cout << "Iteration: " << i << std::endl;
        for (size_t j {0}; j < myModel.getComps().size(); ++j) {
            std::cout << myModel.getComps()[j]->getName() << ": ";
            for (auto k: myModel.getComps()[j]->getCurrentValues()) {
                std::cout << k << " ";
            }
            std::cout << std::endl;
        }
    }

    // File output
    Model* pModel = &myModel;
    FileCSV file("/home/thinh/Downloads", "test.csv");
    file.setModel(pModel);
    file.writeFile();

}

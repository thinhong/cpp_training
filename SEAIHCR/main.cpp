#include <iostream>
#include <array>
#include <fstream>
#include "json.hpp"
#include "Compartment.h"
#include "Model.h"
#include "Distribution.h"
#include "BernoulliDistribution.h"
#include "DiscreteGammaDistribution.h"
#include "DiscreteWeibullDistribution.h"
#include "FileCSV.h"

int main() {
    const double nDays {1000};

    Compartment::daysFollowUp = nDays;
    // Set error tolerance to all distribution
    Distribution::errorTolerance = 0.01;

    // Read a JSON config file to setup all compartments
    std::ifstream inp("/home/thinh/Dropbox/oucru/cpp/cpp_training/compartment.json");
    nlohmann::json compConfig;
    inp >> compConfig;

//    auto transRate = std::make_shared<double>();
//    auto forceInfection = std::make_shared<double>();
//    std::vector<std::shared_ptr<Compartment>> comps;
//    for (auto& comp: compConfig) {
//        auto tmp = std::make_shared<Compartment>(comp["name"], 1000, comp["initialValue"]);
//        if (comp["distribution"].empty()) {
//            *transRate = comp["transRate"];
//            auto bernoulli_S = std::make_shared<BernoulliDistribution>(forceInfection);
//            bernoulli_S->calcCumulativeProb();
//            tmp->setDistribution(bernoulli_S);
//        } else if (comp["distribution"]["name"] == "gamma") {
//            auto gamma = std::make_shared<DiscreteGammaDistribution>(comp["distribution"]["scale"], comp["distribution"]["shape"]);
//            gamma->calcCumulativeProb();
//            tmp->setDistribution(gamma);
//        } else if (comp["distribution"]["name"] == "weibull") {
//            auto weibull = std::make_shared<DiscreteWeibullDistribution>(comp["distribution"]["scale"], comp["distribution"]["shape"]);
//            weibull->calcCumulativeProb();
//            tmp->setDistribution(weibull);
//        }
//        if (!comp["weight"].empty()) {
//            tmp->setWeight(comp["weight"]);
//        }
//        comps.push_back(tmp);
//    }

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
    auto S = std::make_shared<Compartment>("S", 100000);
    auto E = std::make_shared<Compartment>("E", 0);
    auto A = std::make_shared<Compartment>("A", 0);
    auto A_r = std::make_shared<Compartment>("A_r", 0);
    auto I = std::make_shared<Compartment>("I", 1);
    auto H_h = std::make_shared<Compartment>("H_h", 0);
    auto H_c = std::make_shared<Compartment>("H_c", 0);
    auto H_d = std::make_shared<Compartment>("H_d", 0);
    auto C_c = std::make_shared<Compartment>("C_c", 0);
    auto C_d = std::make_shared<Compartment>("C_d", 0);
    auto D = std::make_shared<Compartment>("D", 0);
    auto R = std::make_shared<Compartment>("R", 0);

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

    // Add and also connect all compartments into myModel
    // S -> E -> A
    myModel.addCompsAndConnect(S, E);
    myModel.addCompsAndConnect(E, A);
    // A -> A_r and I
    myModel.addCompsAndConnect(A, A_r);
    myModel.addCompsAndConnect(A, I);
    // A_r -> R
    myModel.addCompsAndConnect(A_r, R);
    // I -> H_h, H_c and H_d
    myModel.addCompsAndConnect(I, H_h);
    myModel.addCompsAndConnect(I, H_c);
    myModel.addCompsAndConnect(I, H_d);
    // H_h -> R
    myModel.addCompsAndConnect(H_h, R);
    // H_c -> C_c -> R
    myModel.addCompsAndConnect(H_c, C_c);
    myModel.addCompsAndConnect(C_c, R);
    // H_d -> C_d -> D
    myModel.addCompsAndConnect(H_d, C_d);
    myModel.addCompsAndConnect(C_d, D);

    // Test with a isCycle: A_r going back to E
    myModel.addCompsAndConnect(A_r, E);
    myModel.DFS();
    std::cout << myModel.getCycle() << "\n";
    myModel.sortComps();

//    myModel.DFS();
//    std::cout << myModel.getCycle() << "\n";
//    myModel.sortComps();

//    for (auto i: myModel.getComps()) {
//        std::cout << i->getName() << ' ';
//    }

//    for (auto i: myModel.getComps()) {
//        std::cout << "Compartment " << i->getName() << std::endl;
//        for (auto j: i->getLinkedCompartment()) {
//            std::cout << j.lock()->getName() << ' ';
//        }
//        std::cout << std::endl;
//        for (auto k: i->getIsIn()) {
//            std::cout << k << ' ';
//        }
//        std::cout << std::endl;
//    }

    // Update model
    for (size_t i {1}; i < myModel.getComps()[0]->daysFollowUp; i++) {
        *forceInfection = (*transRate) * (A->getTotal()[i - 1] + A_r->getTotal()[i - 1] + I->getTotal()[i - 1]);
        myModel.update(i);

        // For debug
//        std::cout << "Iteration: " << i << std::endl;
//        for (size_t j {0}; j < myModel.getComps().size(); ++j) {
//            std::cout << myModel.getComps()[j]->getName() << ": ";
//            for (auto k: myModel.getComps()[j]->getCurrentValues()) {
//                std::cout << k << " ";
//            }
//            std::cout << std::endl;
//        }
    }

    // File output
//    Model* pModel = &myModel;
//    FileCSV file("/home/thinh/Downloads", "test.csv");
//    file.setModel(pModel);
//    file.writeFile();

}

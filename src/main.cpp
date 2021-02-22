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
#include "FileJSON.h"
#include "ObjectJSON.h"

int main() {
    Compartment::daysFollowUp = 200;
    const double populationSize {10000000};
    double transRate {3.0};
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

    auto forceInfection = std::make_shared<double>();
    auto bernoulli = std::make_shared<BernoulliDistribution>(forceInfection);
    bernoulli->calcCumulativeProb();

    auto removed = std::make_shared<double>(0.0);
    auto bernoulli_removed = std::make_shared<BernoulliDistribution>(removed);
    bernoulli_removed->calcCumulativeProb();

    auto gamma_7d = std::make_shared<DiscreteGammaDistribution>(1, 1);
    auto gamma_13d = std::make_shared<DiscreteGammaDistribution>(1, 4);
    auto weil_16d = std::make_shared<DiscreteWeibullDistribution>(5, 1.5);
    auto weil_9d = std::make_shared<DiscreteWeibullDistribution>(4, 3);

    // Setup compartments
    auto S = std::make_shared<Compartment>("S", 3000000, bernoulli);
    auto E = std::make_shared<Compartment>("E", 0, weil_9d);
    auto A = std::make_shared<Compartment>("A", 0, gamma_7d);
    auto A_r = std::make_shared<Compartment>("A_r", 0, weil_16d);
    auto I = std::make_shared<Compartment>("I", 1, gamma_13d);
    auto H_h = std::make_shared<Compartment>("H_h", 0, gamma_7d);
    auto H_c = std::make_shared<Compartment>("H_c", 0, weil_9d);
    auto H_d = std::make_shared<Compartment>("H_d", 0, gamma_7d);
    auto C_c = std::make_shared<Compartment>("C_c", 0, weil_16d);
    auto C_d = std::make_shared<Compartment>("C_d", 0, gamma_13d);
    auto D = std::make_shared<Compartment>("D", 0, bernoulli_removed);
    auto R = std::make_shared<Compartment>("R", 0, bernoulli_removed);

    A_r->setWeight(0.35);
    I->setWeight(0.65);
    H_h->setWeight(0.7);
    H_c->setWeight(0.2);
    H_d->setWeight(0.1);

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
//    myModel.addCompsAndConnect(A_r, E);
    myModel.DFS();
    myModel.sortComps();

    // Update model
    for (size_t i {1}; i < myModel.getComps()[0]->daysFollowUp; i++) {
        *forceInfection = transRate / populationSize * (A->getTotal()[i - 1] + A_r->getTotal()[i - 1] + I->getTotal()[i - 1]);
        myModel.update(i);

        // For debug
        std::cout << "Iteration: " << i << std::endl;
        for (size_t j {0}; j < myModel.getComps().size(); ++j) {
            std::cout << myModel.getComps()[j]->getName() << ": ";
            for (auto k: myModel.getComps()[j]->getSubCompartmentValues()) {
                std::cout << k << " ";
            }
            std::cout << std::endl;
        }
    }

//    nlohmann::json jsonArray;
//    jsonArray += {{"daysFollowUp", Compartment::daysFollowUp}};
//    jsonArray += {{"errorTolerance", Distribution::errorTolerance}};
//    jsonArray += {{"populationSize", populationSize}};
//    jsonArray += {{"transRate", transRate}};
//    for (auto i: myModel.getComps()) {
//        ObjectJSON jsonNode;
//        jsonNode.toJSON(i);
//        jsonArray += jsonNode.getJsonNode();
//    }
//    std::cout << jsonArray;
//
//    std::ofstream myFile("/home/thinh/Downloads/config.json");
//
//    if (myFile.is_open()) {
//        myFile << jsonArray;
//        myFile.close();
//        std::cout << "Successfully written into file: /home/thinh/Downloads/config.json" << std::endl;
//    } else {
//        std::cout << "Unable to open file" << std::endl;
//    }

    // File output
    Model* pModel = &myModel;
    FileCSV file("/home/thinh/Downloads", "test_r0_3_diffWaitingTime_death0.csv", pModel);
    file.writeFile();
//
//    FileJSON json("/home/thinh/Downloads", "test_r0_3_diffWaitingTime.json", pModel);
//    json.writeFile();

}

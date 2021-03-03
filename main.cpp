#include <iostream>
#include <array>
#include <fstream>
#include "src/json.h"
#include "src/Compartment.h"
#include "src/Model.h"
#include "src/Distribution/Distribution.h"
#include "src/Distribution/BernoulliDistribution.h"
#include "src/Distribution/DiscreteGammaDistribution.h"
#include "src/Distribution/DiscreteWeibullDistribution.h"
#include "src/FileCSV.h"
#include "src/FileJSON.h"
#include "src/ObjectJSON.h"

int main() {
    // ========================== Using JSON config ==============================

    // Read a JSON config file to setup all compartments
    std::ifstream configFile("/home/thinh/Downloads/config.json");
    nlohmann::json config;
    configFile >> config;

    // Initialize parameters
    Compartment::daysFollowUp = config["daysFollowUp"];
    const double populationSize = config["populationSize"];
    const double transRate = config["transRate"];
    // Set error tolerance to all distribution
    Distribution::errorTolerance = config["errorTolerance"];
    // Define a vector contains the name of infectious compartments
    std::vector<std::string> infectiousComps = config["infectiousComps"];

    // Automatically generate all compartments from config file
    std::vector<std::shared_ptr<Compartment>> allCompartments;
    for (auto& compConfig: config["compartments"]) {
        std::shared_ptr<Compartment> tmpComp;
        if (compConfig["distribution"]["name"] == "bernoulli") {
            auto bernoulli = std::make_shared<BernoulliDistribution>(std::make_shared<double>(compConfig["distribution"]["successRate"]));
            tmpComp = std::make_shared<Compartment>(compConfig["name"], compConfig["initialValue"], bernoulli);
        } else if (compConfig["distribution"]["name"] == "gamma") {
            auto gamma = std::make_shared<DiscreteGammaDistribution>(compConfig["distribution"]["scale"], compConfig["distribution"]["shape"]);
            tmpComp = std::make_shared<Compartment>(compConfig["name"], compConfig["initialValue"], gamma);
        } else if (compConfig["distribution"]["name"] == "weibull") {
            auto weibull = std::make_shared<DiscreteWeibullDistribution>(compConfig["distribution"]["scale"], compConfig["distribution"]["shape"]);
            tmpComp = std::make_shared<Compartment>(compConfig["name"], compConfig["initialValue"], weibull);
        }
        // Also add linkedWeight and isIn to the compartment since they are all numeric values
        for (double weight: compConfig["linkedWeight"]) {
            tmpComp->addLinkedWeight(weight);
        }
        for (bool isIn: compConfig["isIn"]) {
            tmpComp->addIsIn(isIn);
        }
        allCompartments.push_back(tmpComp);
    }

    // Add linkedCompartment needs to be done as a separated step because we need all compartments created before connecting them
    for (auto& compConfig: config["compartments"]) {
        std::weak_ptr<Compartment> baseComp;
        for (auto& comp: allCompartments) {
            if (comp->getName() == compConfig["name"]) {
                baseComp = comp;
            }
        }
        for (auto& linkedConfig: compConfig["linkedCompartment"]) {
            std::weak_ptr<Compartment> linkedComp;
            for (auto& comp: allCompartments) {
                if (comp->getName() == linkedConfig) {
                    linkedComp = comp;
                    baseComp.lock()->addLinkedCompartment(linkedComp);
                }
            }
        }
    }

    Model myModel;
    myModel.addCompsFromConfig(allCompartments);
    myModel.sortComps();

    // ======================== End JSON config ==============================

    // Update model
    for (size_t i {1}; i < Compartment::daysFollowUp; i++) {
        double totalInfectious {0.0};
        for (auto& comp: myModel.getComps()) {
            for (std::string& iComp: infectiousComps) {
                if (comp->getName() == iComp) {
                    totalInfectious += comp->getTotal()[i - 1];
                }
            }
        }
        for (auto& comp: myModel.getComps()) {
            if (comp->getNInNodes() == 0) {
                std::dynamic_pointer_cast<BernoulliDistribution>(comp->getDist())->setForceInfection(transRate,
                                                                                                     populationSize,
                                                                                                     totalInfectious);
            }
        }
        myModel.update(i);

        // For debug
//        std::cout << "Iteration: " << i << std::endl;
//        for (size_t j {0}; j < myModel.getComps().size(); ++j) {
//            std::cout << myModel.getComps()[j]->getName() << ": ";
//            for (auto k: myModel.getComps()[j]->getSubCompartmentValues()) {
//                std::cout << k << " ";
//            }
//            std::cout << std::endl;
//        }
    }

//    nlohmann::json jsonArray;
//    jsonArray["daysFollowUp"] = Compartment::daysFollowUp;
//    jsonArray["errorTolerance"] = Distribution::errorTolerance;
//    jsonArray["populationSize"] = populationSize;
//    jsonArray["transRate"] = transRate;
//    jsonArray["infectiousComps"] = infectiousComps;
//    for (auto i: myModel.getComps()) {
//        ObjectJSON jsonNode;
//        jsonNode.toJSON(i);
//        jsonArray["compartments"].push_back(jsonNode.getJsonNode());
//    }
//    std::cout << jsonArray;
//
//    std::ofstream myFile("/home/thinh/Downloads/config2.json");
//
//    if (myFile.is_open()) {
//        myFile << jsonArray;
//        myFile.close();
//        std::cout << "Successfully written into file: /home/thinh/Downloads/config2.json" << std::endl;
//    } else {
//        std::cout << "Unable to open file" << std::endl;
//    }

    // File output
//    Model* pModel = &myModel;
//    FileCSV file(config["filePath"], config["fileName"], pModel);
//    file.writeFile();

    Model* pModel = &myModel;
    FileCSV file("/home/thinh/Downloads", "manual_20210302_weil2d.csv", pModel);
    file.writeFile();

}

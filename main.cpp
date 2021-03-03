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
#include "src/CompartmentJSON.h"

int main() {
    // ========================== Using JSON config ==============================
    // Read a JSON config file to setup all compartments
    std::ifstream configFile("../config/config.json");
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
        CompartmentJSON compJson(compConfig);
        allCompartments.push_back(compJson.compFromJSON());
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
    // ======================== End JSON config ==============================

    // ==================== Construct and run model ==========================
    // Construct model
    Model myModel;
    myModel.addCompsFromConfig(allCompartments);
    myModel.sortComps();

    // Run model
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
                std::dynamic_pointer_cast<BernoulliDistribution>(comp->getDist())->setForceInfection(transRate, populationSize, totalInfectious);
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
    // ================== End construct and run model ========================

    // ========================= Write output ================================
    // Create json object for config file
//    nlohmann::json writeConfig;
//    writeConfig["daysFollowUp"] = Compartment::daysFollowUp;
//    writeConfig["errorTolerance"] = Distribution::errorTolerance;
//    writeConfig["populationSize"] = populationSize;
//    writeConfig["transRate"] = transRate;
//    writeConfig["infectiousComps"] = infectiousComps;
//    for (auto i: myModel.getComps()) {
//        CompartmentJSON jsonNode;
//        jsonNode.compToJSON(i);
//        writeConfig["compartments"].push_back(jsonNode.getJsonNode());
//    }
//
//    // Write config file
//    std::ofstream myFile("/home/thinh/Downloads/config2.json");
//    if (myFile.is_open()) {
//        myFile << writeConfig;
//        myFile.close();
//        std::cout << "Successfully written config information into file: /home/thinh/Downloads/config2.json" <<
//        std::endl;
//    } else {
//        std::cout << "Unable to open file" << std::endl;
//    }

    // Write output to CSV file
    Model* pModel = &myModel;
    FileCSV file(config["outputFolder"], config["outputFileName"], pModel);
    file.writeFile();

}

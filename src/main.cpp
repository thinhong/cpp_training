#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <stdexcept>
#include "json.h"
#include "Compartment.h"
#include "Model.h"
#include "Distribution/Distribution.h"
#include "Distribution/BernoulliDistribution.h"
#include "Distribution/DiscreteGammaDistribution.h"
#include "Distribution/DiscreteWeibullDistribution.h"
#include "FileCSV.h"
#include "FileJSON.h"
#include "CompartmentJSON.h"
#include <filesystem>

int main() {
    // ========================== Using JSON input ==============================
    // Read a JSON input file to provide parameters
    std::string inputPath;
    std::cout << "Enter full path to input file (ex: /home/Documents/config.json): ";
    std::cin >> inputPath;
    std::ifstream inputFile(inputPath);
    nlohmann::json input;
    inputFile >> input;

    // Check populationSize = sum of initial values of all compartments
//    double sumAllComps {0};
//    for (auto& compInput: input["compartments"]) {
//        sumAllComps += static_cast<double>(compInput["initialValue"]);
//    }
//    try {
//        if (static_cast<double>(input["populationSize"]) != sumAllComps) {
//            throw std::logic_error("Population size is not equal to sum of all initial values of compartments");
//        }
//    }
//    catch (std::logic_error& oor) {
//        std::terminate();
//    }

    // Initialize parameters
    Compartment::daysFollowUp = input["daysFollowUp"];
    const double populationSize = input["populationSize"];
    const double transRate = input["transRate"];
    // Set error tolerance to all distribution
    Distribution::errorTolerance = input["errorTolerance"];
    // Define a vector contains the name of infectious compartments
    std::vector<std::string> infectiousComps = input["infectiousComps"];

    // Automatically generate all compartments from input file
    std::vector<std::shared_ptr<Compartment>> allCompartments;
    for (auto& compConfig: input["compartments"]) {
        CompartmentJSON compJson(compConfig);
        allCompartments.push_back(compJson.compFromJSON());
    }

    // Add linkedCompartment needs to be done as a separated step because we need all compartments created before connecting them
    for (auto& compConfig: input["compartments"]) {
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
    // ======================== End JSON input ==============================

    // ==================== Construct and run model ==========================
    // Construct model
    Model myModel;
    myModel.addCompsFromConfig(allCompartments);
    myModel.sortComps();

    // Run model
    std::ofstream iterFile("../output/iteration.txt");
    iterFile << "Iteration: " << "0" << std::endl;
    for (size_t j {0}; j < myModel.getComps().size(); ++j) {
        iterFile << myModel.getComps()[j]->getName() << ": ";
        for (auto k: myModel.getComps()[j]->getSubCompartmentValues()) {
            iterFile << k << " ";
        }
        iterFile << std::endl;
    }

    for (size_t i {1}; i < Compartment::daysFollowUp; i++) {
        // Force of infectious: lambda = beta * Y / N, of which Y is the total infectious would change in each iteration
        // Calculate total number of infectious
        double totalInfectious {0.0};
        for (auto& comp: myModel.getComps()) {
            for (std::string& iComp: infectiousComps) {
                if (comp->getName() == iComp) {
                    totalInfectious += comp->getTotal()[i - 1];
                }
            }
        }
        // Then we can calculate force of infectious
        for (auto& comp: myModel.getComps()) {
            if (comp->getNInNodes() == 0) {
                std::dynamic_pointer_cast<BernoulliDistribution>(comp->getDist())->setForceInfection(transRate, populationSize, totalInfectious);
            }
        }
        // Then we can update the model
        myModel.update(i);

        // For debug
        iterFile << "Iteration: " << i << std::endl;
        for (size_t j {0}; j < myModel.getComps().size(); ++j) {
            iterFile << myModel.getComps()[j]->getName() << ": ";
            for (auto k: myModel.getComps()[j]->getSubCompartmentValues()) {
                iterFile << k << " ";
            }
            iterFile << std::endl;
        }
    }
    // ================== End construct and run model ========================

    // ========================= Write output ================================
    // Create json object to store all input parameters
    nlohmann::json writeConfig;
    writeConfig["daysFollowUp"] = Compartment::daysFollowUp;
    writeConfig["errorTolerance"] = Distribution::errorTolerance;
    writeConfig["populationSize"] = populationSize;
    writeConfig["transRate"] = transRate;
    writeConfig["infectiousComps"] = infectiousComps;
    for (auto i: myModel.getComps()) {
        CompartmentJSON jsonNode;
        jsonNode.compToJSON(i);
        writeConfig["compartments"].push_back(jsonNode.getJsonNode());
    }

    // Write input parameters into a file
    std::ofstream myFile("/home/thinh/Downloads/config2.json");
    if (myFile.is_open()) {
        myFile << writeConfig;
        myFile.close();
        std::cout << "Successfully written input information into file: /home/thinh/Downloads/config2.json" <<
        std::endl;
    } else {
        std::cout << "Unable to write file" << std::endl;
    }

    // Write output to CSV file
//    Model* pModel = &myModel;
//    std::string outputFolder;
//    std::string outputFileName;
//    std::cout << "Set path to the folder you want to save output file (ex: /home/Documents): ";
//    std::cin >> outputFolder;
//    std::cout << "Set your output file name (ex: results.csv): ";
//    std::cin >> outputFileName;
//    FileCSV file(outputFolder, outputFileName, pModel);
//    file.writeFile();

    Model* pModel = &myModel;
    FileCSV file("../output/", "testSIR.csv", pModel);
    file.writeFile();
}

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

    // Initialize parameters
    Compartment::daysFollowUp = input["daysFollowUp"];
    Distribution::errorTolerance = input["errorTolerance"];

    // Generate models
    std::vector<std::shared_ptr<Model>> allModels;
    for (auto& locationConfig: input["locations"]) {
        auto myModel = std::make_shared<Model>(locationConfig["name"], locationConfig["transmissionRate"], locationConfig["infectiousComps"]);

        if (!locationConfig["locationInteraction"].is_null()) {
            myModel->addLocationInteraction(locationConfig["locationInteraction"]);
        }

        // Generate all compartments from input file
        std::vector<std::shared_ptr<Compartment>> allCompartments;
        for (auto& compConfig: locationConfig["compartments"]) {
            CompartmentJSON compJson(compConfig);
            allCompartments.push_back(compJson.compFromJSON());
        }

        // Add linkedCompartment needs to be done as a separated step because we need all compartments created before connecting them
        for (auto& compConfig: locationConfig["compartments"]) {
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
        myModel->addCompsFromConfig(allCompartments);
        myModel->sortComps();
        myModel->calcPopulationSize();

        allModels.push_back(myModel);
    }

    for (auto& locationConfig: input["locations"]) {
        if (!locationConfig["linkedLocations"].is_null()) {
            std::weak_ptr<Model> baseModel;
            for (auto& model: allModels) {
                if (model->getName() == locationConfig["name"]) {
                    baseModel = model;
                }
            }
            for (auto& linkedConfig: locationConfig["linkedLocations"]) {
                std::weak_ptr<Model> linkedModel;
                for (auto& model: allModels) {
                    if (model->getName() == linkedConfig) {
                        linkedModel = model;
                        baseModel.lock()->addLinkedLocation(linkedModel);
                    }
                }
            }
        }
    }

    // ======================== End JSON input ==============================

    // ==================== Construct and run model ==========================

    // Run model
//    std::ofstream iterFile("../output/iteration.txt");
//    iterFile << "Iteration: " << "0" << std::endl;
//    for (size_t j {0}; j < myModel.getComps().size(); ++j) {
//        iterFile << myModel.getComps()[j]->getName() << ": ";
//        for (auto k: myModel.getComps()[j]->getSubCompartmentValues()) {
//            iterFile << k << " ";
//        }
//        iterFile << std::endl;
//    }

    for (auto& myModel: allModels) {
        for (size_t i {1}; i < Compartment::daysFollowUp; i++) {
            // Force of infection: lambda = beta * Y / N, of which Y is the total infectious would change in each iteration
            // Calculate total number of infectious

            // Then we can update the model
            myModel->update(i);
        }
    }

        // For debug
//        iterFile << "Iteration: " << i << std::endl;
//        for (size_t j {0}; j < myModel.getComps().size(); ++j) {
//            iterFile << myModel.getComps()[j]->getName() << ": ";
//            for (auto k: myModel.getComps()[j]->getSubCompartmentValues()) {
//                iterFile << k << " ";
//            }
//            iterFile << std::endl;
//        }
//    }
    // ================== End construct and run model ========================

    // ========================= Write output ================================
    // Create json object to store all input parameters
//    nlohmann::json writeConfig;
//    writeConfig["daysFollowUp"] = Compartment::daysFollowUp;
//    writeConfig["errorTolerance"] = Distribution::errorTolerance;
//    writeConfig["populationSize"] = myModel.getPopulationSize();
//    writeConfig["transmissionRate"] = transmissionRate;
//    writeConfig["infectiousComps"] = infectiousComps;
//    for (auto i: myModel.getComps()) {
//        CompartmentJSON jsonNode;
//        jsonNode.compToJSON(i);
//        writeConfig["compartments"].push_back(jsonNode.getJsonNode());
//    }
//    std::ofstream myFile("/home/thinh/Downloads/config2.json");
//    if (myFile.is_open()) {
//        myFile << writeConfig;
//        myFile.close();
//        std::cout << "Successfully written input information into file: /home/thinh/Downloads/config2.json" <<
//        std::endl;
//    } else {
//        std::cout << "Unable to write file" << std::endl;
//    }

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

    for (auto& model: allModels) {
        Model* pModel = &(*model);
        std::string fileName = "testSIR_" + model->getName() + ".csv";
        FileCSV file("../output/", fileName, pModel);
        file.writeFile();
    }

}

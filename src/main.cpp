#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <stdexcept>
#include "json.h"
#include "Compartment.h"
#include "Model.h"
#include "FullModel.h"
#include "Distribution/Distribution.h"
#include "Distribution/TransitionProb.h"
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
    if (!input["timeStep"].is_null()) {
        Distribution::timeStep = input["timeStep"];
    }
    for (std::string structure: input["modelStructure"]) {
        Model::modelStructure.push_back(structure);
    }
    for (std::string infComp: input["infectiousComps"]) {
        Model::infectiousComps.push_back(infComp);
    }

    // ====== Initialize the full model with relationship between locations ======
    FullModel allModels(input["locationContacts"]);

    // For each location:
    for (auto& locationConfig: input["locations"]) {

        // Generate all compartments in this location
        std::vector<std::shared_ptr<Compartment>> allCompartments;
        for (auto& compConfig: locationConfig["compartments"]) {
            CompartmentJSON compJson(compConfig);
            allCompartments.push_back(compJson.getComp());
        }

        // Make model for this location
        auto myModel = std::make_shared<Model>(locationConfig["name"], locationConfig["transmissionRate"]);
        myModel->addCompsFromConfig(allCompartments);

        // Because all compartments had been created, we can connect the compartments now
        myModel->connectComp("->", ":");

        // Check cycle, sort and calculate population size
        myModel->sortComps();
        myModel->calcPopulationSize();

        // Finally, add this model to the full model
        allModels.addModel(myModel);
    }

    allModels.connectLocations("<->", ":");

    // ======================== End JSON input ==============================

    // ==================== Construct and run model ==========================

    // BE CAUTIOUS: The order of the following two for loop is extremely important, at each iteration we want to
    // update location 1, then location 2, then move on to the next iteration. We never want to update a location
    // from iter 1 to iter 100 then continue to update the next location. So the "iter" for loop comes first, then
    // the "location" for loop
    for (size_t i {1}; i < Compartment::daysFollowUp; i++) {
        for (auto& myModel: allModels.getModels()) {
            myModel->update(i);
        }
    }

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
    std::string outputFolder;
    std::cout << "Set path to the folder you want to save output file (ex: /home/Documents): ";
    std::cin >> outputFolder;
    for (auto& model: allModels.getModels()) {
        Model* pModel = &(*model);
        std::string outputFileName = model->getName() + ".csv";
        FileCSV file(outputFolder, outputFileName, pModel);
        file.writeFile();
    }

}

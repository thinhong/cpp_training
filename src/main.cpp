#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include "json.h"
#include "Contact.h"
#include "Compartment.h"
#include "Model.h"
#include "FullModel.h"
#include "Distribution/Distribution.h"
#include "Distribution/TransitionProb.h"
#include "FileCSV.h"
#include "CompartmentJSON.h"
#include <filesystem>
#include <chrono>
#include "Matrix.h"

int main() {

//    Matrix m1({{1, 2}, {3, 4}});
//    Matrix m2({{0, 5}, {6, 7}});
//    Matrix m3(m1.KroneckerProduct(m2));
//    m3.displayMatrix();
//    Matrix m4(m2.KroneckerProduct(m1));
//    m4.displayMatrix();
//
//    Matrix v1({{1, -4, 7},{-2, 3, 3}});
//    Matrix v2({{8, -9, -6, 5}, {1, -3, -4, 7}, {2, 8, -8, -3}, {1, 2, -5, -1}});
//
//    Matrix v(v1.KroneckerProduct(v2));
//    v.displayMatrix();

        // ========================== JSON input ==============================

    // Read a JSON input file to provide parameters
    std::string inputPath;
    std::cout << "Enter full path to input file (ex: /home/Documents/config.json): ";
    std::cin >> inputPath;
    std::ifstream inputFile(inputPath);
    nlohmann::json input;
    inputFile >> input;

    // Record execution time: https://stackoverflow.com/questions/21856025/getting-an-accurate-execution-time-in-c-micro-seconds
    auto startTime = std::chrono::high_resolution_clock::now();

    std::cout << "Reading input file..." << "\n";

    // Initialize parameters
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

    Compartment::timesFollowUp = static_cast<size_t>(static_cast<double>(input["daysFollowUp"]) / Distribution::timeStep + 1);

    // Initialize contactAssumption first because the contact will be generate following this order
    std::vector<std::shared_ptr<Contact>> allContacts;
    if (!input["contacts"].is_null()) {
        for (auto& contactConfig: input["contacts"]) {
            Contact::contactAssumption.push_back(contactConfig["contactType"]);
            auto contact = std::make_shared<Contact>(contactConfig["contactType"], contactConfig["contactClasses"], contactConfig["contactRates"]);
            allContacts.push_back(contact);
        }
    }

    // ====== Initialize the full model ======
    FullModel allModels(allContacts);

    // For each location:
    for (auto& modelConfig: input["models"]) {

        // Generate all compartments in this location
        std::vector<std::shared_ptr<Compartment>> allCompartments;
        for (auto& compConfig: modelConfig["compartments"]) {
            CompartmentJSON compJson(compConfig);
            allCompartments.push_back(compJson.getComp());
        }

        // Make model for this location
        auto myModel = std::make_shared<Model>(modelConfig["modelName"], modelConfig["transmissionRate"]);
        myModel->addCompsFromConfig(allCompartments);

        // Because all compartments had been created, we can connect the compartments now
        myModel->connectComp();

        // Check cycle, sort and calculate population size
        myModel->sortComps();
        myModel->calcPopulationSize();

        // This is to make sure that ["HCM", "male"] == ["male", "HCM"]
        if (!allContacts.empty()) {
            myModel->sortModelGroupByAssumption(allContacts);
        }

        // Finally, add this model to the full model
        allModels.addModel(myModel);
    }

    // After adding all models, connect them
    allModels.connectModels();

    std::cout << "Simulating..." << "\n";

    // ======================== End JSON input ==============================

    // ==================== Construct and run model ==========================

    // BE CAUTIOUS: The order of the following two for loop is extremely important, at each iteration we want to
    // update location 1, then location 2, then move on to the next iteration. We never want to update a location
    // from iter 1 to iter 100 then continue to update the next location. So the "iter" for loop comes first, then
    // the "location" for loop
    for (size_t i {1}; i < Compartment::timesFollowUp; i++) {
        for (auto& myModel: allModels.getModels()) {
            myModel->update(i);
        }
    }

    // Display execution time
    auto elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
    long long seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsedTime).count();
    std::cout << "Simulation completed, elapsed time: " << seconds << " seconds\n";

    // ================== End construct and run model ========================

    // ========================= Write output ================================
//    // Create json object to store all input parameters
//    nlohmann::json writeConfig;
//    writeConfig["timesFollowUp"] = Compartment::timesFollowUp;
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
//        std::cout << "Unable to write config log file" << std::endl;
//    }

    // Write output to CSV file
    std::string outputFolder;
    std::cout << "Set path to the folder you want to save output file (ex: /home/Documents): ";
    std::cin >> outputFolder;
    for (auto& model: allModels.getModels()) {
        Model* pModel = &(*model);
        std::string outputFileName;
        for (size_t i {0}; i < model->getModelGroup().size(); ++i) {
            if (i < (model->getModelGroup().size() - 1)) {
                outputFileName += model->getModelGroup()[i] + "_";
            } else if (i == (model->getModelGroup().size() - 1)) {
                outputFileName += model->getModelGroup()[i];
            }
        }
        outputFileName += ".csv";
        FileCSV file(outputFolder, outputFileName, pModel);
        file.writeFile();
    }

}

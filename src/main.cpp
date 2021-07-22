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
#include "ModelJSON.h"
#include <filesystem>
#include <chrono>
#include "Matrix.h"
#include "muParser/muParser.h"
#include "helpers.h"

int main() {

    // ========================== JSON input ==============================
    // Read JSON input file to provide parameters
    std::string inputPath;
    std::cout << "Enter full path to input file (ex: /home/Documents/config.json): ";
    std::cin >> inputPath;
    std::ifstream inputFile(inputPath);
    nlohmann::json input;
    inputFile >> input;

    // Record execution time: https://stackoverflow.com/questions/21856025/getting-an-accurate-execution-time-in-c-micro-seconds
    auto startTime = std::chrono::high_resolution_clock::now();

    std::cout << "Reading input file..." << "\n";

    // Initialize parameters: errorTolerance, timeStep and daysFollowUp
    Distribution::errorTolerance = input["errorTolerance"];
    if (!input["timeStep"].is_null()) {
        Distribution::timeStep = input["timeStep"];
    }
    Compartment::timesFollowUp = static_cast<size_t>(static_cast<double>(input["daysFollowUp"]) / Distribution::timeStep + 1);

    ModelJSON myModel(input["initialValues"], input["parameters"], input["transitions"]);
    myModel.getModel()->sortComps();
    myModel.getModel()->initAllComps();

//    // Initialize contactAssumption first because the contact will be generate following this order
//    std::vector<std::shared_ptr<Contact>> allContacts;
//    if (!input["contacts"].is_null()) {
//        for (auto& contactConfig: input["contacts"]) {
//            Contact::contactAssumption.push_back(contactConfig["contactType"]);
//            auto contact = std::make_shared<Contact>(contactConfig["contactType"], contactConfig["contactClasses"], contactConfig["contactRates"]);
//            allContacts.push_back(contact);
//        }
//    }
//
//    // ====== Initialize the full model ======
//    FullModel allModels(allContacts);
//
//    // For each location:
//    for (auto& modelConfig: input["models"]) {
//
//        // Generate all compartments in this location
//        std::vector<std::shared_ptr<Compartment>> allCompartments;
//        for (auto& compConfig: modelConfig["compartments"]) {
//            CompartmentJSON compJson(compConfig);
//            allCompartments.push_back(compJson.getComp());
//        }
//
//        // Make model for this location
//        auto model = std::make_shared<Model>(modelConfig["modelName"], paramNames, paramValues);
//        model->addCompsFromConfig(allCompartments);
//
//        // Because all compartments had been created, we can connect the compartments now
//        model->connectComp();
//
//        // Check cycle, sort and calculate population size
//        model->sortComps();
//        model->initAllComps();
//
//        // This is to make sure that ["HCM", "male"] == ["male", "HCM"]
//        if (!allContacts.empty()) {
//            model->sortModelGroupByAssumption(allContacts);
//        }
//
//        // Finally, add this model to the full model
//        allModels.addModel(model);
//    }
//
//    // After adding all models, connect them
//    allModels.connectModels();

    std::cout << "Simulating..." << "\n";

    // ======================== End JSON input ==============================

    // ==================== Construct and run model ==========================

    // BE CAUTIOUS: The order of the following two for loop is extremely important, at each iteration we want to
    // update location 1, then location 2, then move on to the next iteration. We never want to update a location
    // from iter 1 to iter 100 then continue to update the next location. So the "iter" for loop comes first, then
    // the "location" for loop
    for (size_t i {1}; i < Compartment::timesFollowUp; i++) {
        myModel.getModel()->update(i);
    }
//
//    // Display execution time
    auto elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
    double seconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count();
    seconds /= 1000;
    std::cout << "Simulation completed, elapsed time: ";
    std::cout << std::fixed << std::setprecision(4) << seconds << " seconds\n";
//
//    // ================== End construct and run model ========================
//
//    // ========================= Write output ================================
////    // Create json object to store all input parameters
////    nlohmann::json writeConfig;
////    writeConfig["timesFollowUp"] = Compartment::timesFollowUp;
////    writeConfig["errorTolerance"] = Distribution::errorTolerance;
////    writeConfig["populationSize"] = model.getPopulationSize();
////    writeConfig["transmissionRate"] = transmissionRate;
////    writeConfig["infectiousComps"] = infectiousComps;
////    for (auto i: model.getComps()) {
////        CompartmentJSON jsonNode;
////        jsonNode.compToJSON(i);
////        writeConfig["compartments"].push_back(jsonNode.getJsonNode());
////    }
////    std::ofstream myFile("/home/thinh/Downloads/config2.json");
////    if (myFile.is_open()) {
////        myFile << writeConfig;
////        myFile.close();
////        std::cout << "Successfully written input information into file: /home/thinh/Downloads/config2.json" <<
////        std::endl;
////    } else {
////        std::cout << "Unable to write config log file" << std::endl;
////    }
//
//    // Write output to CSV file
    std::string outputFolder;
    std::cout << "Set path to the folder you want to save output file (ex: /home/Documents): ";
    std::cin >> outputFolder;
    Model* pModel = &(*myModel.getModel());
    std::string outputFileName;
    for (size_t i {0}; i < myModel.getModel()->getModelGroup().size(); ++i) {
        if (i < (myModel.getModel()->getModelGroup().size() - 1)) {
            outputFileName += myModel.getModel()->getModelGroup()[i] + "_";
        } else if (i == (myModel.getModel()->getModelGroup().size() - 1)) {
            outputFileName += myModel.getModel()->getModelGroup()[i];
        }
    }
    outputFileName += ".csv";
    FileCSV file(outputFolder, outputFileName, pModel);
    file.writeFile();

}

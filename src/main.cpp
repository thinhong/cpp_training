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

    std::cout << "Simulating..." << "\n";

    // ======================== End JSON input ==============================

    // ==================== Construct and run model ==========================

    for (size_t i {1}; i < Compartment::timesFollowUp; i++) {
        myModel.getModel()->update(i);
        std::cout << "====================================" << "\n";
        std::cout << "Iteration " << i << "\n";
        for (auto& comp: myModel.getModel()->getComps()) {
            std::cout << "Compartment " << comp->getName() << "\n";
            for (size_t i {0}; i < comp->getLinkedCompartmentOut().size(); ++i) {
                std::cout << "Linked out " << i + 1 << ": " << comp->getLinkedCompartmentOut()[i].lock()->getName() << "\n";
                for (size_t k {0}; k < comp->getSubCompartmentValues()[i].size(); ++k) {
                    std::cout << comp->getSubCompartmentValues()[i][k] << " ";
                }
                std::cout << "\n";
                std::cout << "Out value for " << comp->getLinkedCompartmentOut()[i].lock()->getName() << ": " <<
                comp->getOutValues()[i] << "\n";
            }
        }
    }
//
//    // Display execution time
//    auto elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
//    double seconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count();
//    seconds /= 1000;
//    std::cout << "Simulation completed, elapsed time: ";
//    std::cout << std::fixed << std::setprecision(4) << seconds << " seconds\n";
//
//    // ================== End construct and run model ========================
//
//    // Write output to CSV file
//    std::string outputFolder;
//    std::cout << "Set path to the folder you want to save output file (ex: /home/Documents): ";
//    std::cin >> outputFolder;
//    Model* pModel = &(*myModel.getModel());
//    std::string outputFileName;
//    for (size_t i {0}; i < myModel.getModel()->getModelGroup().size(); ++i) {
//        if (i < (myModel.getModel()->getModelGroup().size() - 1)) {
//            outputFileName += myModel.getModel()->getModelGroup()[i] + "_";
//        } else if (i == (myModel.getModel()->getModelGroup().size() - 1)) {
//            outputFileName += myModel.getModel()->getModelGroup()[i];
//        }
//    }
//    outputFileName += ".csv";
//    FileCSV file(outputFolder, outputFileName, pModel);
//    file.writeFile();

}

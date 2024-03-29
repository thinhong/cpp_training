#include <iostream>
#include <string>
#include <fstream>
#include "json.h"
#include "Compartment.h"
#include "Distribution.h"
#include "FileCSV.h"
#include "ModelJSON.h"
#include <filesystem>
#include <chrono>
#include "helpers.h"

int main() {

    // ========================== JSON input ==============================
    // Read JSON input file to provide parameters
    std::string inputPath;
    std::cout << "Enter path to input file: ";
    std::cin >> inputPath;
    std::ifstream inputFile(inputPath);
    nlohmann::ordered_json input;
    inputFile >> input;

    // Record execution time: https://stackoverflow.com/questions/21856025/getting-an-accurate-execution-time-in-c-micro-seconds
    auto startTime = std::chrono::high_resolution_clock::now();
    std::cout << "Reading input file..." << "\n";

    // ======== Check whether all compartments have initial values ========
    try {
        if (!checkInitVal(input["initialValues"], input["transitions"]).empty()) {
            throw 99;
        }
    }
    catch (int exCode) {
        std::vector<std::string> diffs = checkInitVal(input["initialValues"], input["transitions"]);
        std::cout << "Compartment ";
        for (auto& diff: diffs) {
            std::cout << diff << " ";
        }
        if (diffs.size() == 1) {
            std::cout << "is not initialised" << std::endl;
        } else if (diffs.size() > 1) {
            std::cout << "are not initialised" << std::endl;
        }
        std::exit(EXIT_FAILURE);
    }

    // Then check errorTolerance > 0
    try {
        if (input["errorTolerance"] == 0) {
            throw 99;
        }
    }
    catch (int exCode) {
        std::cout << "Error: errorTolerance must > 0" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    // =====================================================================

    // Initialize parameters: errorTolerance, timeStep and daysFollowUp
    Distribution::errorTolerance = input["errorTolerance"];
    if (!input["timeStep"].is_null()) {
        Distribution::timeStep = input["timeStep"];
    }

    // Compute real time follow up by dividing simulation duration to time step
    Compartment::timesFollowUp = static_cast<size_t>(static_cast<double>(input["simulationDuration"]) / Distribution::timeStep + 1);
    // Make model object
    ModelJSON myModel(input["initialValues"], input["parameters"], input["transitions"]);

    // We sort the comps with order defined by input transition
    myModel.getModel()->sortCompsByInputTransition();
    myModel.getModel()->initAllComps();

    // Debug: view model structure
//    viewModelStructure(myModel.getModel());

    std::cout << "Simulating..." << "\n";

    // ======================== End JSON input ==============================

    // ==================== Construct and run model ==========================

    for (size_t i {1}; i < Compartment::timesFollowUp; i++) {
        myModel.getModel()->update(i);
        // Debug: view each time step update
//        viewModelUpdate(myModel.getModel(), i);
    }

    // Display execution time
    auto elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
    double seconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count();
    seconds /= 1000;
    std::cout << "Simulation completed, elapsed time: ";
    std::cout << std::fixed << std::setprecision(4) << seconds << " seconds\n";

    // ================== End construct and run model ========================

    // Write output to CSV file
    std::string outputFolder;
    std::cout << "Set path to the folder you want to save output file (ex: /home/Documents): ";
    std::cin >> outputFolder;
    Model* pModel = &(*myModel.getModel());
    std::string outputFileName = "output.csv";
    FileCSV file(outputFolder, outputFileName, pModel);
    file.writeFile();

}

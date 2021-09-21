#include "helpers.h"

std::vector<std::string> extractCompNames(std::string element) {
    std::vector<std::string> compNames;
    // Remove all whitespace
    element.erase(remove(element.begin(), element.end(), ' '), element.end());
    // Find the location of "->"
    int transitionSymbol_pos = element.find("->");
    // Check whether there is a ":" symbol in this flow
    int probSymbol_pos = element.find(':');
    // inComp start from position 0 and spread from 0 -> transitionSymbol_pos => length = transitionSymbol_pos - 0 = transitionSymbol_pos
    std::string inCompName = element.substr(0, transitionSymbol_pos);
    // outComp start from transitionSymbol_pos + 2 (transitionSymbol_pos is "->" therefore occupies 2 positions), and
    // spread from transitionSymbol_pos + 2 to probSymbol_pos => length = probSymbol_pos - (transitionSymbol_pos + 2)
    std::string outCompName = element.substr(transitionSymbol_pos + 2, probSymbol_pos - (transitionSymbol_pos + 2));
    compNames.insert(compNames.end(), {inCompName, outCompName});
    return compNames;
}

void viewModelStructure(std::shared_ptr<Model> model) {
    for (auto& comp: model->getComps()) {
        std::cout << "Compartment " << comp->getCompName() << "\n";

        std::cout << "In compartments: ";
        for (auto& inComp: comp->getInCompartments()) {
            std::cout << inComp.lock()->getCompName() << " ";
        }
        std::cout << "\n";

        std::cout << "Out compartments: ";
        for (auto& outComp: comp->getOutCompartments()) {
            std::cout << outComp.lock()->getCompName() << " ";
        }
        std::cout << "\n";

        std::cout << "Out distribution: ";
        for (auto& outDist: comp->getOutDistributions()) {
            std::cout << outDist << " (" << outDist->getDistName() << "), ";
        }
        std::cout << "\n";

        std::cout << "Out weights: ";
        for (auto& outWeight: comp->getOutWeights()) {
            std::cout << outWeight << " ";
        }
        std::cout << "\n";

        std::cout << "============" << "\n";
    }
}

void viewModelUpdate(std::shared_ptr<Model> model, long iter) {
    std::cout << "====================================" << "\n";
    std::cout << "Iteration " << iter << "\n";
    for (auto& comp: model->getComps()) {
        std::cout << "Compartment " << comp->getCompName() << "\n";
        for (size_t k {0}; k < comp->getSubCompartmentValues().size(); ++k) {
            std::cout << comp->getSubCompartmentValues()[k] << " ";
        }
        std::cout << "\n";
        for (size_t j {0}; j < comp->getOutCompartments().size(); ++j) {
            std::cout << "Out value for " << comp->getOutCompartments()[j].lock()->getCompName() << ": " <<
                      comp->getOutValues()[j] << "\n";
        }
        std::cout << "Total " << comp->getCompName() << ": ";
        for (auto& total: comp->getCompTotal()) {
            std::cout << total << " ";
        }
        std::cout << "\n";
    }
}

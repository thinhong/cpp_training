#include <iostream>
#include <fstream>
#include "Compartment.h"
#include "Model.h"

const double transRate = 2.0 / 100000;
const double recovRate = 1.0 / 10;

int main() {
    // Compartment S, I, R
    Compartment S("S", 1000, 20000);
    Compartment I("I", 1000, 1);
    Compartment R("R", 1000, 0);

    // Make shared pointer for Compartment S, I and R
    auto pS = std::make_shared<Compartment>(S);
    auto pI = std::make_shared<Compartment>(I);
    auto pR = std::make_shared<Compartment>(R);
    // Make a null pointer point to type Compartment
    std::shared_ptr<Compartment> pNull = nullptr;

    // Also shared pointer for parameters
    std::shared_ptr<double> pTransRate = std::make_shared<double>(transRate);
    std::shared_ptr<double> pRecovRate = std::make_shared<double>(recovRate);

    // Model SIR consists of S, I and R
    Model SIR;
    SIR.setComps(pS);
    SIR.setComps(pI);
    SIR.setComps(pR);

    // Connect S -> I, I -> R
    SIR.connect(pS, pI, pTransRate, pI);
    SIR.connect(pI, pR, pRecovRate, pNull);

    // Update this model
    SIR.update();

    for (size_t i {}; i < 30; i++) {
        std::cout << SIR.getComps()[0]->getValue()[i] << ",";
        std::cout << SIR.getComps()[1]->getValue()[i] << ",";
        std::cout << SIR.getComps()[2]->getValue()[i] << "\n";
    }

    std::ofstream myfile ("/home/thinh/Downloads/SIR_basic.csv");
    if (myfile.is_open()) {
        myfile << SIR.getComps()[0]->getName() << ","
                << SIR.getComps()[1]->getName() << ","
                << SIR.getComps()[2]->getName() << "\n";
        for (size_t i {}; i < 1000; i++) {
            myfile << SIR.getComps()[0]->getValue()[i] << ",";
            myfile << SIR.getComps()[1]->getValue()[i] << ",";
            myfile << SIR.getComps()[2]->getValue()[i] << "\n";
        }
        myfile.close();
        std::cout << "Successfully written into file";
    }
    else std::cout << "Unable to open file";
    return 0;
}

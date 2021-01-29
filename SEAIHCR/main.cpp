#include <iostream>
#include <fstream>
#include "Compartment.h"
#include "Model.h"

const double lambda = 2.0 / 100000;
const double theta = 1.0 / 3;
const double tau = 1.0 / 2;
const double p_h = 0.2;
const double p_c = 0.1;
const double p_d = 0.02;
const double tau_sumP = tau * (p_h + p_c + p_d);
const double tau_sumP_comp = tau * (1 - (p_h + p_c + p_d));
const double gamma_a = 1.0 / 5;
const double gamma_h = 1.0 / 7;
const double gamma_c = 1.0 / 12;
const double alpha_c = 1.0 / 5;
const double alpha_d = 1.0 / 5;
const double beta_d = 1.0 / 5;

int main() {
    // Create all compartments
    Compartment S("S", 1000, 100000);
    Compartment E("E", 1000, 0);
    Compartment A("A", 1000, 1);
    Compartment A_r("A_r", 1000, 1);
    Compartment I("I", 1000, 1);
    Compartment H_h("H_h", 1000, 0);
    Compartment H_c("H_c", 1000, 0);
    Compartment H_d("H_d", 1000, 0);
    Compartment C_c("C_c", 1000, 0);
    Compartment C_d("C_d", 1000, 0);
    Compartment D("D", 1000, 0);
    Compartment R("R", 1000, 0);

    // Make shared pointer for Compartment S, I and R
    auto pS = std::make_shared<Compartment>(S);
    auto pE = std::make_shared<Compartment>(E);
    auto pA = std::make_shared<Compartment>(A);
    auto pA_r = std::make_shared<Compartment>(A_r);
    auto pI = std::make_shared<Compartment>(I);
    auto pH_h = std::make_shared<Compartment>(H_h);
    auto pH_c = std::make_shared<Compartment>(H_c);
    auto pH_d = std::make_shared<Compartment>(H_d);
    auto pC_c = std::make_shared<Compartment>(C_c);
    auto pC_d = std::make_shared<Compartment>(C_d);
    auto pD = std::make_shared<Compartment>(D);
    auto pR = std::make_shared<Compartment>(R);

    // Also shared pointer for parameters
    std::shared_ptr<double> pLambda = std::make_shared<double>(lambda);
    std::shared_ptr<double> pTheta = std::make_shared<double>(theta);
    std::shared_ptr<double> pTau = std::make_shared<double>(tau);
    std::shared_ptr<double> pP_h = std::make_shared<double>(p_h);
    std::shared_ptr<double> pP_c = std::make_shared<double>(p_c);
    std::shared_ptr<double> pP_d = std::make_shared<double>(p_d);
    std::shared_ptr<double> pTau_sumP = std::make_shared<double>(tau_sumP);
    std::shared_ptr<double> pTau_sumP_comp = std::make_shared<double>(tau_sumP_comp);
    std::shared_ptr<double> pGamma_a = std::make_shared<double>(gamma_a);
    std::shared_ptr<double> pGamma_h = std::make_shared<double>(gamma_h);
    std::shared_ptr<double> pGamma_c = std::make_shared<double>(gamma_c);
    std::shared_ptr<double> pAlpha_c = std::make_shared<double>(alpha_c);
    std::shared_ptr<double> pAlpha_d = std::make_shared<double>(alpha_d);
    std::shared_ptr<double> pBeta_d = std::make_shared<double>(beta_d);

    // Model myModel consists of S, I and R
    Model myModel;
    myModel.setComps(pS);
    myModel.setComps(pE);
    myModel.setComps(pA);
    myModel.setComps(pA_r);
    myModel.setComps(pI);
    myModel.setComps(pH_h);
    myModel.setComps(pH_c);
    myModel.setComps(pH_d);
    myModel.setComps(pC_c);
    myModel.setComps(pC_d);
    myModel.setComps(pD);
    myModel.setComps(pR);

    // Connect S -> E -> A
    myModel.connect(pS, pE, pLambda);
    myModel.connect(pE, pA, pTheta);
    // A -> A_r and I
    myModel.connect(pA, pA_r, pTau_sumP_comp);
    myModel.connect(pA, pI, pTau_sumP);
    // A_r -> R
    myModel.connect(pA_r, pR, pGamma_a);
    // I -> H_h, H_c and H_d
    myModel.connect(pI, pH_h, pP_h);
    myModel.connect(pI, pH_c, pP_c);
    myModel.connect(pI, pH_d, pP_d);
    // H_h -> R
    myModel.connect(pH_h, pR, pGamma_h);
    // H_c -> C_c -> R
    myModel.connect(pH_c, pC_c, pAlpha_c);
    myModel.connect(pC_c, pR, pGamma_c);
    // H_d -> C_d -> D
    myModel.connect(pH_d, pC_d, pAlpha_d);
    myModel.connect(pC_d, pD, pBeta_d);

    // Set extra parameters to S
    myModel.getComps()[0]->setExtraParam(pA);
    myModel.getComps()[0]->setExtraParam(pA_r);
    myModel.getComps()[0]->setExtraParam(pI);

    for (auto i: myModel.getComps()[0]->getExtraParam()) {
        std::cout << i << ' ';
    }

    // Update this model
    myModel.update();

//    for (size_t i {}; i < 30; i++) {
//        for (size_t j {}; j < myModel.getComps().size(); ++j) {
//            if (j == myModel.getComps().size() - 1) {
//                std::cout << myModel.getComps()[j]->getValue()[i] << "\n";
//            } else {
//                std::cout << myModel.getComps()[j]->getValue()[i] << ",";
//            }
//        }
//    }

    std::ofstream myFile ("/home/thinh/Downloads/SIR_test.csv");
    if (myFile.is_open()) {
        for (size_t j {}; j < myModel.getComps().size(); ++j) {
            if (j == myModel.getComps().size() - 1) {
                myFile << myModel.getComps()[j]->getName() << "\n";
            } else {
                myFile << myModel.getComps()[j]->getName() << ",";
            }
        }
        for (size_t i {}; i < 1000; ++i) {
            for (size_t j {}; j < myModel.getComps().size(); ++j) {
                if (j == myModel.getComps().size() - 1) {
                    myFile << myModel.getComps()[j]->getValue()[i] << "\n";
                } else {
                    myFile << myModel.getComps()[j]->getValue()[i] << ",";
                }
            }
        }
        myFile.close();
        std::cout << "Successfully written into file";
    }
    else std::cout << "Unable to open file";
    return 0;
}

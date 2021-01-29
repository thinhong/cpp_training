#include <iostream>
#include <fstream>
#include "Compartment.h"
#include "Model.h"

int main() {
    // Make shared pointer for Compartment S, I and R
    auto S = std::make_shared<Compartment>("S", 1000, 100000);
    auto E = std::make_shared<Compartment>("E", 1000, 0);
    auto A = std::make_shared<Compartment>("A", 1000, 0);
    auto A_r = std::make_shared<Compartment>("A_r", 1000, 0);
    auto I = std::make_shared<Compartment>("I", 1000, 1);
    auto H_h = std::make_shared<Compartment>("H_h", 1000, 0);
    auto H_c = std::make_shared<Compartment>("H_c", 1000, 0);
    auto H_d = std::make_shared<Compartment>("H_d", 1000, 0);
    auto C_c = std::make_shared<Compartment>("C_c", 1000, 0);
    auto C_d = std::make_shared<Compartment>("C_d", 1000, 0);
    auto D = std::make_shared<Compartment>("D", 1000, 0);
    auto R = std::make_shared<Compartment>("R", 1000, 0);

    // Also shared pointer for parameters
    auto lambda = std::make_shared<double>(2.0 / 500000);
    auto theta = std::make_shared<double>(1.0 / 3);
    auto tau = std::make_shared<double>(1.0 / 2);
    auto p_h = std::make_shared<double>(0.2);
    auto p_c = std::make_shared<double>(0.1);
    auto p_d = std::make_shared<double>(0.02);
    auto tau_sum = std::make_shared<double>((*p_h) + (*p_c) + (*p_d));
    auto tau_sum_comp = std::make_shared<double>(1 - (*tau_sum));
    auto gamma_a = std::make_shared<double>(1.0 / 5);
    auto gamma_h = std::make_shared<double>(1.0 / 7);
    auto gamma_c = std::make_shared<double>(1.0 / 12);
    auto alpha_c = std::make_shared<double>(1.0 / 5);
    auto alpha_d = std::make_shared<double>(1.0 / 5);
    auto beta_d = std::make_shared<double>(1.0 / 5);

    // Model myModel consists of S, I and R
    Model myModel;
    myModel.setComps(S);
    myModel.setComps(E);
    myModel.setComps(A);
    myModel.setComps(A_r);
    myModel.setComps(I);
    myModel.setComps(H_h);
    myModel.setComps(H_c);
    myModel.setComps(H_d);
    myModel.setComps(C_c);
    myModel.setComps(C_d);
    myModel.setComps(D);
    myModel.setComps(R);

    // Connect S -> E -> A
    myModel.connect(S, E, lambda, true);
    myModel.connect(E, A, theta, false);
    // A -> A_r and I
    myModel.connect(A, A_r, tau_sum_comp, false);
    myModel.connect(A, I, tau_sum, false);
    // A_r -> R
    myModel.connect(A_r, R, gamma_a, false);
    // I -> H_h, H_c and H_d
    myModel.connect(I, H_h, p_h, false);
    myModel.connect(I, H_c, p_c, false);
    myModel.connect(I, H_d, p_d, false);
    // H_h -> R
    myModel.connect(H_h, R, gamma_h, false);
    // H_c -> C_c -> R
    myModel.connect(H_c, C_c, alpha_c, false);
    myModel.connect(C_c, R, gamma_c, false);
    // H_d -> C_d -> D
    myModel.connect(H_d, C_d, alpha_d, false);
    myModel.connect(C_d, D, beta_d, false);


    std::vector<std::shared_ptr<Compartment>> otherCompartments {A, A_r, I};

    // Update this model
    myModel.update(otherCompartments);

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
        std::cout << "Successfully written into file" << std::endl;
    }
    else std::cout << "Unable to open file" << std::endl;
    return 0;
}

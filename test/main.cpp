#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

class Compartment {
public:
    std::string name;
    std::vector<double> total;
    std::vector<double> currentValues;
    double outValueNextIter {0};
    std::vector<double> dist;
    std::vector<std::weak_ptr<Compartment>> linkedCompartment;
    std::vector<double> linkedWeight {1};
    std::vector<bool> isIn;
    void updateValue(long iter);
};

void Compartment::updateValue(long iter) {
    // For compartments other than S (currentValues vector has many values)
    if (currentValues.size() > 1) {
        // Going backward from currentValues[n] -> currentValues[1]
        for (size_t i {currentValues.size() - 1}; i > 0; --i) {
            outValueNextIter += currentValues[i] * dist[i];
            currentValues[i] = currentValues[i - 1] * (1 - dist[i - 1]);
        }
        // All of currentValues[0] will go to currentValues[1] so initialize currentValues[0] = 0
        currentValues[0] = 0;
        // Loop over all linkedCompartment, find the linkedCompartment with isIn == true
        // Let currentValues[0] = outValueNextIter of that linkedCompartment
        // Multiply with linkedWeight for situations such as A -> Ar and I, I -> H_h, H_c and H_d
        for (size_t j {0}; j < linkedCompartment.size(); ++j) {
            if (isIn[j]) {
                currentValues[0] = linkedCompartment[j].lock()->outValueNextIter * linkedWeight[j];
            }
        }
    } else if (currentValues.size() == 1) { // For S compartment (S only has 1 value)
        outValueNextIter = currentValues[0] * dist[0];
        currentValues[0] = currentValues[0] * (1 - dist[0]);
    }

    // Finally sum up the current values of this iteration to total
    for (auto value: currentValues) {
        total[iter] += value;
    }
}

int main() {
    auto S = std::make_shared<Compartment>();
    auto E = std::make_shared<Compartment>();
    auto A = std::make_shared<Compartment>();
    auto A_r = std::make_shared<Compartment>();
    auto I = std::make_shared<Compartment>();
    auto H_h = std::make_shared<Compartment>();
    auto H_c = std::make_shared<Compartment>();
    auto H_d = std::make_shared<Compartment>();
    auto C_c = std::make_shared<Compartment>();
    auto C_d = std::make_shared<Compartment>();
    auto R = std::make_shared<Compartment>();
    auto D = std::make_shared<Compartment>();

    S->name = "S";
    S->currentValues = {10000};
    S->total.resize(100);
    S->dist = {0.2};
    S->linkedCompartment = {E};
    S->isIn = {false};

    E->name = "E";
    E->currentValues = {500, 600, 700};
    E->total.resize(100);
    E->dist = {0, 0.8, 1};
    E->linkedCompartment = {S, A};
    E->isIn = {true, false};

    A->name = "A";
    A->currentValues = {5, 6, 7, 8};
    A->total.resize(100);
    A->dist = {0, 0.3, 0.7, 1};
    A->linkedCompartment = {E, A_r, I};
    A->isIn = {true, false, false};

    A_r->name = "A_r";
    A_r->currentValues = {0, 0, 0, 0};
    A_r->total.resize(100);
    A_r->dist = {0, 0.2, 0.4, 1};
    A_r->linkedCompartment = {A, R};
    A_r->isIn = {true, false};
    A_r->linkedWeight = {0.3};

    I->name = "I";
    I->currentValues = {1, 0, 0, 0};
    I->total.resize(100);
    I->dist = {0, 0.3, 0.5, 1};
    I->linkedCompartment = {A, H_h, H_c, H_d};
    I->isIn = {true, false, false, false};
    I->linkedWeight = {0.7};

    R->name = "R";
    R->currentValues = {0, 0, 0, 0};
    R->total.resize(100);
    R->dist = {0, 0.3, 0.5, 1};
    R->linkedCompartment = {A_r, I};
    R->isIn = {true, true};
    R->linkedWeight = {0.7};

    H_h->name = "H_h";
    H_h->linkedCompartment = {I, R};
    H_h->isIn = {true, false};

    H_c->name = "H_c";
    H_c->linkedCompartment = {I, C_c};
    H_c->isIn = {true, false};

    H_d->name = "H_d";
    H_d->linkedCompartment = {I, C_d};
    H_d->isIn = {true, false};

    C_c->name = "C_c";
    C_c->linkedCompartment = {H_c, R};
    C_c->isIn = {true, false};

    C_d->name = "C_d";
    C_d->linkedCompartment = {H_d, D};
    C_d->isIn = {true, false};

    D->name = "D";
    D->linkedCompartment = {C_d};
    D->isIn = {true};

    std::vector<std::weak_ptr<Compartment>> model = {C_c, E, R, H_h, C_d, D, H_d, H_c, A_r, S, I, A};

    // Find S and R
    for (size_t i {0}; i < model.size(); ++i) {
        int sumIsIn {0};
        for (auto j: model[i].lock()->isIn) {
            sumIsIn += j;
        }

        // Find S and shift S to the first position
        if (sumIsIn == 0) {
            auto tmp = model[0];
            model[0] = model[i];
            model[i] = tmp;
        }

        // Find R and shift R to the last position
        if (sumIsIn > 1) {
            auto tmp = model[model.size() - 1];
            model[model.size() - 1] = model[i];
            model[i] = tmp;
        }
    }

    // Sort the compartments between S and R
    // Start from the first compartment (S)
    for (size_t i {0}; i < model.size() - 1; ++i) {
        // Get its linked compartment
        for (auto& linked: model[i].lock()->linkedCompartment) {
            // Search from the second compartment, if there is a compartment with the same name
            // as linked compartment, switch it next to the current (i.e i + 1)
            for (size_t j {i + 1}; j < model.size() - 1; ++j) {
                if (model[j].lock()->name == linked.lock()->name) {
                    auto tmp = model[i + 1];
                    model[i + 1] = model[j];
                    model[j] = tmp;
                }
            }
        }
    }


    for (auto i: model) {
        std::cout << i.lock()->name << ' ';
    }

//    for (auto i: model2) {
//        std::cout << i.lock()->name << ' ';
//    }

//    for (auto i: S->currentValues) {
//        std::cout << i << ' ';
//    }
//    std::cout << std::endl;
//    for (auto i: E->currentValues) {
//        std::cout << i << ' ';
//    }
//    std::cout << std::endl;
//    for (auto i: A->currentValues) {
//        std::cout << i << ' ';
//    }
//    std::cout << std::endl;
//    for (auto i: A_r->currentValues) {
//        std::cout << i << ' ';
//    }
//    std::cout << std::endl;
//    for (auto i: I->currentValues) {
//        std::cout << i << ' ';
//    }
//    std::cout << std::endl;

//    std::vector<std::shared_ptr<Compartment>> model = {S, E, A, A_r, I};

//    for (size_t iter {0}; iter < 4; ++iter) {
//        for (auto& comp: model) {
//            comp->updateValue(iter);
//        }
//
//        std::cout << "Iteration " << iter + 1 << std::endl;
//        for (auto i: S->currentValues) {
//            std::cout << "S: " << i << ' ';
//        }
//        std::cout << std::endl << "E: ";
//        for (auto i: E->currentValues) {
//            std::cout << i << ' ';
//        }
//        std::cout << std::endl << "A: ";
//        for (auto i: A->currentValues) {
//            std::cout << i << ' ';
//        }
//        std::cout << std::endl << "A_r: ";
//        for (auto i: A_r->currentValues) {
//            std::cout << i << ' ';
//        }
//        std::cout << std::endl << "I: ";
//        for (auto i: I->currentValues) {
//            std::cout << i << ' ';
//        }
//        std::cout << std::endl;
//    }

}
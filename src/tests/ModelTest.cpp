#include <gtest/gtest.h>
#include <memory>
#include "../Distribution.h"
#include "../Distribution.cpp"
#include "../prob.hpp"
#include "../prob.cpp"
#include "../BernoulliDistribution.h"
#include "../BernoulliDistribution.cpp"
#include "../DiscreteGammaDistribution.h"
#include "../DiscreteGammaDistribution.cpp"
#include "../DiscreteWeibullDistribution.h"
#include "../DiscreteWeibullDistribution.cpp"
#include "../Compartment.h"
#include "../Compartment.cpp"
#include "../Model.h"
#include "../Model.cpp"

TEST(ModelTest, checkCycle) {
    Compartment::daysFollowUp = 200;
    Distribution::errorTolerance = 0.01;
    auto bernoulli = std::make_shared<BernoulliDistribution>(std::make_shared<double>(0.1));
    auto bernoulli_removed = std::make_shared<BernoulliDistribution>(std::make_shared<double>(0.0));
    auto gamma_7d = std::make_shared<DiscreteGammaDistribution>(1, 1);
    auto gamma_13d = std::make_shared<DiscreteGammaDistribution>(1, 4);
    auto weil_16d = std::make_shared<DiscreteWeibullDistribution>(5, 1.5);
    auto weil_9d = std::make_shared<DiscreteWeibullDistribution>(4, 3);

    auto S = std::make_shared<Compartment>("S", 3000000, bernoulli);
    auto E = std::make_shared<Compartment>("E", 0, weil_9d);
    auto A = std::make_shared<Compartment>("A", 0, gamma_7d);
    auto A_r = std::make_shared<Compartment>("A_r", 0, weil_16d);
    auto I = std::make_shared<Compartment>("I", 1, gamma_13d);
    auto H_h = std::make_shared<Compartment>("H_h", 0, gamma_7d);
    auto H_c = std::make_shared<Compartment>("H_c", 0, weil_9d);
    auto H_d = std::make_shared<Compartment>("H_d", 0, gamma_7d);
    auto C_c = std::make_shared<Compartment>("C_c", 0, weil_16d);
    auto C_d = std::make_shared<Compartment>("C_d", 0, gamma_13d);
    auto D = std::make_shared<Compartment>("D", 0, bernoulli_removed);
    auto R = std::make_shared<Compartment>("R", 0, bernoulli_removed);

    Model myModel;

    // Add and also connect all compartments into myModel
    myModel.addCompsAndConnect(S, E);
    myModel.addCompsAndConnect(E, A);
    myModel.addCompsAndConnect(A, A_r, 0.35);
    myModel.addCompsAndConnect(A, I, 0.65);
    myModel.addCompsAndConnect(A_r, R);
    myModel.addCompsAndConnect(I, H_h, 0.7);
    myModel.addCompsAndConnect(I, H_c, 0.2);
    myModel.addCompsAndConnect(I, H_d, 0.1);
    myModel.addCompsAndConnect(H_h, R);
    myModel.addCompsAndConnect(H_c, C_c);
    myModel.addCompsAndConnect(C_c, R);
    myModel.addCompsAndConnect(H_d, C_d);
    myModel.addCompsAndConnect(C_d, D);
    // Test with a isCycle: C_c going back to S
    myModel.addCompsAndConnect(C_c, S);

    myModel.checkCycle();
    EXPECT_TRUE(myModel.getCycle());
}

TEST(ModelTest, sortComp) {
    Compartment::daysFollowUp = 200;
    Distribution::errorTolerance = 0.01;
    auto bernoulli = std::make_shared<BernoulliDistribution>(std::make_shared<double>(0.1));
    auto bernoulli_removed = std::make_shared<BernoulliDistribution>(std::make_shared<double>(0.0));
    auto gamma_7d = std::make_shared<DiscreteGammaDistribution>(1, 1);
    auto gamma_13d = std::make_shared<DiscreteGammaDistribution>(1, 4);
    auto weil_16d = std::make_shared<DiscreteWeibullDistribution>(5, 1.5);
    auto weil_9d = std::make_shared<DiscreteWeibullDistribution>(4, 3);

    auto S = std::make_shared<Compartment>("S", 3000000, bernoulli);
    auto E = std::make_shared<Compartment>("E", 0, weil_9d);
    auto A = std::make_shared<Compartment>("A", 0, gamma_7d);
    auto A_r = std::make_shared<Compartment>("A_r", 0, weil_16d);
    auto I = std::make_shared<Compartment>("I", 1, gamma_13d);
    auto H_h = std::make_shared<Compartment>("H_h", 0, gamma_7d);
    auto H_c = std::make_shared<Compartment>("H_c", 0, weil_9d);
    auto H_d = std::make_shared<Compartment>("H_d", 0, gamma_7d);
    auto C_c = std::make_shared<Compartment>("C_c", 0, weil_16d);
    auto C_d = std::make_shared<Compartment>("C_d", 0, gamma_13d);
    auto D = std::make_shared<Compartment>("D", 0, bernoulli_removed);
    auto R = std::make_shared<Compartment>("R", 0, bernoulli_removed);

    Model myModel;

    // Add and also connect all compartments into myModel
    myModel.addCompsAndConnect(C_c, R);
    myModel.addCompsAndConnect(H_c, C_c);
    myModel.addCompsAndConnect(A_r, R);
    myModel.addCompsAndConnect(S, E);
    myModel.addCompsAndConnect(A, I, 0.65);
    myModel.addCompsAndConnect(E, A);
    myModel.addCompsAndConnect(C_d, D);
    myModel.addCompsAndConnect(A, A_r, 0.35);
    myModel.addCompsAndConnect(I, H_c, 0.2);
    myModel.addCompsAndConnect(H_h, R);
    myModel.addCompsAndConnect(I, H_h, 0.7);
    myModel.addCompsAndConnect(I, H_d, 0.1);
    myModel.addCompsAndConnect(H_d, C_d);

    // Test with a isCycle: A_r going back to E
    std::cout << "Before sorting: ";
    for (auto& comp: myModel.getComps()) {
        std::cout << comp->getName() << ' ';
    }
    std::cout << "\n";
    // myModel.addCompsAndConnect(A_r, E);
    myModel.sortComps();
    std::cout << "After sorting: ";
    for (auto& comp: myModel.getComps()) {
        std::cout << comp->getName() << ' ';
    }
    std::cout << "\n";

    // Test for each flow
    // S -> E -> A -> A_r -> R
    std::vector<std::shared_ptr<Compartment>> flow1 {S, E, A, A_r, R};
    std::vector<size_t> indexFlow1;
    indexFlow1.reserve(flow1.size());
    for (auto& i: flow1) {
        indexFlow1.push_back(myModel.getIndex(i));
    }
    // S -> E -> A -> I -> H_h -> R
    std::vector<std::shared_ptr<Compartment>> flow2 {S, E, A, I, H_h, R};
    std::vector<size_t> indexFlow2;
    indexFlow2.reserve(flow2.size());
    for (auto& i: flow2) {
        indexFlow2.push_back(myModel.getIndex(i));
    }
    // S -> E -> A -> I -> H_c -> C_c -> R
    std::vector<std::shared_ptr<Compartment>> flow3 {S, E, A, I, H_c, C_c, R};
    std::vector<size_t> indexFlow3;
    indexFlow3.reserve(flow3.size());
    for (auto& i: flow3) {
        indexFlow3.push_back(myModel.getIndex(i));
    }
    // // S -> E -> A -> I -> H_d -> C_d -> D
    std::vector<std::shared_ptr<Compartment>> flow4 {S, E, A, I, H_d, C_d, D};
    std::vector<size_t> indexFlow4;
    indexFlow4.reserve(flow4.size());
    for (auto& i: flow4) {
        indexFlow4.push_back(myModel.getIndex(i));
    }

    EXPECT_TRUE(std::is_sorted(indexFlow1.begin(), indexFlow1.end()));
    EXPECT_TRUE(std::is_sorted(indexFlow2.begin(), indexFlow2.end()));
    EXPECT_TRUE(std::is_sorted(indexFlow3.begin(), indexFlow3.end()));
    EXPECT_TRUE(std::is_sorted(indexFlow4.begin(), indexFlow4.end()));
}

class SimpleCompartment {
public:
    std::string name;
    std::vector<double> dist;
    std::vector<std::weak_ptr<SimpleCompartment>> linkedCompartment;
    std::vector<double> linkedWeight;
    std::vector<bool> isIn;
    // Variables for computational analyses
    std::vector<double> total;
    std::vector<double> subCompartmentValues;
    double outValue {0};

    void updateValue(long iter);
};

void SimpleCompartment::updateValue(long iter) {
    int sumIsIn {0};
    for (bool value: isIn) {
        sumIsIn += value;
    }
    // For all compartments except S and R
    if (subCompartmentValues.size() > 1) {
        outValue = 0;
        // Going backward from subCompartmentValues[n] -> subCompartmentValues[1]
        for (size_t i {subCompartmentValues.size() - 1}; i > 0; --i) {
            outValue += subCompartmentValues[i] * dist[i];
            subCompartmentValues[i] = subCompartmentValues[i - 1] * (1 - dist[i - 1]);
        }
        // All of subCompartmentValues[0] will go to subCompartmentValues[1] so initialize subCompartmentValues[0] = 0
        subCompartmentValues[0] = 0;
        // Loop over all linkedCompartment, find the linkedCompartment with isIn == true
        // Let subCompartmentValues[0] = outValue of that linkedCompartment
        // Multiply with linkedWeight for situations such as A -> Ar and I, I -> H_h, H_c and H_d
        for (size_t j {0}; j < linkedCompartment.size(); ++j) {
            if (isIn[j]) {
                subCompartmentValues[0] += linkedCompartment[j].lock()->outValue * linkedWeight[j];
            }
        }
    } else if (subCompartmentValues.size() == 1 && sumIsIn == 0) { // For S compartment (S only has 1 value)
        outValue = subCompartmentValues[0] * dist[0];
        subCompartmentValues[0] -= outValue;
    } else if (subCompartmentValues.size() == 1 && sumIsIn > 0) { // For R compartment, only add people from its coming compartments
        for (size_t j {0}; j < linkedCompartment.size(); ++j) {
            if (isIn[j]) {
                subCompartmentValues[0] += linkedCompartment[j].lock()->outValue * linkedWeight[j];
            }
        }
    }

    // Finally sum up subCompartmentValues of this iteration to obtain total value
    for (auto value: subCompartmentValues) {
        total[iter] += value;
    }
}

TEST(CompartmentTest, update) {
    auto S = std::make_shared<SimpleCompartment>();
    auto E = std::make_shared<SimpleCompartment>();
    auto A = std::make_shared<SimpleCompartment>();
    auto A_r = std::make_shared<SimpleCompartment>();
    auto I = std::make_shared<SimpleCompartment>();
    auto R = std::make_shared<SimpleCompartment>();

    S->name = "S";
    S->subCompartmentValues = {10000};
    S->total.resize(100, 0);
    S->dist = {0.2};
    S->linkedCompartment = {E};
    S->isIn = {false};
    S->linkedWeight = {1.0};

    E->name = "E";
    E->subCompartmentValues = {500, 600, 700};
    E->total.resize(100, 0);
    E->dist = {0, 0.8, 1};
    E->linkedCompartment = {S, A};
    E->isIn = {true, false};
    E->linkedWeight = {1.0, 1.0};

    A->name = "A";
    A->subCompartmentValues = {5, 6, 7, 8};
    A->total.resize(100, 0);
    A->dist = {0, 0.3, 0.7, 1};
    A->linkedCompartment = {E, A_r, I};
    A->isIn = {true, false, false};
    A->linkedWeight = {1.0, 1.0, 1.0};

    A_r->name = "A_r";
    A_r->subCompartmentValues = {0, 0, 0};
    A_r->total.resize(100, 0);
    A_r->dist = {0, 0.4, 1};
    A_r->linkedCompartment = {A, R};
    A_r->isIn = {true, false};
    A_r->linkedWeight = {0.3, 1.0};

    I->name = "I";
    I->subCompartmentValues = {1, 0, 0};
    I->total.resize(100, 0);
    I->dist = {0, 0.5, 1};
    I->linkedCompartment = {A, R};
    I->isIn = {true, false};
    I->linkedWeight = {0.7, 1.0};

    R->name = "R";
    R->subCompartmentValues = {0};
    R->total.resize(100, 0);
    R->dist = {0};
    R->linkedCompartment = {A_r, I};
    R->isIn = {true, true};
    R->linkedWeight = {1.0, 1.0};

    std::vector<std::shared_ptr<SimpleCompartment>> model = {S, E, A, A_r, I, R};
    for(auto& comp: model) {
        comp->updateValue(1);
    }

    EXPECT_TRUE(true);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
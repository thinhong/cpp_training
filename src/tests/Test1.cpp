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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
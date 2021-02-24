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
    // S -> E -> A
    myModel.addCompsAndConnect(S, E);
    myModel.addCompsAndConnect(E, A);
    // A -> A_r and I
    myModel.addCompsAndConnect(A, A_r, 0.35);
    myModel.addCompsAndConnect(A, I, 0.65);
    // A_r -> R
    myModel.addCompsAndConnect(A_r, R);
    // I -> H_h, H_c and H_d
    myModel.addCompsAndConnect(I, H_h, 0.7);
    myModel.addCompsAndConnect(I, H_c, 0.2);
    myModel.addCompsAndConnect(I, H_d, 0.1);
    // H_h -> R
    myModel.addCompsAndConnect(H_h, R);
    // H_c -> C_c -> R
    myModel.addCompsAndConnect(H_c, C_c);
    myModel.addCompsAndConnect(C_c, R);
    // H_d -> C_d -> D
    myModel.addCompsAndConnect(H_d, C_d);
    myModel.addCompsAndConnect(C_d, D);

    // Test with a isCycle: A_r going back to E
    // myModel.addCompsAndConnect(A_r, E);
    myModel.sortComps();

    EXPECT_TRUE(myModel.getIndex(S) < myModel.getIndex(D));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
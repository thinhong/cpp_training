#include <gtest/gtest.h>
#include "../src/Distribution/Distribution.h"
#include "../src/Distribution/Distribution.cpp"
#include "../src/Distribution/TransitionProb.h"
#include "../src/Distribution/TransitionProb.cpp"
#include "../src/Distribution/DiscreteGammaDistribution.h"
#include "../src/Distribution/DiscreteGammaDistribution.cpp"
#include "../src/Distribution/CustomDistribution.h"
#include "../src/Distribution/CustomDistribution.cpp"
#include "../src/Compartment.h"
#include "../src/Compartment.cpp"
#include "../src/prob.h"
#include "../src/prob.cpp"
#include "../src/Model.h"
#include "../src/Model.cpp"

TEST(GammaTest, getCumulativeProb) {
    std::vector<double> cumProb = {0.5, 0.6, 0.7};
    DiscreteGammaDistribution dist(cumProb);
    EXPECT_EQ(0.5, dist.getTransitionProb(0));
}

TEST(GammaTest, calcCumulativeProb) {
    DiscreteGammaDistribution dist(1, 1);
    EXPECT_NEAR(0.6321206, dist.getTransitionProb(1), 0.00001);
    EXPECT_NEAR(0.8646647, dist.getTransitionProb(2), 0.00001);
    EXPECT_NEAR(0.9502129, dist.getTransitionProb(3), 0.00001);
}

TEST(CustomDistributionTest, getCumulativeProb) {
    std::vector<double> cumProb = {0.5, 0.6, 0.7};
    CustomDistribution dist(cumProb);
    EXPECT_TRUE(true);
}

TEST(ModelTest, test) {
    const double populationSize = 1000000;
    const double transRate = 2.0;
    auto forceInfection = std::make_shared<double>();
    std::vector<double> cumProb = {0.0, 0.3, 1};
    auto S = make_shared<Compartment>("S", 10000, make_shared<TransitionProb>(forceInfection));
    auto I = make_shared<Compartment>("I", 500, make_shared<DiscreteGammaDistribution>(cumProb));
    auto R = make_shared<Compartment>("R", 0, make_shared<TransitionProb>(make_shared<double>(0.0)));
    auto D = make_shared<Compartment>("D", 0, make_shared<TransitionProb>(make_shared<double>(0.0)));

    Model myModel;
    myModel.addCompsAndConnect(S, I, 1);
    myModel.addCompsAndConnect(I, R, 0.9);
    myModel.addCompsAndConnect(I, D, 0.1);

    std::vector<std::string> infectiousComps {"I"};

    myModel.sortComps();
    for (size_t i {1}; i < Compartment::timesFollowUp; i++) {
        double totalInfectious {0.0};
        for (auto& comp: myModel.getComps()) {
            for (std::string& iComp: infectiousComps) {
                if (comp->getName() == iComp) {
                    totalInfectious += comp->getTotal()[i - 1];
                }
            }
        }
        *forceInfection = transRate / populationSize * totalInfectious;
        myModel.update(i);
    }
    EXPECT_EQ(S->getName(), myModel.getComps()[0]->getName());

    size_t iter;
    double total;
    // Iteration 1
    iter = 1;
    EXPECT_EQ(9990, myModel.getComps()[0]->getTotal()[iter]);
    EXPECT_EQ(510, myModel.getComps()[1]->getTotal()[iter]);
    // Iteration 2
    iter = 2;
    EXPECT_NEAR(9979.81, myModel.getComps()[0]->getTotal()[iter], 0.01);
    EXPECT_NEAR(370.19, myModel.getComps()[1]->getTotal()[iter], 0.01);
    EXPECT_EQ(15, myModel.getComps()[2]->getTotal()[iter]);
    EXPECT_EQ(135, myModel.getComps()[3]->getTotal()[iter]);
    // Assure that total value remain constant
    total = 0;
    for (auto& comp: myModel.getComps()) {
        total += comp->getTotal()[iter];
    }
    EXPECT_NEAR(10500, total, 0.01);
    // Iteration 3
    iter = 3;
    EXPECT_NEAR(9972.421, myModel.getComps()[0]->getTotal()[iter], 0.01);
    EXPECT_NEAR(24.5786, myModel.getComps()[1]->getTotal()[iter], 0.01);
    EXPECT_NEAR(50.3, myModel.getComps()[2]->getTotal()[iter], 0.01);
    EXPECT_NEAR(452.7, myModel.getComps()[3]->getTotal()[iter], 0.01);
    // Assure that total value remain constant
    total = 0;
    for (auto& comp: myModel.getComps()) {
        total += comp->getTotal()[iter];
    }
    EXPECT_NEAR(10500, total, 0.01);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
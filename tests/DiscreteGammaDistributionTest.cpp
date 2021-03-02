#include <gtest/gtest.h>
#include "../src/Distribution/Distribution.h"
#include "../src/Distribution/Distribution.cpp"
#include "../src/Distribution/BernoulliDistribution.h"
#include "../src/Distribution/BernoulliDistribution.cpp"
#include "../src/Distribution/DiscreteGammaDistribution.h"
#include "../src/Distribution/DiscreteGammaDistribution.cpp"
#include "../src/Compartment.h"
#include "../src/Compartment.cpp"
#include "../src/prob.h"
#include "../src/prob.cpp"
#include "../src/Model.h"
#include "../src/Model.cpp"

TEST(GammaTest, getCumulativeProb) {
    std::vector<double> cumProb = {0.5, 0.6, 0.7};
    DiscreteGammaDistribution dist(cumProb, 3);
    EXPECT_EQ(0.5, dist.getCumulativeProb(0));
}

TEST(GammaTest, calcCumulativeProb) {
    DiscreteGammaDistribution dist(1, 1);
    EXPECT_NEAR(0.6321206, dist.getCumulativeProb(1), 0.00001);
    EXPECT_NEAR(0.8646647, dist.getCumulativeProb(2), 0.00001);
    EXPECT_NEAR(0.9502129, dist.getCumulativeProb(3), 0.00001);
}

TEST(ModelTest, test) {
    const double populationSize = 1000000;
    const double transRate = 2.0;
    auto forceInfection = std::make_shared<double>();
    std::vector<double> cumProb = {0.0, 0.8, 1};
    auto S = make_shared<Compartment>("S", 10000, make_shared<BernoulliDistribution>(forceInfection));
    auto E = make_shared<Compartment>("E", 500, make_shared<DiscreteGammaDistribution>(cumProb, 3));
    auto R = make_shared<Compartment>("R", 0, make_shared<BernoulliDistribution>(make_shared<double>(0.0)));

    Model myModel;
    myModel.addCompsAndConnect(S, E, 1);
    myModel.addCompsAndConnect(E, R, 1);

    std::vector<std::string> infectiousComps {"E"};

    myModel.sortComps();
    for (size_t i {1}; i < Compartment::daysFollowUp; i++) {
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
    EXPECT_EQ(9990, myModel.getComps()[0]->getTotal()[1]);
    EXPECT_EQ(510, myModel.getComps()[1]->getTotal()[1]);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/Distribution/DiscreteGammaDistribution.h"
#include "../src/Compartment.h"


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
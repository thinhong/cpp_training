cmake_minimum_required(VERSION 3.17)
project(SEAIHCR)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(tests)

add_executable(SEAIHCR main.cpp prob.cpp prob.hpp json.hpp Compartment.cpp Compartment.h Model.cpp Model.h Distribution.cpp Distribution.h DiscreteGammaDistribution.cpp DiscreteGammaDistribution.h DiscreteWeibullDistribution.cpp DiscreteWeibullDistribution.h BernoulliDistribution.cpp BernoulliDistribution.h File.cpp File.h FileCSV.cpp FileCSV.h FileJSON.cpp FileJSON.h ObjectJSON.cpp ObjectJSON.h)
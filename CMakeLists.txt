cmake_minimum_required(VERSION 3.17)
project(SEAIHCR)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(tests)

add_executable(SEAIHCR main.cpp
        src/prob.cpp src/prob.hpp
        src/json.hpp src/Compartment.cpp src/Compartment.h src/Model.cpp src/Model.h src/Distribution.cpp src/Distribution.h src/DiscreteGammaDistribution.cpp src/DiscreteGammaDistribution.h src/DiscreteWeibullDistribution.cpp src/DiscreteWeibullDistribution.h src/BernoulliDistribution.cpp src/BernoulliDistribution.h src/File.cpp src/File.h src/FileCSV.cpp src/FileCSV.h src/FileJSON.cpp src/FileJSON.h src/ObjectJSON.cpp src/ObjectJSON.h)
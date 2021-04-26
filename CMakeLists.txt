cmake_minimum_required(VERSION 3.17)
project(SEAIHCR)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(test)

add_executable(SEAIHCR src/main.cpp
        src/prob.cpp src/prob.h src/json.h
        src/Compartment.cpp src/Compartment.h src/Model.cpp src/Model.h
        src/Distribution/Distribution.cpp src/Distribution/Distribution.h
        src/Distribution/DiscreteGammaDistribution.cpp src/Distribution/DiscreteGammaDistribution.h
        src/Distribution/DiscreteWeibullDistribution.cpp src/Distribution/DiscreteWeibullDistribution.h
        src/Distribution/TransitionProb.cpp src/Distribution/TransitionProb.h
        src/Distribution/DiscreteExponentialDistribution.cpp src/Distribution/DiscreteExponentialDistribution.h
        src/Distribution/CustomDistribution.cpp src/Distribution/CustomDistribution.h
        src/File.cpp src/File.h src/FileCSV.cpp src/FileCSV.h src/FileJSON.cpp src/FileJSON.h src/CompartmentJSON.cpp src/CompartmentJSON.h src/FullModel.cpp src/FullModel.h src/Contact.cpp src/Contact.h src/Matrix.cpp src/Matrix.h)
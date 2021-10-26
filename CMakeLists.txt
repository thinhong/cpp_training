cmake_minimum_required(VERSION 3.17)
project(discreteModel)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(test)

add_executable(discreteModel src/main.cpp
        src/myProb.cpp src/myProb.h src/json.h
        src/Compartment.cpp src/Compartment.h src/Model.cpp src/Model.h
        src/Distribution.cpp src/Distribution.h
        src/DistributionDiscreteGamma.cpp src/DistributionDiscreteGamma.h
        src/DistributionDiscreteWeibull.cpp src/DistributionDiscreteWeibull.h
        src/DistributionTransitionProb.cpp src/DistributionTransitionProb.h
        src/DistributionDiscreteExponential.cpp src/DistributionDiscreteExponential.h
        src/DistributionValues.cpp src/DistributionValues.h
        src/File.cpp src/File.h src/FileCSV.cpp src/FileCSV.h
        src/muParser.h src/muParser.cpp src/muParserBase.cpp src/muParserBase.h
        src/muParserBytecode.cpp src/muParserBytecode.h src/muParserCallback.cpp src/muParserCallback.h src/muParserDef.h
        src/muParserError.cpp src/muParserError.h src/muParserFixes.h src/muParserTemplateMagic.h src/muParserTemplateMagic.h
        src/muParserToken.h src/muParserTokenReader.h src/muParserTokenReader.cpp src/DistributionMathExpression.cpp src/DistributionMathExpression.h src/ModelJSON.cpp src/ModelJSON.h src/helpers.cpp src/helpers.h src/DistributionFrequency.cpp src/DistributionFrequency.h src/myProb.cpp src/myProb.h)
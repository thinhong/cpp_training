cmake_minimum_required(VERSION 3.17)
project(discreteModel)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(test)

add_executable(discreteModel src/main.cpp
        src/Distribution/myProb.cpp src/Distribution/myProb.h src/json.h
        src/Compartment.cpp src/Compartment.h src/Model.cpp src/Model.h
        src/Distribution/Distribution.cpp src/Distribution/Distribution.h
        src/Distribution/DiscreteGammaDistribution.cpp src/Distribution/DiscreteGammaDistribution.h
        src/Distribution/DiscreteWeibullDistribution.cpp src/Distribution/DiscreteWeibullDistribution.h
        src/Distribution/TransitionProb.cpp src/Distribution/TransitionProb.h
        src/Distribution/DiscreteExponentialDistribution.cpp src/Distribution/DiscreteExponentialDistribution.h
        src/Distribution/ValuesDistribution.cpp src/Distribution/ValuesDistribution.h
        src/File.cpp src/File.h src/FileCSV.cpp src/FileCSV.h
        src/muParser/muParser.h src/muParser/muParser.cpp src/muParser/muParserBase.cpp src/muParser/muParserBase.h
        src/muParser/muParserBytecode.cpp src/muParser/muParserBytecode.h src/muParser/muParserCallback.cpp src/muParser/muParserCallback.h src/muParser/muParserDef.h
        src/muParser/muParserError.cpp src/muParser/muParserError.h src/muParser/muParserFixes.h src/muParser/muParserTemplateMagic.h src/muParser/muParserTemplateMagic.h
        src/muParser/muParserToken.h src/muParser/muParserTokenReader.h src/muParser/muParserTokenReader.cpp src/Distribution/MathExpression.cpp src/Distribution/MathExpression.h src/ModelJSON.cpp src/ModelJSON.h src/helpers.cpp src/helpers.h src/Distribution/Frequency.cpp src/Distribution/Frequency.h src/Distribution/myProb.cpp src/Distribution/myProb.h)
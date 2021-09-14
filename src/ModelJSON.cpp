//
// Created by thinh on 22/07/2021.
//

#include "ModelJSON.h"

ModelJSON::ModelJSON(nlohmann::json &initialValues, nlohmann::json &parameters, nlohmann::json &transitions) {

    // Set parameters with parameters json
    std::vector<std::string> paramNames;
    std::vector<double> paramValues;
    for (auto& param: parameters.items()) {
        // All keys to paramNames and all values to paramValues
        paramNames.push_back(param.key());
        paramValues.push_back(param.value());
    }
    model = std::make_shared<Model>(paramNames, paramValues);

    // Create all compartments with initialValues json
    std::vector<std::shared_ptr<Compartment>> allCompartments;
    for (auto& initVal: initialValues.items()) {
        std::shared_ptr<Compartment> comp = std::make_shared<Compartment>(initVal.key(), initVal.value());
        allCompartments.push_back(comp);
    }
    model->addCompsFromConfig(allCompartments);

    // Then add other compartment attributes with the transitions json
    for (auto& transition: transitions.items()) {
        std::string flow = transition.key();
        auto distributionConfig = transition.value();
        // Remove whitespace
        flow.erase(remove(flow.begin(), flow.end(), ' '), flow.end());

        int transitionSymbol_pos = flow.find("->");
        // Check whether there is a ":" symbol in this flow
        int probSymbol_pos = flow.find(':');

        // [inComp] [->] [outComp] [:] [prob]
        // inComp start from position 0 and spread from 0 -> transitionSymbol_pos => length = transitionSymbol_pos - 0 = transitionSymbol_pos
        std::string inCompName = flow.substr(0, transitionSymbol_pos);
        // outComp start from transitionSymbol_pos + 2 (transitionSymbol_pos is "->" therefore occupies 2 positions), and
        // spread from transitionSymbol_pos + 2 to probSymbol_pos => length = probSymbol_pos - (transitionSymbol_pos + 2)
        std::string outCompName = flow.substr(transitionSymbol_pos + 2, probSymbol_pos - (transitionSymbol_pos + 2));
        // prob start from probSymbol_pos + 1 and spread to the end of the string
        double weight;
        if (probSymbol_pos != -1) {
            weight = std::stod(flow.substr(probSymbol_pos + 1));
        } else {
            weight = 1;
        }
        std::weak_ptr<Compartment> inComp = model->getAddressFromName(inCompName);
        std::weak_ptr<Compartment> outComp = model->getAddressFromName(outCompName);

        // Set linked compartment in and out and their weight
        inComp.lock()->addOutCompartment(outComp);
        outComp.lock()->addInCompartment(inComp);
        inComp.lock()->addOutWeight(weight);

        // Set distribution for the inCompartment
        if (distributionConfig["distribution"] == "transitionProb") {
            double prob = distributionConfig["transitionProb"];
            prob *= Distribution::timeStep;
            auto transitionProb = std::make_shared<TransitionProb>(prob);
            inComp.lock()->addOutDistribution(transitionProb);
        }
            // Gamma distribution: parameters are "scale" and "shape"
        else if (distributionConfig["distribution"] == "gamma") {
            double scale = distributionConfig["scale"];
            scale /= Distribution::timeStep;
            double shape = distributionConfig["shape"];
            auto gamma = std::make_shared<DiscreteGammaDistribution>(scale, shape);
            inComp.lock()->addOutDistribution(gamma);
        }
            // Weibull distribution: parameters are "scale" and "shape"
        else if (distributionConfig["distribution"] == "weibull") {
            double scale = distributionConfig["scale"];
            scale /= Distribution::timeStep;
            double shape = distributionConfig["shape"];
            auto weibull = std::make_shared<DiscreteWeibullDistribution>(scale, shape);
            inComp.lock()->addOutDistribution(weibull);
        }
            // Exponential distribution: parameter is "rate"
        else if (distributionConfig["distribution"] == "exponential") {
            double rate = distributionConfig["rate"];
            rate *= Distribution::timeStep;
            auto exponential = std::make_shared<DiscreteExponentialDistribution>(rate);
            inComp.lock()->addOutDistribution(exponential);
        }
            // Custom distribution: parameter is a vector "waitingTime"
        else if (distributionConfig["distribution"] == "custom") {
            std::vector<double> waitingTime = distributionConfig["waitingTime"];
            auto custom = std::make_shared<CustomDistribution>(waitingTime);
            inComp.lock()->addOutDistribution(custom);
        }
        else if (distributionConfig["distribution"] == "mathExpression") {
            std::string expression = distributionConfig["expression"];
            auto mathExpression = std::make_shared<MathExpression>(expression);
            inComp.lock()->addOutDistribution(mathExpression);
        }
    }
    for (auto& comp: model->getComps()) {
        if (comp->getOutDistributions().empty()) {
            double prob = 0.0;
            auto transitionProb = std::make_shared<TransitionProb>(prob);
            comp->addOutDistribution(transitionProb);
        }
        comp->setLengthSubCompartment();
        comp->setOutValues();
    }
}

std::shared_ptr<Model> ModelJSON::getModel() {
    return model;
}
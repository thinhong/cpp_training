//
// Created by thinh on 22/02/2021.
//

#include "CompartmentJSON.h"

CompartmentJSON::CompartmentJSON(nlohmann::json& jsonNode) {
    // Directly add transition probability: parameter is "transitionProb"
    if (jsonNode["distribution"]["name"] == "transitionProb") {
        auto transitionProb = std::make_shared<TransitionProb>(jsonNode["distribution"]["transitionProb"]);
        comp = std::make_shared<Compartment>(jsonNode["name"], jsonNode["initialValue"], transitionProb);
    }
        // Gamma distribution: parameters are "scale" and "shape"
    else if (jsonNode["distribution"]["name"] == "gamma") {
        auto gamma = std::make_shared<DiscreteGammaDistribution>(jsonNode["distribution"]["scale"], jsonNode["distribution"]["shape"]);
        comp = std::make_shared<Compartment>(jsonNode["name"], jsonNode["initialValue"], gamma);
    }
        // Weibull distribution: parameters are "scale" and "shape"
    else if (jsonNode["distribution"]["name"] == "weibull") {
        auto weibull = std::make_shared<DiscreteWeibullDistribution>(jsonNode["distribution"]["scale"], jsonNode["distribution"]["shape"]);
        comp = std::make_shared<Compartment>(jsonNode["name"], jsonNode["initialValue"], weibull);
    }
        // Exponential distribution: parameter is "rate"
    else if (jsonNode["distribution"]["name"] == "exponential") {
        auto exponential = std::make_shared<DiscreteExponentialDistribution>(jsonNode["distribution"]["rate"]);
        comp = std::make_shared<Compartment>(jsonNode["name"], jsonNode["initialValue"], exponential);
    }
        // Custom distribution: parameter is a vector "waitingTime"
    else if (jsonNode["distribution"]["name"] == "custom") {
        std::vector<double> waitingTime = jsonNode["distribution"]["waitingTime"];
        auto custom = std::make_shared<CustomDistribution>(waitingTime);
        comp = std::make_shared<Compartment>(jsonNode["name"], jsonNode["initialValue"], custom);
    }

    // Also add linkedWeight and isIn to the compartment
    for (double weight: jsonNode["linkedWeight"]) {
        comp->addLinkedWeight(weight);
    }
    for (bool isIn: jsonNode["isIn"]) {
        comp->addIsIn(isIn);
    }
}

std::shared_ptr<Compartment> CompartmentJSON::getComp() {
    return comp;
}

nlohmann::json CompartmentJSON::compToJSON() {
    nlohmann::json jsonNode;
    jsonNode["name"] = comp->getName();
    jsonNode["initialValue"] = comp->getTotal()[0];
    if (comp->getDist()->getDistName() == "transitionProb") {
        jsonNode["distribution"] = {{"name", comp->getDist()->getDistName()}, {"transitionProb", comp->getDist()
                                                                                                  ->getTransitionProb(0)}};
    }
    else if (comp->getDist()->getDistName() == "gamma") {
        auto castedDist = std::dynamic_pointer_cast<DiscreteGammaDistribution>(comp->getDist());
        jsonNode["distribution"] = {{"name", castedDist->getDistName()}, {"scale", castedDist->getScale()}, {"shape", castedDist->getShape()}};
    }
    else if (comp->getDist()->getDistName() == "weibull") {
        auto castedDist = std::dynamic_pointer_cast<DiscreteWeibullDistribution>(comp->getDist());
        jsonNode["distribution"] = {{"name", castedDist->getDistName()}, {"scale", castedDist->getScale()}, {"shape", castedDist->getShape()}};
    }
    else if (comp->getDist()->getDistName() == "exponential") {
        auto castedDist = std::dynamic_pointer_cast<DiscreteExponentialDistribution>(comp->getDist());
        jsonNode["distribution"] = {{"name", castedDist->getDistName()}, {"rate", castedDist->getRate()}};
    }
    else if (comp->getDist()->getDistName() == "custom") {
        auto castedDist = std::dynamic_pointer_cast<CustomDistribution>(comp->getDist());
        jsonNode["distribution"] = {{"name", castedDist->getDistName()}, {"waitingTime", castedDist->getWaitingTime()}};
    }
    jsonNode["linkedCompartment"] = {};
    for (auto& i: comp->getLinkedCompartment()) {
        jsonNode["linkedCompartment"] += i.lock()->getName();
    }
    jsonNode["isIn"] = comp->getIsIn();
    jsonNode["linkedWeight"] = comp->getLinkedWeight();
    return jsonNode;
}
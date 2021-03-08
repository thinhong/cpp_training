//
// Created by thinh on 22/02/2021.
//

#include "CompartmentJSON.h"

CompartmentJSON::CompartmentJSON(nlohmann::json& jsonNode) {
    this->jsonNode = jsonNode;
}

std::shared_ptr<Compartment> CompartmentJSON::compFromJSON() {
    std::shared_ptr<Compartment> comp;
    if (jsonNode["distribution"]["name"] == "bernoulli") {
        auto bernoulli = std::make_shared<BernoulliDistribution>(std::make_shared<double>(jsonNode["distribution"]["successRate"]));
        comp = std::make_shared<Compartment>(jsonNode["name"], jsonNode["initialValue"], bernoulli);
    } else if (jsonNode["distribution"]["name"] == "gamma") {
        auto gamma = std::make_shared<DiscreteGammaDistribution>(jsonNode["distribution"]["scale"], jsonNode["distribution"]["shape"]);
        comp = std::make_shared<Compartment>(jsonNode["name"], jsonNode["initialValue"], gamma);
    } else if (jsonNode["distribution"]["name"] == "weibull") {
        auto weibull = std::make_shared<DiscreteWeibullDistribution>(jsonNode["distribution"]["scale"], jsonNode["distribution"]["shape"]);
        comp = std::make_shared<Compartment>(jsonNode["name"], jsonNode["initialValue"], weibull);
    } else if (jsonNode["distribution"]["name"] == "exponential") {
        auto exponential = std::make_shared<DiscreteExponentialDistribution>(jsonNode["distribution"]["rate"]);
        comp = std::make_shared<Compartment>(jsonNode["name"], jsonNode["initialValue"], exponential);
    }
    else if (jsonNode["distribution"]["name"] == "custom") {
        std::vector<double> prob = jsonNode["distribution"]["prob"];
        auto custom = std::make_shared<CustomDistribution>(prob);
        comp = std::make_shared<Compartment>(jsonNode["name"], jsonNode["initialValue"], custom);
    }
    // Also add linkedWeight and isIn to the compartment since they are all numeric values
    for (double weight: jsonNode["linkedWeight"]) {
        comp->addLinkedWeight(weight);
    }
    for (bool isIn: jsonNode["isIn"]) {
        comp->addIsIn(isIn);
    }
    return comp;
}

nlohmann::json CompartmentJSON::compToJSON(std::shared_ptr<Compartment> &comp) {
    jsonNode["name"] = comp->getName();
    jsonNode["initialValue"] = comp->getTotal()[0];
    if (comp->getDist()->getDistName() == "gamma") {
        auto castedDist = std::dynamic_pointer_cast<DiscreteGammaDistribution>(comp->getDist());
        jsonNode["distribution"] = {{"name", comp->getDist()->getDistName()}, {"scale", castedDist->getScale()}, {"shape", castedDist->getShape()}};
    } else if (comp->getDist()->getDistName() == "weibull") {
        auto castedDist = std::dynamic_pointer_cast<DiscreteWeibullDistribution>(comp->getDist());
        jsonNode["distribution"] = {{"name", comp->getDist()->getDistName()}, {"scale", castedDist->getScale()}, {"shape", castedDist->getShape()}};
    } else if (comp->getDist()->getDistName() == "bernoulli") {
        jsonNode["distribution"] = {{"name", comp->getDist()->getDistName()}, {"successRate", comp->getDist()->getCumulativeProb(0)}};
    } else if (comp->getDist()->getDistName() == "exponential") {
        auto castedDist = std::dynamic_pointer_cast<DiscreteExponentialDistribution>(comp->getDist());
        jsonNode["distribution"] = {{"name", comp->getDist()->getDistName()}, {"rate", castedDist->getRate()}};
    }
    else if (comp->getDist()->getDistName() == "custom") {
        jsonNode["distribution"] = {{"name", comp->getDist()->getDistName()}, {"successRate", comp->getDist()->getCumulativeProb(0)}};
    }
    jsonNode["linkedCompartment"] = {};
    for (auto i: comp->getLinkedCompartment()) {
        jsonNode["linkedCompartment"] += i.lock()->getName();
    }
    jsonNode["isIn"] = comp->getIsIn();
    jsonNode["linkedWeight"] = comp->getLinkedWeight();
    return jsonNode;
}
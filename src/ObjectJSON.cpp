//
// Created by thinh on 22/02/2021.
//

#include "ObjectJSON.h"

nlohmann::json ObjectJSON::toJSON(std::shared_ptr<Compartment> &comp) {
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
    }
    jsonNode["linkedCompartment"] = {};
    for (auto i: comp->getLinkedCompartment()) {
        jsonNode["linkedCompartment"] += i.lock()->getName();
    }
    jsonNode["isIn"] = comp->getIsIn();
    jsonNode["linkedWeight"] = comp->getLinkedWeight();
    return jsonNode;
}
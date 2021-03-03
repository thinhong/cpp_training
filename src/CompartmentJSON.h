//
// Created by thinh on 22/02/2021.
//

#ifndef MAIN_CPP_COMPARTMENTJSON_H
#define MAIN_CPP_COMPARTMENTJSON_H

#include "json.h"
#include <memory>
#include "Compartment.h"
#include "Distribution/BernoulliDistribution.h"
#include "Distribution/DiscreteGammaDistribution.h"
#include "Distribution/DiscreteWeibullDistribution.h"

class CompartmentJSON {
private:
    nlohmann::json jsonNode;
public:
    CompartmentJSON() = default;
    explicit CompartmentJSON(nlohmann::json& jsonNode);
    std::shared_ptr<Compartment> compFromJSON();
    nlohmann::json compToJSON(std::shared_ptr<Compartment>& comp);
    nlohmann::json getJsonNode() {return jsonNode;};
};


#endif //MAIN_CPP_COMPARTMENTJSON_H

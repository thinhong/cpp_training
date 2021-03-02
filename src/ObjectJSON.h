//
// Created by thinh on 22/02/2021.
//

#ifndef MAIN_CPP_OBJECTJSON_H
#define MAIN_CPP_OBJECTJSON_H

#include "json.h"
#include <memory>
#include "Compartment.h"
#include "Distribution/DiscreteGammaDistribution.h"
#include "Distribution/DiscreteWeibullDistribution.h"

class ObjectJSON {
    nlohmann::json jsonNode;
public:
    void fromJSON();
    nlohmann::json toJSON(std::shared_ptr<Compartment>& comp);
    nlohmann::json getJsonNode() {return jsonNode;};
};


#endif //MAIN_CPP_OBJECTJSON_H

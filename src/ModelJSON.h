//
// Created by thinh on 22/07/2021.
//

#ifndef MAIN_CPP_MODELJSON_H
#define MAIN_CPP_MODELJSON_H

#include "json.h"
#include <memory>
#include "Compartment.h"
#include "Model.h"
#include "Distribution/TransitionProb.h"
#include "Distribution/DiscreteGammaDistribution.h"
#include "Distribution/DiscreteWeibullDistribution.h"
#include "Distribution/DiscreteExponentialDistribution.h"
#include "Distribution/CustomDistribution.h"
#include "Distribution/MathExpression.h"

class ModelJSON {
private:
    std::shared_ptr<Model> model;
public:
    explicit ModelJSON(nlohmann::json& initialValues, nlohmann::json& parameters, nlohmann::json& transitions);
    std::shared_ptr<Model> getModel();
};


#endif //MAIN_CPP_MODELJSON_H

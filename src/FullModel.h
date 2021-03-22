#ifndef MAIN_CPP_FULLMODEL_H
#define MAIN_CPP_FULLMODEL_H

#include <vector>
#include <memory>
#include "Model.h"

class FullModel {
private:
    std::vector<std::shared_ptr<Compartment>> models;
public:
    FullModel() = default;

};


#endif //MAIN_CPP_FULLMODEL_H

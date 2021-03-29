#ifndef MAIN_CPP_FULLMODEL_H
#define MAIN_CPP_FULLMODEL_H

#include <vector>
#include <memory>
#include "Model.h"

class FullModel {
private:
    std::vector<std::shared_ptr<Model>> models;
    std::vector<std::string> locationContacts;
public:
    explicit FullModel(std::vector<std::string> locationContacts);
    void addModel(std::shared_ptr<Model> model);

    // Helper functions
    std::weak_ptr<Model> getAddressFromName(std::string modelName);
    std::vector<std::shared_ptr<Model>> getModels();

    void connectLocations(std::string contactSymbol, std::string probSymbol);
};


#endif //MAIN_CPP_FULLMODEL_H

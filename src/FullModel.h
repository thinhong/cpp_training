#ifndef MAIN_CPP_FULLMODEL_H
#define MAIN_CPP_FULLMODEL_H

#include <vector>
#include <memory>
#include "Model.h"
#include "Contact.h"

class FullModel {
private:
    std::vector<std::shared_ptr<Model>> models;
    std::vector<std::shared_ptr<Contact>> allContacts;
public:
    explicit FullModel(std::vector<std::shared_ptr<Contact>> allContacts);
    void addModel(std::shared_ptr<Model> model);

    // Helper functions
    std::vector<std::shared_ptr<Model>> getModels();
    std::vector<std::weak_ptr<Model>> getModelsWithGroup(std::vector<std::string> groupToGet);
    double getContactProbByComparingPairs(std::vector<std::string> modelGroup);

    void connectModels();
};


#endif //MAIN_CPP_FULLMODEL_H

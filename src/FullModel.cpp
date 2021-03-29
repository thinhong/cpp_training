#include "FullModel.h"
#include <algorithm>

FullModel::FullModel(std::vector<std::string> locationContacts) {
    this->locationContacts = locationContacts;
}

void FullModel::addModel(std::shared_ptr<Model> model) {
    models.push_back(model);
}

std::weak_ptr<Model> FullModel::getAddressFromName(std::string modelName) {
    std::weak_ptr<Model> modelAddress;
    for (auto& model: models) {
        if (model->getName() == modelName) {
            modelAddress = model;
        }
    }
    return modelAddress;
}

std::vector<std::shared_ptr<Model>> FullModel::getModels() {
    return models;
}

void FullModel::connectLocations(std::string contactSymbol, std::string probSymbol) {
    for (std::string locationContact: locationContacts) {
        // Remove whitespace
        locationContact.erase(remove(locationContact.begin(), locationContact.end(), ' '), locationContact.end());

        int contactSymbol_pos = locationContact.find(contactSymbol);
        // Check whether there is a ":" symbol in this flow
        int probSymbol_pos = locationContact.find(probSymbol);

        // [location 1] [<->] [location 2] [:] [prob]
        // location 1 start from position 0 and spread from 0 -> contactSymbol_pos => length = contactSymbol_pos - 0 = contactSymbol_pos
        std::string location1Name = locationContact.substr(0, contactSymbol_pos);
        // location 2 start from contactSymbol_pos + 3 (contactSymbol_pos is "<->" therefore occupies 3 positions), and
        // spread from contactSymbol_pos + 3 to probSymbol_pos => length = probSymbol_pos - (contactSymbol_pos + 2)
        std::string location2Name = locationContact.substr(contactSymbol_pos + 3, probSymbol_pos - (contactSymbol_pos + 3));
        // prob start from probSymbol_pos + 1 and spread to the end of the string
        double prob;
        if (probSymbol_pos != -1) {
            prob = std::stod(locationContact.substr(probSymbol_pos + 1));
        } else {
            prob = 1;
        }
        if (location1Name == location2Name) {
            std::weak_ptr<Model> location = this->getAddressFromName(location1Name);
            location.lock()->setSelfContactProb(prob);
        }
        if (location1Name != location2Name) {
            std::weak_ptr<Model> location1 = this->getAddressFromName(location1Name);
            std::weak_ptr<Model> location2 = this->getAddressFromName(location2Name);
            location1.lock()->addLinkedLocation(location2);
            location2.lock()->addLinkedLocation(location1);
            location1.lock()->addLinkedContactProb(prob);
            location2.lock()->addLinkedContactProb(prob);
        }
    }
}
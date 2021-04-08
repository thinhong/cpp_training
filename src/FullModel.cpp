#include "FullModel.h"
#include <algorithm>
#include <stdexcept>

FullModel::FullModel(std::vector<std::shared_ptr<Contact>> allContacts) {
    this->allContacts = allContacts;
}

void FullModel::addModel(std::shared_ptr<Model> model) {
    models.push_back(model);
}

std::vector<std::shared_ptr<Model>> FullModel::getModels() {
    return models;
}

std::vector<std::weak_ptr<Model>> FullModel::getModelsWithSpecificGroup(std::vector<std::string> groupToGet) {
    std::vector<std::weak_ptr<Model>> results;
    for (auto model: models) {
        std::vector<std::string> modelGroup = model->getModelGroup();
        // Check whether a vector is a subset of another vector
        // https://stackoverflow.com/questions/6906521/how-to-check-whether-a-vector-is-a-subset-of-another-in-c
        std::sort(modelGroup.begin(), modelGroup.end());
        std::sort(groupToGet.begin(), groupToGet.end());
        if (std::includes(modelGroup.begin(), modelGroup.end(), groupToGet.begin(), groupToGet.end())) {
            results.push_back(model);
        }
    }
    return results;
}

double FullModel::getContactRateByComparingPairs(std::vector<std::string> modelGroup) {
    double contactProb {0};
    for (auto& contactConfig: allContacts) {
        for (std::string contactRateConfig: contactConfig->getContactRates()) {
            // Remove whitespace
            contactRateConfig.erase(remove(contactRateConfig.begin(), contactRateConfig.end(), ' '), contactRateConfig.end());

            int contactSymbol_pos = contactRateConfig.find("<->");
            // Check whether there is a ":" symbol in this flow
            int probSymbol_pos = contactRateConfig.find(':');

            // [location 1] [<->] [location 2] [:] [prob]
            // location 1 start from position 0 and spread from 0 -> contactSymbol_pos => length = contactSymbol_pos - 0 = contactSymbol_pos
            std::string group1Name = contactRateConfig.substr(0, contactSymbol_pos);
            // location 2 start from contactSymbol_pos + 3 (contactSymbol_pos is "<->" therefore occupies 3 positions), and
            // spread from contactSymbol_pos + 3 to probSymbol_pos => length = probSymbol_pos - (contactSymbol_pos + 2)
            std::string group2Name = contactRateConfig.substr(contactSymbol_pos + 3, probSymbol_pos - (contactSymbol_pos + 3));
            // prob start from probSymbol_pos + 1 and spread to the end of the string
            double prob;
            if (probSymbol_pos != -1) {
                prob = std::stod(contactRateConfig.substr(probSymbol_pos + 1));
            } else {
                prob = 0.0;
            }
            std::vector<std::string> classPairConfig;
            classPairConfig.push_back(group1Name);
            classPairConfig.push_back(group2Name);
            // Sort before comparing because we want to compare modelGroup and classPairConfig ignoring the order
            std::sort(modelGroup.begin(), modelGroup.end());
            std::sort(classPairConfig.begin(), classPairConfig.end());

            if (classPairConfig == modelGroup) {
                contactProb = prob;
            }
        }
    }
    return contactProb;
}

void FullModel::connectModels() {
    // Create a vector of weak pointer to all models
    std::vector<std::weak_ptr<Model>> allModels;
    for (auto& model: models) {
        allModels.push_back(model);
    }
    // Now begin the connection process
    for (auto& model: models) {
        // Set linkedModels as all models
        model->addLinkedModels(allModels);
        size_t lastIndex = model->getModelGroup().size() - 1;
        for (auto& linkedModel: model->getLinkedModels()) {
            std::vector<std::string> classPair;
            classPair.push_back(model->getModelGroup()[lastIndex]);
            classPair.push_back(linkedModel.lock()->getModelGroup()[lastIndex]);
            model->addNewLinkedContactRate(getContactRateByComparingPairs(classPair));
        }
        std::vector<std::string> groupToSubset;
        for (size_t i {0}; i <= lastIndex - 1; ++i) {
            groupToSubset.push_back(model->getModelGroup()[lastIndex - i]);
            std::vector<std::weak_ptr<Model>> subsetModels = getModelsWithSpecificGroup(groupToSubset);
            for (auto& subsetModel: subsetModels) {
                std::vector<std::string> classPair;
                classPair.push_back(model->getModelGroup()[lastIndex - i - 1]);
                classPair.push_back(subsetModel.lock()->getModelGroup()[lastIndex - i - 1]);
                // Get index of this subModel
                int k = model->getIndexLinkedModel(subsetModel.lock()->getModelGroup());
                model->updateLinkedContactRate(getContactRateByComparingPairs(classPair), k);
            }
        }
    }
}
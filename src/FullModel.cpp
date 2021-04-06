#include "FullModel.h"
#include <algorithm>

FullModel::FullModel(std::vector<std::shared_ptr<Contact>> allContacts) {
    this->allContacts = allContacts;
}

void FullModel::addModel(std::shared_ptr<Model> model) {
    models.push_back(model);
}

//std::weak_ptr<Model> FullModel::getAddressFromName(std::string modelGroup) {
//    std::weak_ptr<Model> modelAddress;
//    for (auto& model: models) {
//        if (model->getModelGroup() == modelGroup) {
//            modelAddress = model;
//        }
//    }
//    return modelAddress;
//}

std::vector<std::shared_ptr<Model>> FullModel::getModels() {
    return models;
}

std::vector<std::weak_ptr<Model>> FullModel::getModelsFromGroup(std::string modelGroup) {
    std::vector<std::weak_ptr<Model>> results;
    for (auto model: models) {
        std::vector<std::string> group = model->getModelGroup();
        if (std::find(group.begin(), group.end(), modelGroup) != group.end()) {
            results.push_back(model);
        }
    }
    return results;
}

void FullModel::connectModels() {
    // Create a vector of weak pointer to all models
    std::vector<std::weak_ptr<Model>> allModels;
    for (auto model: models) {
        allModels.push_back(model);
    }

    // Now begin the connection process
    for (auto& model: models) {
        // Set linkedModels as all models
        model->addLinkedModels(allModels);
        size_t i {1};
            for (auto linkedModel: model->getLinkedModels()) {
                std::vector<std::string> classPair;
                classPair.push_back(model->getModelGroup()[i]);
                classPair.push_back(linkedModel.lock()->getModelGroup()[i]);
                // Sort classPair
                std::sort(classPair.begin(), classPair.end());
                // Compare this classPair to classPair in config contactProb
                // First we need to reconstruct the contactProb vector
                for (auto contactConfig: allContacts) {
                    for (std::string contactProbConfig: contactConfig->getContactProbs()) {
                        // Remove whitespace
                        contactProbConfig.erase(remove(contactProbConfig.begin(), contactProbConfig.end(), ' '), contactProbConfig.end());

                        int contactSymbol_pos = contactProbConfig.find("<->");
                        // Check whether there is a ":" symbol in this flow
                        int probSymbol_pos = contactProbConfig.find(':');

                        // [location 1] [<->] [location 2] [:] [prob]
                        // location 1 start from position 0 and spread from 0 -> contactSymbol_pos => length = contactSymbol_pos - 0 = contactSymbol_pos
                        std::string group1Name = contactProbConfig.substr(0, contactSymbol_pos);
                        // location 2 start from contactSymbol_pos + 3 (contactSymbol_pos is "<->" therefore occupies 3 positions), and
                        // spread from contactSymbol_pos + 3 to probSymbol_pos => length = probSymbol_pos - (contactSymbol_pos + 2)
                        std::string group2Name = contactProbConfig.substr(contactSymbol_pos + 3, probSymbol_pos - (contactSymbol_pos + 3));
                        // prob start from probSymbol_pos + 1 and spread to the end of the string
                        double prob;
                        if (probSymbol_pos != -1) {
                            prob = std::stod(contactProbConfig.substr(probSymbol_pos + 1));
                        } else {
                            prob = 1;
                        }
                        std::vector<std::string> classPairConfig;
                        classPairConfig.push_back(group1Name);
                        classPairConfig.push_back(group2Name);
                        // Sort classPairConfig
                        std::sort(classPairConfig.begin(), classPairConfig.end());

                        if (classPairConfig == classPair) {
                            model->addNewLinkedContactProb(prob);
                        }
                    }
                }
            }
        std::cout << "\n";
        std::vector<std::weak_ptr<Model>> subsetModels = getModelsFromGroup(model->getModelGroup()[i]);
        for (auto subsetModel: subsetModels) {
            std::vector<std::string> classPair;
            classPair.push_back(model->getModelGroup()[i-1]);
            classPair.push_back(subsetModel.lock()->getModelGroup()[i-1]);
            std::sort(classPair.begin(), classPair.end());
            // Get index of this subModel
            int k = model->getIndexLinkedModel(subsetModel.lock()->getModelGroup());
            for (auto contactConfig: allContacts) {
                for (std::string contactProbConfig: contactConfig->getContactProbs()) {
                    // Remove whitespace
                    contactProbConfig.erase(remove(contactProbConfig.begin(), contactProbConfig.end(), ' '), contactProbConfig.end());

                    int contactSymbol_pos = contactProbConfig.find("<->");
                    // Check whether there is a ":" symbol in this flow
                    int probSymbol_pos = contactProbConfig.find(':');

                    // [location 1] [<->] [location 2] [:] [prob]
                    // location 1 start from position 0 and spread from 0 -> contactSymbol_pos => length = contactSymbol_pos - 0 = contactSymbol_pos
                    std::string group1Name = contactProbConfig.substr(0, contactSymbol_pos);
                    // location 2 start from contactSymbol_pos + 3 (contactSymbol_pos is "<->" therefore occupies 3 positions), and
                    // spread from contactSymbol_pos + 3 to probSymbol_pos => length = probSymbol_pos - (contactSymbol_pos + 2)
                    std::string group2Name = contactProbConfig.substr(contactSymbol_pos + 3, probSymbol_pos - (contactSymbol_pos + 3));
                    // prob start from probSymbol_pos + 1 and spread to the end of the string
                    double prob;
                    if (probSymbol_pos != -1) {
                        prob = std::stod(contactProbConfig.substr(probSymbol_pos + 1));
                    } else {
                        prob = 1;
                    }
                    std::vector<std::string> classPairConfig;
                    classPairConfig.push_back(group1Name);
                    classPairConfig.push_back(group2Name);
                    // Sort classPairConfig
                    std::sort(classPairConfig.begin(), classPairConfig.end());

                    if (classPairConfig == classPair) {
                        model->updateLinkedContactProb(prob, k);
                    }
                }
            }
        }
        for (auto y: model->getModelGroup()) {
            std::cout << y << ' ';
        }
        std::cout << "\n";
        for (auto linked: model->getLinkedModels()) {
            for (auto x: linked.lock()->getModelGroup()) {
                std::cout << x << ' ';
            }
        }
        std::cout << "\n";
        for (auto linkedProb: model->getLinkedContactProb()) {
            std::cout << linkedProb << ' ';
        }
        std::cout << "\n";

    }
}
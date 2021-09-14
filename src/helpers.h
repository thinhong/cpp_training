//
// Created by thinh on 08/09/2021.
//

#ifndef MAIN_CPP_HELPERS_H
#define MAIN_CPP_HELPERS_H

#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include "Model.h"

std::vector<std::string> extractCompNames(std::string element);
void viewModelStructure(std::shared_ptr<Model> model);

#endif //MAIN_CPP_HELPERS_H

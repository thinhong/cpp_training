//
// Created by thinh on 19/02/2021.
//

#include "FileJSON.h"
#include <fstream>
#include <iomanip>
#include "json.hpp"

FileJSON::FileJSON(std::string filePath, std::string fileName, Model *model) {
    this->filePath = filePath;
    this->fileName = fileName;
    this->model = model;
}

void FileJSON::writeFile() {
    // Validate full path
    std::string fullPath;
    if (filePath.back() == '/') {
        fullPath = filePath + fileName;
    } else {
        fullPath = filePath + "/" + fileName;
    }

    // Initialize a jsonObject and file
    nlohmann::json jsonObject;
    std::ofstream myFile(fullPath);
    myFile << std::fixed << std::setprecision(precision);

    if (myFile.is_open()) {
        for (size_t i {}; i < model->getComps().size(); ++i) {
            jsonObject[model->getComps()[i]->getName()] = model->getComps()[i]->getTotal();
        }
        myFile << jsonObject;
        myFile.close();
        std::cout << "Successfully written into file: " << fullPath << std::endl;
    } else {
        std::cout << "Unable to open file" << std::endl;
    }
}
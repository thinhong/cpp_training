//
// Created by thinh on 09/02/2021.
//

#include <fstream>
#include "FileCSV.h"

FileCSV::FileCSV(std::string filePath, std::string fileName) {
    this->filePath = filePath;
    this->fileName = fileName;
};

void FileCSV::setModel(Model* model) {
    this->model = model;
}

void FileCSV::writeFile() {
    std::string fullPath;
    if (filePath.back() == '/') {
        fullPath = filePath + fileName;
    } else {
        fullPath = filePath + "/" + fileName;
    }

    std::ofstream myFile(fullPath);
    if (myFile.is_open()) {
        for (size_t j {}; j < model->getComps().size(); ++j) {
            if (j == model->getComps().size() - 1) {
                myFile << model->getComps()[j]->getName() << "\n";
            } else {
                myFile << model->getComps()[j]->getName() << ",";
            }
        }
        for (size_t i {}; i < 1000; ++i) {
            for (size_t j {}; j < model->getComps().size(); ++j) {
                if (j == model->getComps().size() - 1) {
                    myFile << model->getComps()[j]->getTotal()[i] << "\n";
                } else {
                    myFile << model->getComps()[j]->getTotal()[i] << ",";
                }
            }
        }
        myFile.close();
        std::cout << "Successfully written into file: " << fullPath << std::endl;
    } else {
        std::cout << "Unable to open file" << std::endl;
    }
}
//
// Created by thinh on 19/02/2021.
//

#ifndef MAIN_CPP_FILEJSON_H
#define MAIN_CPP_FILEJSON_H

#include "File.h"

class FileJSON: public File {
private:
    Model* model;
    std::string filePath;
    std::string fileName;
public:
    FileJSON(std::string filePath, std::string fileName, Model* model);
    void writeFile() override;
};


#endif //MAIN_CPP_FILEJSON_H

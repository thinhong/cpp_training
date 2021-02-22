//
// Created by thinh on 09/02/2021.
//

#ifndef MAIN_CPP_FILE_H
#define MAIN_CPP_FILE_H

#include <string>
#include <memory>
#include "Model.h"

class File {
public:
    static inline int precision {12};
    virtual void writeFile() = 0;
};


#endif //MAIN_CPP_FILE_H

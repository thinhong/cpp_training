//
// Created by thinh on 02/04/2021.
//

#ifndef MAIN_CPP_CONTACT_H
#define MAIN_CPP_CONTACT_H

#include <string>
#include <vector>

class Contact {
private:
    std::string contactType;
    std::vector<std::string> contactClasses;
    std::vector<std::string> contactProbs;
public:
    Contact(std::string contactType, std::vector<std::string> contactClasses, std::vector<std::string> contactProbs);
    static inline std::vector<std::string> contactAssumption;
    std::string getContactType();
    std::vector<std::string> getContactClasses();
    std::vector<std::string> getContactProbs();
};


#endif //MAIN_CPP_CONTACT_H

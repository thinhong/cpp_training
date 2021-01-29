#ifndef OOPSIR_1_COMPARTMENT_H
#define OOPSIR_1_COMPARTMENT_H

#include <iostream>
#include <vector>
#include <memory>
#include <string>

class Compartment {
private:
    std::vector<double> value;
    std::string name;
    std::vector<std::shared_ptr<Compartment>> linkedCompartment;
    std::vector<std::shared_ptr<double>> linkedWeight;
    std::vector<bool> isIn;
    std::vector<std::shared_ptr<Compartment>> extraParam;
public:
    explicit Compartment(std::string name, size_t size, double initVal) {
        value.resize(size);
        value[0] = initVal;
        this->name = name;
    };
    Compartment() = default;

    // Getters
    std::vector<double> getValue() {return value;};
    std::string getName() {return name;};
    std::vector<std::shared_ptr<Compartment>> getExtraParam() {return extraParam;};

    // Setters
    void setLinkedCompartment(std::shared_ptr<Compartment>& newLinkCompartment);
    void setLinkedWeight(std::shared_ptr<double>& newLinkWeight);
    void setIsIn(bool isInVal);
    void setExtraParam(std::shared_ptr<Compartment>& newExtraParam);

    // Method for update value in each iteration
    void updateValue(long iter);
};



#endif //OOPSIR_1_COMPARTMENT_H

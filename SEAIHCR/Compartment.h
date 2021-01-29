#ifndef SEAIHCR_COMPARTMENT_H
#define SEAIHCR_COMPARTMENT_H

#include <iostream>
#include <vector>
#include <memory>
#include <string>

class Compartment {
private:
    std::vector<double> value;
    std::string name;
    std::vector<std::weak_ptr<Compartment>> linkedCompartment;
    std::vector<std::weak_ptr<double>> linkedWeight;
    std::vector<bool> isIn;
    std::vector<bool> extraParam;
public:
    explicit Compartment(std::string name, size_t size, double initVal) {
        value.resize(size);
        value[0] = initVal;
        this->name = name;
    };
    Compartment() = default;
    ~Compartment() {
        std::cout << name << " destructor called." << std::endl;
    }

    // Getters
    std::vector<double> getValue() {return value;};
    std::string getName() {return name;};
    std::vector<bool> getExtraParam() {return extraParam;};

    // Setters
    void setLinkedCompartment(std::shared_ptr<Compartment>& newLinkCompartment);
    void setLinkedWeight(std::shared_ptr<double>& newLinkWeight);
    void setIsIn(bool isInVal);
    void setExtraParam(bool newExtraParam);

    // Method for update value in each iteration
    void updateValue(long iter, std::vector<std::shared_ptr<Compartment>>& listComps);

    // Update weight
    double updateWeight(std::vector<std::shared_ptr<Compartment>>& otherCompartments, long iter);

};


#endif //SEAIHCR_COMPARTMENT_H

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
    std::vector<std::shared_ptr<double>> linkedWeight;
    std::vector<bool> isIn;
public:
    explicit Compartment(std::string name, size_t size, double initVal) {
        value.resize(size);
        value[0] = initVal;
        this->name = name;
    };
    Compartment() = delete;

    Compartment(const Compartment& comp) = delete;

    ~Compartment() {
        std::cout << name << " destructor called." << std::endl;
    }

    // Getters
    std::vector<double> getValue() {return value;};
    std::string getName() {return name;};

    // Setters (add)
    void addLinkedCompartment(std::shared_ptr<Compartment>& newLinkCompartment);
    void addLinkedWeight(std::shared_ptr<double>& newLinkWeight);
    void addIsIn(bool isInputComp);

    // Method for update value in each iteration
    void updateValue(long iter);

};


#endif //SEAIHCR_COMPARTMENT_H

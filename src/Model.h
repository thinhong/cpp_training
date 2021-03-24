#ifndef SEAIHCR_MODEL_H
#define SEAIHCR_MODEL_H

#include <vector>
#include <memory>
#include <stack>
#include "Compartment.h"

class Model {
private:
    std::string name;
    std::vector<std::string> infectiousComps;
    double transmissionRate {0};
    // Population size is computed after sortComps in main()
    double populationSize {0};
    std::vector<std::weak_ptr<Model>> linkedLocations;
    std::vector<double> locationInteraction;
    double selfInteraction;

    std::vector<std::shared_ptr<Compartment>> comps;
public:
    explicit Model(std::string name, double transmissionRate, std::vector<std::string> infectiousComps);
    ~Model() {
//        std::cout << name << " model destructor called." << std::endl;
    }
    std::string getName();
    std::vector<std::shared_ptr<Compartment>> getComps();
    void calcPopulationSize();
    double getPopulationSize();
    std::vector<std::string> getInfectiousComps();
    double getTransmissionRate();
    std::vector<std::weak_ptr<Model>> getLinkedLocation();
    std::vector<double> getLocationInteraction();
    double getSelfInteraction();

    // Interaction among locations
    void addLinkedLocation(std::weak_ptr<Model> linkedLocation);
    void addLocationInteraction(std::vector<double> locationInteraction);

    // Add compartment to model using JSON config file
    void addCompsFromConfig(std::vector<std::shared_ptr<Compartment>>& comps);
    void addCompsAndConnect(std::shared_ptr<Compartment>& A, std::shared_ptr<Compartment>& B, double weight);
    int getIndex(std::shared_ptr<Compartment> comp);

    // Use depth-first-search algorithm to detect cycle https://www.geeksforgeeks.org/detect-cycle-in-a-graph/
    bool checkCycleHelper(size_t i, std::vector<bool>& visited, std::vector<bool>& recursiveStack);
    void checkCycle();
    // Then we sort the compartments using topological sorting algorithm https://www.geeksforgeeks.org/topological-sorting/
    void sortCompsHelper(size_t i, std::vector<bool>& visited, std::stack<std::shared_ptr<Compartment>>& stack);
    void sortComps();

    double calcForceInfection(size_t iter);

    /**
     * Update subCompartmentValues and total for each compartments in the model
     * @param iter
     */
    void update(long iter);


};


#endif //SEAIHCR_MODEL_H

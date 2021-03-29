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
    std::vector<std::string> transitionFlow;
    double transmissionRate {0};
    // Population size is computed after sortComps in main()
    double populationSize {0};
    std::vector<std::weak_ptr<Model>> linkedLocations;
    std::vector<double> linkedContactProb;
    double selfContactProb {1};

    std::vector<std::shared_ptr<Compartment>> comps;
public:
    explicit Model(std::string name, double transmissionRate, std::vector<std::string> infectiousComps, std::vector<std::string> transitionFlow);
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
    std::vector<std::string> getTransitionFlow();
    /**
     * Compartments of a Model object are stored as pointer in vector <b>comps</b>, but we normally identify compartment
     * by name, this function take the compartment name as input and return the address of that compartment pointer
     * @param compName: a string that matches name of the compartment of interest
     * @return a weak pointer of that compartment
     */
    std::weak_ptr<Compartment> getAddressFromName(std::string compName);

    // Interaction among locations
    void setSelfContactProb(double selfContactProb);
    void addLinkedContactProb(double linkedContactProb);
    void addLinkedLocation(std::weak_ptr<Model> linkedLocation);

    // Add compartment to model using JSON config file
    void addCompsFromConfig(std::vector<std::shared_ptr<Compartment>>& comps);
    void connectComp(std::string transitionSymbol, std::string weightSymbol);
    void addCompsAndConnect(std::shared_ptr<Compartment>& A, std::shared_ptr<Compartment>& B, double weight);

    /**
     * Return the index of a compartment in vector <b>comps</b>
     * @param comp: shared pointer of the compartment of interest
     * @return the index of that compartment
     */
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

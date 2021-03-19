#ifndef SEAIHCR_MODEL_H
#define SEAIHCR_MODEL_H

#include <vector>
#include <memory>
#include <stack>
#include "Compartment.h"

class Model {
private:
    std::vector<std::shared_ptr<Compartment>> comps;
    double populationSize {0};
public:
    Model() = default;
    std::vector<std::shared_ptr<Compartment>> getComps();
    void calcPopulationSize();
    double getPopulationSize();

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

    /**
     * Update subCompartmentValues and total for each compartments in the model
     * @param iter
     */
    void update(long iter);
};


#endif //SEAIHCR_MODEL_H

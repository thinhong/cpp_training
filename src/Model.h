#ifndef SEAIHCR_MODEL_H
#define SEAIHCR_MODEL_H

#include <vector>
#include <memory>
#include <stack>
#include "Compartment.h"

class Model {
private:
    std::vector<std::shared_ptr<Compartment>> comps;
    bool isCycle {false};
public:
    Model() = default;
    std::vector<std::shared_ptr<Compartment>> getComps();
    // Add compartment to model when using JSON config file
    void addFromConfig(std::vector<std::shared_ptr<Compartment>>& comps);
    // When add and connect compartment, define weight if weight != 1, otherwise use default weight = 1
    void addCompsAndConnect(std::shared_ptr<Compartment>& A, std::shared_ptr<Compartment>& B, double weight = 1);

    // Use depth-first-search algorithm to detect cycle https://www.geeksforgeeks.org/detect-cycle-in-a-graph/
    int getIndex(std::shared_ptr<Compartment>&& comp);
    bool checkCycleHelper(size_t i, std::vector<bool>& visited, std::vector<bool>& recursiveStack);
    void checkCycle();
    // Only for debugging
    bool getCycle() {return isCycle;};

    // Only sort compartments after ensuring that no isCycle exists
    // Use topological sorting algorithm to sort https://www.geeksforgeeks.org/topological-sorting/
    void sortCompsHelper(size_t i, std::vector<bool>& visited, std::stack<std::shared_ptr<Compartment>>& stack);
    void sortComps();
    void update(long iter);
};


#endif //SEAIHCR_MODEL_H

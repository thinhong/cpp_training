#ifndef SEAIHCR_MODEL_H
#define SEAIHCR_MODEL_H

#include <vector>
#include <memory>
#include <stack>
#include "Compartment.h"

class Model {
private:
    std::vector<std::shared_ptr<Compartment>> comps;
public:
    Model() = default;
    std::vector<std::shared_ptr<Compartment>> getComps();
    // Add compartment to model when using JSON config file
    void addCompsFromConfig(std::vector<std::shared_ptr<Compartment>>& comps);
    int getIndex(std::shared_ptr<Compartment> comp);

    // Use depth-first-search algorithm to detect cycle https://www.geeksforgeeks.org/detect-cycle-in-a-graph/
    bool checkCycleHelper(size_t i, std::vector<bool>& visited, std::vector<bool>& recursiveStack);
    void checkCycle();

    // Only sort compartments after ensuring that no isCycle exists
    // Use topological sorting algorithm to sort https://www.geeksforgeeks.org/topological-sorting/
    void sortCompsHelper(size_t i, std::vector<bool>& visited, std::stack<std::shared_ptr<Compartment>>& stack);
    void sortComps();

    void update(long iter);
};


#endif //SEAIHCR_MODEL_H

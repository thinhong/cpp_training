#ifndef SEAIHCR_MODEL_H
#define SEAIHCR_MODEL_H

#include <vector>
#include <memory>
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

    // Functions to detect isCycle in directed graph (depth-first-search)
    // Source: https://www.geeksforgeeks.org/detect-cycle-in-a-graph/
    int getIndex(std::shared_ptr<Compartment>&& comp);
    bool DFS_helper(size_t i, std::vector<bool>& visited, std::vector<bool>& recursiveStack);
    void DFS();
    // Only for debugging
    bool getCycle() {return isCycle;};

    // Only sort compartments after ensuring that no isCycle exists
    void sortComps();
    void update(long iter);
};


#endif //SEAIHCR_MODEL_H

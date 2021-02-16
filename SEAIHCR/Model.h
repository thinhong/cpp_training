#ifndef SEAIHCR_MODEL_H
#define SEAIHCR_MODEL_H

#include <vector>
#include <memory>
#include "Compartment.h"

class Model {
private:
    std::vector<std::shared_ptr<Compartment>> comps;
public:
    Model() = default;
    std::vector<std::shared_ptr<Compartment>> getComps() {return comps;};
    void addCompsAndConnect(std::shared_ptr<Compartment>& A, std::shared_ptr<Compartment>& B);
    void sortComps();
    void update(long iter);
};


#endif //SEAIHCR_MODEL_H

#include <iostream>
#include <vector>
#include <memory>

class Compartment {
public:
    std::vector<double> total;
    std::vector<double> current_values;
    double out_value_nextIter {0};
    std::vector<double> dist;
    std::vector<std::weak_ptr<Compartment>> linkedCompartment;
    std::vector<bool> isIn;
    void updateValue(long iter);
};

void Compartment::updateValue(long iter) {
    // For compartments other than S (current_values vector has many values)
    if (current_values.size() > 1) {
        // Going backward from current_values[n] -> current_values[1]
        for (size_t i {current_values.size() - 1}; i > 0; --i) {
            out_value_nextIter += current_values[i] * dist[i];
            current_values[i] = current_values[i - 1] * (1 - dist[i - 1]);
        }
        // All of current_values[0] will go to current_values[1] so initialize current_values[0] = 0
        current_values[0] = 0;
        // Loop over all linkedCompartment, find the linkedCompartment with isIn == true
        // Let current_values[0] = out_value_nextIter of that linkedCompartment
        for (size_t j {0}; j < linkedCompartment.size(); ++j) {
            if (isIn[j]) {
                current_values[0] = linkedCompartment[j].lock()->out_value_nextIter;
            }
        }
    } else if (current_values.size() == 1) { // For S compartment (S only has 1 value)
        out_value_nextIter = current_values[0] * dist[0];
        current_values[0] = current_values[0] * (1 - dist[0]);
    }

    // Finally sum up the current values of this iteration to total
    for (auto value: current_values) {
        total[iter] += value;
    }
}

int main() {
    auto S = std::make_shared<Compartment>();
    auto E = std::make_shared<Compartment>();
    auto A = std::make_shared<Compartment>();

    S->current_values = {10000};
    S->total.resize(100);
    S->dist = {0.2};
    S->linkedCompartment = {E};
    S->isIn = {false};

    E->current_values = {500, 600, 700};
    E->total.resize(100);
    E->dist = {0, 0.8, 1};
    E->linkedCompartment = {S, A};
    E->isIn = {true, false};

    A->current_values = {5, 6, 7, 8};
    A->total.resize(100);
    A->dist = {0, 0.3, 0.7, 1};
    A->linkedCompartment = {E};
    A->isIn = {true};

    for (auto i: S->current_values) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
    for (auto i: E->current_values) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
    for (auto i: A->current_values) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;

    std::vector<std::shared_ptr<Compartment>> model = {S, E, A};
    for (size_t iter {0}; iter < 4; ++iter) {
        for (auto& comp: model) {
            comp->updateValue(iter);
        }

        std::cout << "Iteration " << iter + 1 << std::endl;
        for (auto i: S->current_values) {
            std::cout << i << ' ';
        }
        std::cout << std::endl;
        for (auto i: E->current_values) {
            std::cout << i << ' ';
        }
        std::cout << std::endl;
        for (auto i: A->current_values) {
            std::cout << i << ' ';
        }
        std::cout << std::endl;
    }

}
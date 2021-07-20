#ifndef SEAIHCR_MODEL_H
#define SEAIHCR_MODEL_H

#include <vector>
#include <memory>
#include <stack>
#include "Compartment.h"
#include "Contact.h"
#include "muParser/muParser.h"

class Model {
private:
    std::vector<std::string> modelName;
    /**
     * Contains all compartments of this model
     */
    std::vector<std::shared_ptr<Compartment>> comps;

    std::vector<std::string> paramNames;
    std::vector<double> paramValues;

    // Extract these values from comps vector
    std::vector<std::string> allCompNames;
    std::vector<double> allCompValues;

    /**
     * Contains weak pointers to all models, including itself, with corresponding contact probability stored at the
     * same indices in vector linkedContactRates
     */
    std::vector<std::weak_ptr<Model>> linkedModels;

    /**
     * Contain the contact probabilities that corresponds to the same indices in linkedModels
     */
    std::vector<double> linkedContactRates;
public:
    // Model structure and infectious compartment are the same for all models for a disease
    static inline std::vector<std::string> modelStructure;
    static inline std::vector<std::string> infectiousComps;
    Model(std::vector<std::string> modelGroup, std::vector<std::string>& paramNames, std::vector<double>& paramValues);
    ~Model() {
//        std::string name;
//        for (auto group: modelName) {
//            name += group;
//        }
//        std::cout << name << " model destructor called." << std::endl;
    }
    std::vector<std::string> getModelGroup();
    std::vector<std::shared_ptr<Compartment>> getComps();

    std::vector<double> getLinkedContactRates() {return linkedContactRates;};


    std::vector<std::weak_ptr<Model>> getLinkedModels();

    /**
     * Compartments of a Model object are stored as pointer in vector <b>comps</b>, but we normally identify compartment
     * by name, this function take the compartment name as input and return the address of that compartment pointer
     * @param compName: a string that matches name of the compartment of interest
     * @return a weak pointer of that compartment
     */
    std::weak_ptr<Compartment> getAddressFromName(std::string compName);

    /**
     * Ensure that the order of elements in model group follows instruction from contact assumption
     * @param allContacts: a vector contains all Contact objects
     */
    void sortModelGroupByAssumption(std::vector<std::shared_ptr<Contact>> allContacts);

    // Interaction among locations
    void addNewLinkedContactRate(double linkedContactRate);
    void updateLinkedContactRate(double linkedContactRateToUpdate, size_t index);
    void addLinkedModels(std::vector<std::weak_ptr<Model>> allModels);

    // Add compartment to model using JSON config file
    void addCompsFromConfig(std::vector<std::shared_ptr<Compartment>>& comps);
    void connectComp();
    void addCompsAndConnect(std::shared_ptr<Compartment>& A, std::shared_ptr<Compartment>& B, double weight);

    /**
     * Return the index of a compartment in vector <b>comps</b>
     * @param comp: shared pointer of the compartment of interest
     * @return the index of that compartment
     */
    int getIndex(std::shared_ptr<Compartment> comp);

    int getIndexLinkedModel(std::vector<std::string> modelGroup);

    // Functions to check cycle and sort compartments to the correct order
    // Use depth-first-search algorithm to detect cycle https://www.geeksforgeeks.org/detect-cycle-in-a-graph/
    bool checkCycleHelper(size_t i, std::vector<bool>& visited, std::vector<bool>& recursiveStack);
    void checkCycle();
    // Then we sort the compartments using topological sorting algorithm https://www.geeksforgeeks.org/topological-sorting/
    void sortCompsHelper(size_t i, std::vector<bool>& visited, std::stack<std::shared_ptr<Compartment>>& stack);
    /**
     * <b>sortComps()</b> comprises of many helper functions (<b>checkCycleHelper()</b>, <b>checkCycle()</b> and
     * <b>sortCompsHelper()</b>), it will first make sure that there is no cycle in the model, then perform topological
     * sorting algorithm
     */
    void sortComps();

    /**
     * Update subCompartmentValues and total for each compartments in the model
     * @param iter
     */
    void update(long iter);

    /**
     * Get all compartment names from the comps vector and also initial values of allCompValues
     */
    void initAllComps();

    /**
     * Helper function to update allCompValues after each iteration
     */
    void updateAllCompValues(long iter);

};


#endif //SEAIHCR_MODEL_H

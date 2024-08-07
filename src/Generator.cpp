#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include "Generator.hpp"

using std::unordered_map, std::unordered_set, std::vector, std::array;

std::random_device rd;
std::mt19937 gen(rd());

energyRange::energyRange(int min, int max): minimum{min}, maximum{max} {};

fitnessProb::fitnessProb(long double fit, long double w) : fitness{fit}, weight{w} {};
fitnessProb::fitnessProb() : fitness{}, weight{} {};

// THESE WERE DEPRECATED IN FAVOUR OF USING THE REJECTION METHOD, FOR NOW THAT FUNCTION IS FOUND IN NETWORK.CPP BUT IT SHOULD BE MOVED HERE IN THE FUTURE

// void generateBoseEinstein(energyRange range, long double beta_constant, vector<fitnessProb>* fitness_distribution) {
//     /* 
//     Function that generates an energy distribution based on the "Bose-Einstein Condensation in Complex Networks" article
//     -INPUT:
//      range - minimum and maximum energy
//      beta_constant - physical variable beta = 1/T, where T is temperature (unit K), remains constant during network generation
//      fitness_distribution - vector pointer to the network's fitness distribution
//      */
//     int theta{1}; // the theta constant
//     long double increment{(long double)(range.maximum - range.minimum) / 1000000};
//     long double energy{range.minimum + increment};
//     int max{range.maximum};

//     for (int i=0; i<999999; ++i){
//         long double weight = ((theta +1) * std::pow(energy, theta +1)) /std::pow(max, theta +1);
//         long double fitness{std::exp( -(beta_constant * energy) )};
//         (*fitness_distribution).emplace_back(fitnessProb(fitness, weight));
//         energy += increment;
//     }
// }

// long double chooseFitness(vector<fitnessProb>* distribution) {
//     std::uniform_real_distribution<> area(0.0, 1.0);
//     long double variate = area(gen);
//     int size = distribution->size();
//     int index = std::floor(variate*size);
//     long double max_weight = (*(std::max_element(distribution->begin(), distribution->end(), [](const fitnessProb& prob1, const fitnessProb& prob2){ return (prob1.weight < prob2.weight);}))).weight;
//     auto& random_fit = (*distribution)[index];
//     // rejection method because linear searching was 99% of the process
//     while (variate*size -index >= random_fit.weight / max_weight){
//         variate = area(gen);
//         index = std::floor(variate*size);
//         random_fit = (*distribution)[index];
//     }
    
//     return random_fit.fitness;
// }

WeightLeaf::WeightLeaf(int n, long double w) : name{n}, weight{w} {};


LeafResult::LeafResult() : chosen_leaf{nullptr}, leaf_index{} {};

LeafResult::LeafResult(WeightLeaf* chosen_l, int leaf_i) : chosen_leaf{chosen_l}, leaf_index{leaf_i} {};

bool LeafCompare::operator()(const LeafResult* leaf1, const LeafResult* leaf2) const {
    return ( (leaf1->chosen_leaf != leaf2->chosen_leaf) && ( (leaf1->leaf_index > leaf2->leaf_index) ) );
}


// auto leaf_cmp = [](const WeightLeaf* leaf1, const WeightLeaf* leaf2) {return (leaf1->weight < leaf2->weight);};

WeightBranch::WeightBranch(int range_n) : is_level_one{false}, is_root{false}, staged_changes{false}, branch_name{range_n}, total_weight{0.0l}, old_weight{0.0l}, leafs{}, branches{} {};

WeightBranch::~WeightBranch() {for (auto& e : leafs) {delete e;}};

void WeightBranch::toggleRoot() {
    is_root = !is_root;
}

bool WeightBranch::checkRoot() {
    return is_root;
}

void WeightBranch::setLevelOne() {
    is_level_one = true;
}

int WeightBranch::getName() {
    return branch_name;
}

// void WeightBranch::setRangeOld() {
//     old_weight_range = std::floor( std::log2(total_weight) ) +1;
// }

void WeightBranch::setWeightOld() {
    old_weight = total_weight;
}

void WeightBranch::setSizeOld() {
    old_size = this->getSize();
}

// int WeightBranch::getRange() {
//     return old_weight_range;   
// }

long double WeightBranch::getWeight() {
    return total_weight;
}

int WeightBranch::getSizeOld() {
    return old_size;
}

void WeightBranch::addWeight(long double weight_delta) {
    if (!staged_changes){
        // setRangeOld();
        setWeightOld();
        setSizeOld();
        staged_changes = true;
    }
    total_weight += weight_delta;
}

long double WeightBranch::getWeightOld() {
    return old_weight;
}

void WeightBranch::setChangesFalse() {
    staged_changes = false;
}

void WeightBranch::extractElement(const WeightLeaf* leaf_to_rm, int index) {
    if(!is_level_one){
        std::cerr << "Invalid argument for non-level-one branch!\n"; // this should never happen, but just in case
    } else{
        if (!staged_changes){
            setSizeOld();
            // setRangeOld();
            setWeightOld();
            staged_changes = true;
        }
        total_weight -= leaf_to_rm->weight;
        if (this->getSize() > 1){
            std::iter_swap(leafs.begin() +index, leafs.end()-1);
            this->leafs.pop_back();
        } else{
            this->leafs.clear();
        }
        // leafs.erase(std::remove(leafs.begin(), leafs.end(), leaf_to_rm), leafs.end()); // this ended up being slightly faster even though it should do the same thing as the first to below it
        // std::erase_if(leafs, [leaf_to_rm](const auto& leaf) {return (leaf == leaf_to_rm);} );
        // leafs.erase(std::remove_if(leafs.begin(), leafs.end(), [leaf_to_rm](auto& leaf) {return (leaf == leaf_to_rm);}), leafs.end());
        // leafs.erase(leaf_to_rm);
    }
}

void WeightBranch::extractElement(int range) {
    if(is_level_one){
        std::cerr << "Invalid argument for level-one branch!\n"; // this should never happen, but just in case
    } else{
        if (!branches.contains(range)){
            std::cout << "Not good!\n";
            std::cout << "Looked for: " << range << '\n';
            std::cout << "In: " << getName() << '\n';
            std::cout << "Children: " << getSize() << '\n';
        }
        if (!staged_changes){
            setSizeOld();
            // setRangeOld();
            setWeightOld();
            staged_changes = true;
        }
        total_weight -= (branches[range])->getWeightOld();
        branches.erase(range);
    }
}

void WeightBranch::insertElement(WeightLeaf* new_leaf) {
    if(!is_level_one){
        std::cerr << "Invalid argument for non-level-one branch!\n"; // this should never happen, but just in case
    } else{
        if (!staged_changes){
            setSizeOld();
            // setRangeOld();
            setWeightOld();
            staged_changes = true;
        }
        leafs.push_back(new_leaf);
        total_weight += new_leaf->weight;
    }
}

void WeightBranch::insertElement(WeightBranch* new_branch) {
    if(is_level_one){
        std::cerr << "Invalid argument for level-one branch!\n"; // this should never happen, but just in case
    } else{
        if (!staged_changes){
            setSizeOld();
            // setRangeOld();
            setWeightOld();
            staged_changes = true;
        }
        branches.insert({new_branch->getName(), new_branch});
        total_weight += new_branch->getWeight();
    }
}


LeafResult* WeightBranch::isLevelOne(){
    long double max_weight = std::pow(2, this->getName());
    auto children = getSize();
    std::uniform_int_distribution<> area(0, children-1);
    std::uniform_real_distribution<> weight(0.0, max_weight);
    int index = area(gen);
    long double variate = weight(gen);
    auto random_leaf = leafs[index];
    
    while (variate >= (random_leaf)->weight){
        variate = weight(gen);
        index = area(gen);
        random_leaf = leafs[index];
    }
    LeafResult* result = new LeafResult(random_leaf, index);
    return result;
}

LeafResult* WeightBranch::isNotLvlOne(){
    long double max_weight = std::pow(2, this->getName());
    
    auto children = getSize();
    std::uniform_int_distribution<> area(0, children-1);
    std::uniform_real_distribution<> weight(0.0, max_weight);
    int index = area(gen);
    long double variate = weight(gen);
    auto random_branch = std::next(branches.begin(), index);

    while(variate >= (*random_branch).second->getWeight() ){
        variate = weight(gen);
        index = area(gen);
        random_branch = std::next(branches.begin(), index);
    }
    return (*random_branch).second->recurRejection();
}

LeafResult* WeightBranch::recurRejection() {
    if (is_level_one) {
        return isLevelOne();
    } else{
        return isNotLvlOne();
    }
}

int WeightBranch::getSize() {
    return (is_level_one ? leafs.size() : branches.size());
}

WeightBranch* findRange(int range_number, int level, unordered_map<int, unordered_map<int, WeightBranch*>>* level_table) {
    if (!level_table->contains(level)){
        // No ranges and this level doesn't exist yet! create new level and range
        WeightBranch* new_branch = new WeightBranch(range_number);
        ((*level_table)[level]).insert({new_branch->getName(), new_branch});
        return new_branch;
    } else {
        if ( (*level_table)[level].contains(range_number) ){
            return (*level_table)[level][range_number];
        }
        // no correct range found on level! create a new one
        WeightBranch* new_branch = new WeightBranch(range_number);
        ((*level_table)[level]).insert({new_branch->getName(), new_branch});
        return new_branch;
    }
}


WeightTable::WeightTable(long double w, long double r) : total_weight{w}, roots{r} {};
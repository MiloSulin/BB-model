#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include "../include/Generator.hpp"

using std::unordered_map, std::unordered_set, std::vector;

std::random_device rd;
std::mt19937 gen(rd());



WeightLeaf::WeightLeaf(int n, long double w) : name{n}, weight{w} {};


LeafResult::LeafResult() : chosen_leaf{nullptr}, leaf_index{} {};

LeafResult::LeafResult(WeightLeaf* chosen_l, int leaf_i) : chosen_leaf{chosen_l}, leaf_index{leaf_i} {};

bool LeafCompare::operator()(const LeafResult& leaf1, const LeafResult& leaf2) const {
    return ( (leaf1.chosen_leaf != leaf2.chosen_leaf) && ( (leaf1.leaf_index > leaf2.leaf_index) ) );
}


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

bool WeightBranch::checkChanges() {
    return staged_changes;
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
            setWeightOld();
            staged_changes = true;
        }
        branches.insert({new_branch->getName(), new_branch});
        total_weight += new_branch->getWeight();
    }
}


LeafResult WeightBranch::isLevelOne(){
    long double max_weight = std::pow(2, this->getName());
    auto children = getSize();
    std::uniform_real_distribution<> weight(0.0, 1.0);
    long double variate = weight(gen);
    int index = std::floor(variate * children);
    auto random_leaf = leafs[index];
    
    while (variate*children - index >= (random_leaf)->weight / max_weight){
        variate = weight(gen);
        index = std::floor(variate * children);
        random_leaf = leafs[index];
    }
    LeafResult result = LeafResult(random_leaf, index);
    return result;
}

LeafResult WeightBranch::isNotLvlOne(){
    long double max_weight = std::pow(2, this->getName());
    auto children = getSize();
    std::uniform_real_distribution<> weight(0.0, 1.0);
    long double variate = weight(gen);
    int index = std::floor(variate * children);
    auto random_branch = std::next(branches.begin(), index);

    while(variate*children - index >= (*random_branch).second->getWeight() / max_weight ){
        variate = weight(gen);
        index = std::floor(variate * children);
        random_branch = std::next(branches.begin(), index);
    }
    return (*random_branch).second->recurRejection();
}

LeafResult WeightBranch::recurRejection() {
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

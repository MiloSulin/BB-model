#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <vector>
#include <array>
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


void generateBoseEinstein(energyRange range, long double beta_constant, vector<fitnessProb>* fitness_distribution) {
    /* 
    Function that generates an energy distribution based on the "Bose-Einstein Condensation in Complex Networks" article
    -INPUT:
     range - minimum and maximum energy
     beta_constant - physical variable beta = 1/T, where T is temperature (unit K), remains constant during network generation
     fitness_distribution - vector pointer to the network's fitness distribution
     */
    int theta{1}; // the theta constant
    long double increment{(long double)(range.maximum - range.minimum) / 1000000};
    long double energy{range.minimum + increment};
    int max{range.maximum};
    long double total_weight{0};

    for (int i=0; i<999999; ++i){
        total_weight += ((theta +1) * std::pow(energy, theta +1)) /std::pow(max, theta +1);
        long double fitness{std::exp( -(beta_constant * energy) )};
        (*fitness_distribution).push_back(fitnessProb(fitness, total_weight));
        energy += increment;
    }
}

long double chooseFitness(vector<fitnessProb>* distribution) {
    long double max_weight{( (*distribution).back() ).weight};
    std::uniform_real_distribution<> area(0.0, max_weight);
    long double prob_u = area(gen);
    long double lower_b{0.0l};
    for (const auto& p : *distribution){
        if(lower_b < prob_u && prob_u <= p.weight){
            return p.fitness;
        }
        lower_b = p.weight;
    }
    std::cerr << "Something went wrong when generating a random fitness! Variate U wasn't within range.\n";
    return 1.0;
}

WeightLeaf::WeightLeaf(int n, long double w) : name{n}, weight{w} {};

WeightBranch::WeightBranch(int range_n) : is_level_one{false}, is_root{false}, staged_changes{false}, branch_name{range_n}, old_weight_range{}, total_weight{0.0l}, old_weight{0.0l}, leafs{}, branches{} {};

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

void WeightBranch::setRangeOld() {
    old_weight_range = std::floor( std::log2(total_weight) ) +1;
}

void WeightBranch::setWeightOld() {
    old_weight = total_weight;
}

int WeightBranch::getRange() {
    return old_weight_range;   
}

long double WeightBranch::getWeight() {
    return total_weight;
}

void WeightBranch::addWeight(long double weight_delta) {
    if (!staged_changes){
        setRangeOld();
        setWeightOld();
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

void WeightBranch::extractElement(WeightLeaf* leaf_to_rm) {
    if(!is_level_one){
        std::cerr << "Invalid argument for non-level-one branch!\n"; // this should never happen, but just in case
    } else{
        if (!staged_changes){
            setRangeOld();
            setWeightOld();
            staged_changes = true;
        }
        total_weight -= leaf_to_rm->weight;
        leafs.erase(leaf_to_rm);
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
            setRangeOld();
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
            setRangeOld();
            setWeightOld();
            staged_changes = true;
        }
        leafs.insert(new_leaf);
        total_weight += new_leaf->weight;
    }
}

void WeightBranch::insertElement(WeightBranch* new_branch) {
    if(is_level_one){
        std::cerr << "Invalid argument for level-one branch!\n"; // this should never happen, but just in case
    } else{
        if (!staged_changes){
            setRangeOld();
            setWeightOld();
            staged_changes = true;
        }
        branches.insert({new_branch->getName(), new_branch});
        total_weight += new_branch->getWeight();
    }
}

WeightLeaf* WeightBranch::recurRejection() {
    std::uniform_real_distribution<> area(0.0, 1.0);
    auto variate = area(gen);
    auto children = this->getSize();
    int index = std::floor(variate*children);
    if (is_level_one) {
        long double max_weight = (*std::max_element(leafs.begin(), leafs.end(), [](const WeightLeaf* leaf1, const WeightLeaf* leaf2) {return leaf1->weight < leaf2->weight; }))->weight; // this monstrosity returns the weight of the heaviest leaf
        auto random_leaf = std::next(leafs.begin(), index);
        while(variate*children - index >= (*random_leaf)->weight /max_weight ){
            variate = area(gen);
            index = std::floor(variate * children);
            random_leaf = std::next(leafs.begin(), index);
        }
        return (*random_leaf);
    } else{
        auto max_weight = (*(std::max_element(branches.begin(), branches.end(), [](const std::pair<int, WeightBranch*>& branch1, const std::pair<int, WeightBranch*>& branch2 ){ 
            return branch1.second->getWeight() < branch2.second->getWeight(); } // this bigger monstrosity returns the weight of heaviest branch the current branch points to 
            ))).second->getWeight();

        auto random_branch = std::next(branches.begin(), index);
        while(variate*children - index >= (*random_branch).second->getWeight() /max_weight ){
            variate = area(gen);
            index = std::floor(variate * children);
            random_branch = std::next(branches.begin(), index);
        }
        return (*random_branch).second->recurRejection();
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
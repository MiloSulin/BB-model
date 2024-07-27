#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <vector>
#include <array>
#include <thread>
#include <cmath>
#include <random>
#include <chrono>
#include "Generator.hpp"

using namespace std::chrono;
using std::unordered_map, std::unordered_set, std::vector, std::array;

std::random_device rd;
std::mt19937 gen(rd());

energyRange::energyRange(int min, int max): minimum{min}, maximum{max} {};

fitnessProb::fitnessProb(double fit, double w) : fitness{fit}, weight{w} {};
fitnessProb::fitnessProb() : fitness{}, weight{} {};


void generateBoseEinstein(energyRange range, double beta_constant, vector<fitnessProb>* fitness_distribution) {
    /* 
    Function that generates an energy distribution based on the "Bose-Einstein Condensation in Complex Networks" article
    -INPUT:
     range - minimum and maximum energy
     beta_constant - physical variable beta = 1/T, where T is temperature (unit K), remains constant during network generation
     fitness_distribution - vector pointer to the network's fitness distribution
     */
    int theta{1}; // the theta constant
    double increment{double(range.maximum - range.minimum) / 1000000};
    double energy{range.minimum + increment};
    int max{range.maximum};
    double total_weight{0};

    for (int i=0; i<999999; ++i){
        total_weight += ((theta +1) * std::pow(energy, theta +1)) /std::pow(max, theta +1);
        double fitness{std::exp( -(beta_constant * energy) )};
        (*fitness_distribution).push_back(fitnessProb(fitness, total_weight));
        energy += increment;
    }
}

double chooseFitness(vector<fitnessProb>* distribution) {
    double max_weight{( (*distribution).back() ).weight};
    std::uniform_real_distribution<> area(0.0, max_weight);
    double prob_u = area(gen);
    double lower_b{0.0};
    for (const auto& p : *distribution){
        if(lower_b < prob_u && prob_u <= p.weight){
            return p.fitness;
        }
        lower_b = p.weight;
    }
    std::cerr << "Something went wrong when generating a random fitness! Variate U wasn't within range.\n";
    return 1.0;
}


WeightLeaf::WeightLeaf(int n, double w) : name{n}, weight{w} {};


WeightBranch::WeightBranch(int range_name) : is_level_one{false}, is_root{false}, range_moniker{range_name}, total_weight{}, leafs{}, branches{} {};

WeightBranch::~WeightBranch() {for (auto& e : leafs) {delete e;}}

void WeightBranch::toggleRoot() {
    is_root = !is_root;
}

void WeightBranch::setLevelOne() {
    is_level_one = true;
}

int WeightBranch::getRange() {
    return range_moniker;
}

double WeightBranch::getWeight() {
    return total_weight;
}

void WeightBranch::insertElement(WeightLeaf* new_leaf) {
    if(!is_level_one){
        std::cerr << "Invalid argument for non-level-one branch!\n"; // this should never happen, but just in case
    } else{
        leafs.insert(new_leaf);
        total_weight += new_leaf->weight;
    }
}

void WeightBranch::insertElement(WeightBranch* new_branch) {
    if(is_level_one){
        std::cerr << "Invalid argument for level-one branch!\n"; // this should never happen, but just in case
    } else{
        branches.insert({new_branch->getRange(), new_branch});
        total_weight += new_branch->getWeight();
    }
}

int WeightBranch::getSize() {
    return (is_level_one ? leafs.size() : branches.size());
}

WeightBranch* findRange(int range_number, int level, unordered_map<int, unordered_set<WeightBranch*>>* level_table) {
    if (!level_table->contains(level)){
        // No ranges and this level doesn't exist yet! create new level and range
        WeightBranch* new_branch = new WeightBranch(range_number);
        ((*level_table)[level]).insert(new_branch);
        return new_branch;
    } else {
        for (auto& range : (*level_table)[level]){
            if (range->getRange() == range_number){
                return range;
            }
        }
        // no correct range found on level! create a new one
        WeightBranch* new_branch = new WeightBranch(range_number);
        ((*level_table)[level]).insert(new_branch);
        return new_branch;
    }
}


WeightTable::WeightTable(double w, double r) : total_weight{w}, roots{r} {};
#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <array>
#include <cmath>
#include <thread>
#include <random>

using std::unordered_map, std::unordered_set, std::vector, std::array;

extern std::random_device rd;
extern std::mt19937 gen;

struct energyRange {
    energyRange(int, int);
    // minimum and maximum (excluded) bounds of the energy as two integers 
    int minimum;
    int maximum;
};

struct fitnessProb {
    fitnessProb();
    fitnessProb(double, double);
    // contains the weight for the specified fitness value
    double fitness;
    double weight;
};


void generateBoseEinstein(energyRange, double, vector<fitnessProb>*);

double chooseFitness(vector<fitnessProb>*);


// objects required for constructing the full vertex distribution according to the alogrithm presented in "Dynamic Generation of Discrete Random Variates" by Matias et al.

struct WeightLeaf {
    // level 0 node in the weight distribution of vertices
    WeightLeaf(int, double);
    int name; // name of the vertex this leaf represents
    double weight; // weight of the vertex, in the case of Bianconi-Barabasi model this is fitness multiplied by degree
};

class WeightBranch {
    public:
        WeightBranch(int);
        ~WeightBranch();
        void toggleRoot(); // toggles the boolean value denoting whether or not this branch is a root or not
        double getWeight();
        int getRange();
        void setLevelOne();
        void insertElement(WeightLeaf*);
        void insertElement(WeightBranch*);
        int getSize();
    private:
        bool is_level_one;
        bool is_root;

        int range_moniker; // integer value x that denotes the max value of the range (2^(x-1), 2^x) this branch represents
        double total_weight; // total weight of this branch
        unordered_set<WeightLeaf*> leafs; // if the branch is level one it will contain a set of leaves (level 0 nodes)
        unordered_map<int, WeightBranch*> branches; // if the branch is not level one it will split into a branch/branches
};

struct WeightTable {
    WeightTable(double, double);
    double total_weight; // total weight in this table
    double roots; // sum of all the ranges' upper bound
};

WeightBranch* findRange(int, int, unordered_map<int, unordered_set<WeightBranch*>>*);

#endif
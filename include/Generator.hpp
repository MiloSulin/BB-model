#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include <unordered_map>
#include <unordered_set>
#include <list>
#include <vector>
#include <array>
#include <cmath>
#include <random>

using std::unordered_map, std::unordered_set, std::vector, std::array, std::list;

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
    fitnessProb(long double, long double);
    // contains the weight for the specified fitness value
    long double fitness;
    long double weight;
};


void generateBoseEinstein(energyRange, long double, vector<fitnessProb>*);

long double chooseFitness(vector<fitnessProb>*);


// objects required for constructing the full vertex distribution according to the alogrithm presented in "Dynamic Generation of Discrete Random Variates" by Matias et al.

struct WeightLeaf {
    // level 0 node in the weight distribution of vertices
    WeightLeaf(int, long double);
    int name; // name of the vertex this leaf represents
    long double weight; // weight of the vertex, in the case of Bianconi-Barabasi model this is fitness multiplied by degree
};


class WeightBranch {
    public:
        WeightBranch(int);
        ~WeightBranch();
        void toggleRoot(); // toggles the boolean value denoting whether or not this branch is a root or not
        bool checkRoot();
        long double getWeight();
        int getName();
        void setRangeOld();
        int getRange();
        void setLevelOne();
        void addWeight(long double);
        long double getWeightOld();
        void setChangesFalse();

        void insertElement(WeightLeaf*);
        void insertElement(WeightBranch*);
        void extractElement(const WeightLeaf*);
        void extractElement(int);

        WeightLeaf* recurRejection();
        int getSize();
    private:
        WeightLeaf* isLevelOne();
        WeightLeaf* isNotLvlOne();
        long double getMaxWeight();
        void setWeightOld();
        bool is_level_one;
        bool is_root;
        bool staged_changes;

        int branch_name; // integer value x that denotes the range [2^(x-1), 2^x) this branch represents
        int old_weight_range; // integer value x that denotes the range [2^(x-1), 2^x) this branch belonged to before changes took place
        long double total_weight; // total weight of this branch
        long double old_weight;
        vector<WeightLeaf*> leafs; // if the branch is level one it will contain a set of leaves (level 0 nodes)
        unordered_map<int, WeightBranch*> branches; // if the branch is not level one it will split into a branch/branches
};

struct WeightTable {
    WeightTable(long double, long double);
    long double total_weight; // total weight in this table
    long double roots; // sum of all the ranges' upper bound
};

WeightBranch* findRange(int, int, unordered_map<int, unordered_map<int, WeightBranch*>>*);

#endif
#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <array>
#include <cmath>
#include <thread>

using std::unordered_map, std::unordered_set, std::vector, std::array;

struct energyRange {
    energyRange(int, int);
    // minimum and maximum (excluded) bounds of the energy as two integers 
    int minimum;
    int maximum;
};

struct fitnessProb{
    fitnessProb();
    fitnessProb(double, double);
    double fitness;
    double weight;
};

void generateBoseEinstein(energyRange, double, vector<fitnessProb>*);

double chooseFitness(vector<fitnessProb>*);

#endif
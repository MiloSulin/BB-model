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


void generateBoseEinstein(energyRange range, double beta_constant, vector<fitnessProb>* fitness_distribution){
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
        double fitness{std::exp(- (beta_constant * energy) )};
        (*fitness_distribution).push_back(fitnessProb(fitness, total_weight));
        energy += increment;
    }
}

double chooseFitness(vector<fitnessProb>* distribution){
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
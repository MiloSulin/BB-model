#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <vector>
#include <array>
#include <thread>
#include <string>
#include "NetworkComponents.hpp"
#include "Generator.hpp"

using std::unordered_map, std::unordered_set, std::vector, std::array, std::cout, std::string;

class Network {
    public:
        Network(string type="uniform", double beta=1.0); // constructor that initialises the network, beta parameter is the physical quantity 1/T for the Bose-Einstein fitness with default value 1.0
        ~Network();
        int chooseVertex();
        void updateWeights();
        void addVertex();
        void printVertices();
        void initWeightDistribution();
        void constructLevel(int, unordered_set<WeightBranch*>*);

    private:
        vector<fitnessProb> fitness_distribution;
        bool uniform_distr;
        double total_weight; // sum of the weights of all vertices in the network

        unordered_map<int, WeightTable*> weight_distribution; // vector of WeightTable objects which include pointers to root branches
        unordered_map<int, unordered_set<WeightBranch*>> level_table; // separates weight ranges of vertices by level, root or not

        vector<Vertex> all_vertices; // vector container for vertices, oldest ones are at the beginning of the vector
        vector<Edge> all_edges; // vector container for the edges of the network
};


#endif
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

class Network{
    public:
        Network(string);
        int chooseVertex();
        void updateWeights();
        void addVertex();
        void printVertices();
    private:
        vector<fitnessProb> fitness_distribution;
        bool uniform_distr{true};
        double total_weight; // sum of the weights of all vertices in the network
        // vector<WeightTable> weight_distribution Subject To Change! vector seems the best one for now, maybe a map or set
        vector<Vertex> all_vertices; // vector container for vertices, oldest ones are at the beginning of the vector
        vector<Edge> all_edges; // vector container for the edges of the network
};

#endif
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <vector>
#include <array>
#include <thread>
#include <string>
#include <random>
#include <exception>
#include "NetworkComponents.hpp"
#include "Network.hpp"
#include "Generator.hpp"

using std::unordered_map, std::unordered_set, std::vector, std::array, std::cout, std::string;


Network::Network(string type) {
    if(type == "BEC"){
        uniform_distr = false;
        generateBoseEinstein(energyRange(0, 1), 0.5, &fitness_distribution);
        for (int i = 0; i < 10; ++i){
            all_vertices.push_back(Vertex(i, chooseFitness(&fitness_distribution), 2));
            if (i != 9){
                all_edges.push_back(Edge(i, i+1));
            }else {
                all_edges.push_back(Edge(i, 0));
            }
        }
    }else if(type == "uniform"){
        std::mt19937 gen;
        std::uniform_real_distribution<> area(0.000001, 0.999999);
        for (int i = 0; i < 10; ++i){
            all_vertices.push_back(Vertex(i, area(gen), 2));
            if (i != 9){
                all_edges.push_back(Edge(i, i+1));
            }else {
                all_edges.push_back(Edge(i, 0));
            }
        }
    }else {
        throw std::invalid_argument("Invalid argument for network type!");
    }

    for (auto& v : all_vertices){
        total_weight += v.calculateWeight();
    }
}

void Network::printVertices(){
    for(auto& v : all_vertices){
        cout << v << '\n';
    }
}
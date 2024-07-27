#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <random>
#include <exception>
#include "NetworkComponents.hpp"
#include "Network.hpp"

using std::unordered_map, std::unordered_set, std::vector, std::array, std::cout, std::string;


Network::Network(string type, double beta) : uniform_distr{true}, total_weight{}, weight_distribution{}, level_table{} {
    if(type == "BEC"){
        uniform_distr = false;
        generateBoseEinstein(energyRange(0, 1), beta, &fitness_distribution);
        for (int i = 0; i < 10; ++i){
            all_vertices.push_back(Vertex(i, chooseFitness(&fitness_distribution), 2));
            if (i != 9){
                all_edges.push_back(Edge(i, i+1));
            }else {
                all_edges.push_back(Edge(i, 0));
            }
        }
    }else if(type == "uniform"){
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

Network::~Network() {
    for (auto& e : weight_distribution){
        delete e.second;
    }
    for (auto& e : level_table){
        for (auto& el : e.second){
            delete el;
        }
    }
}

void Network::printVertices() {
    for(auto& v : all_vertices){
        cout << v << '\n';
    }
}

void Network::constructLevel(int level, unordered_set<WeightBranch*>* lower_ranges) {
    double table_weight{0.0};
    double table_roots{0.0};
    unordered_set<WeightBranch*> higher_ranges;
    bool more_than_one{false};
    for (auto& range : *lower_ranges) {
        if(range->getSize() > 1) {
            int range_of_weight = std::floor( std::log2(range->getWeight()) ) +1;
            auto ptr_new_range = findRange(range_of_weight, level+1, &level_table);
            ptr_new_range->insertElement(range);
            higher_ranges.insert(ptr_new_range);
            if (!more_than_one) {more_than_one = true;}
        }else {
            range->toggleRoot();
            table_weight += range->getWeight();
            table_roots += std::pow(2, range->getRange());
        }
    }
    if (table_weight != 0.0 && table_roots != 0.0){
        WeightTable* new_table = new WeightTable(table_weight, table_roots);
        weight_distribution.insert({level, new_table});
    }
    if (more_than_one) {constructLevel(level+1, &higher_ranges);}
}

void Network::initWeightDistribution() {
    unordered_set<WeightBranch*> changed_ranges;
    for (auto& vertice : all_vertices){
        double v_weight = vertice.calculateWeight();
        int v_name = vertice.getName();

        int range_of_weight = std::floor( std::log2(v_weight) ) +1;
        auto ptr_to_range = findRange(range_of_weight, 1, &level_table);
        ptr_to_range->setLevelOne();

        WeightLeaf* new_leaf = new WeightLeaf(v_name, v_weight);
        ptr_to_range->insertElement(new_leaf);

        changed_ranges.insert(ptr_to_range);
    }
    constructLevel(1, &changed_ranges);
}

void Network::checkWeights() {
    cout << "Total weight of network: " << total_weight << '\n';
    cout << "Weights on tables:\n";
    for (auto& e : weight_distribution){
        cout << e.first << ": " << e.second->total_weight << '\n';
    }
}

void Network::checkLevels() {
    for (const auto& e : level_table){
        cout << "Level: " << e.first << '\n';
        for (const auto& el : e.second){
            cout << "Range: " << el->getRange() << " Weight: " << el->getWeight() << " Children: " << el->getSize() << '\n';
        }
        cout << '\n';
    }
}
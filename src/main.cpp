// bianconi barabasi model
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <vector>
#include <array>
#include <thread>
#include <chrono>
#include "NetworkComponents.hpp"


using namespace std::chrono;
using std::unordered_map, std::unordered_set, std::vector, std::array, std::cout;



class Network{
    Network();
    int chooseVertex();
    void updateWeights();
    void addVertex();
    private:
        double total_weight; // sum of the weights of all vertices in the network
        // vector<WeightTable> weight_distribution Subject To Change! vector seems the best one for now, maybe a map or set
        vector<Vertex> all_vertices; // vector container for vertices, oldest ones are at the beginning of the vector
        vector<Edge> all_edges; // vector container for the edges of the network
};
Network::Network() {

}



int main(){





    return 0;
}
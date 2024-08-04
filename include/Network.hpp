#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <unordered_map>
#include <map>
#include <unordered_set>
#include <iostream>
#include <vector>
#include <string>
#include "NetworkComponents.hpp"
#include "Generator.hpp"

using std::map, std::unordered_map, std::unordered_set, std::vector, std::array, std::cout, std::string;

class Network {
    public:
        Network(string type="uniform", double_t beta=1.0, int size=10, int* e_list1=nullptr, int* e_list2=nullptr, double_t* fit_list=nullptr, bool for_python=false); // beta parameter is the physical quantity 1/T for the Bose-Einstein fitness with default value 1.0
        // Network(string type="uniform", long double beta=1.0, int*, long double*); // Py wrapper version of constructor that doesn't init edge vector and instead uses the provided edge matrix pointer
        ~Network();
        void growNetwork(int, int); // simulates network growth, first parameter denotes how many new vertices are created and the second the amount of new edges they create
        void growNetworkPy(int, int, int*, int*, double_t*);
        // print the respective member variables, for debug purposes
        void printVertices();
        void checkWeights();
        void checkLevels();
    protected:
        void addNewVertex(int, int); // function that adds a new vertex to the network, arguments are the name and degree of the vertex
        void addNewVertexPy(int, int, int*, int*, double_t*, int);
        void constructLevel(int, unordered_set<WeightBranch*>*);
        void initWeightDistribution();
        long double generateFitness();
        int chooseVertex(unordered_set<WeightLeaf *> *);
        void updateLevel(int, unordered_set<WeightBranch*>*);
        void updateWeights(unordered_set<WeightLeaf*>*, unordered_set<WeightBranch*>*);

        // vector<fitnessProb> fitness_distribution;
        bool for_python;
        bool uniform_distr;
        long double total_weight; // sum of the weights of all vertices in the network
        long double beta_constant;

        map<int, WeightTable*> weight_distribution; // vector of WeightTable objects which include pointers to root branches
        unordered_map<int, unordered_map<int, WeightBranch*>> level_table; // separates weight ranges of vertices by level, root or not

        vector<Vertex> all_vertices; // vector container for vertices, oldest ones are at the beginning of the vector
        vector<Edge> all_edges; // vector container for the edges of the network
};

// TODO: FIX THE INHERITANCE IN THIS MESS FOR CLEANER DIFFERENTIATION BETWEEN C++ AND PYTHON VERSIONS
// class NetworkPy: public Network {
//     public:
//         NetworkPy(string , double_t , int , int* , int* , double_t*);
//         void growNetwork(int, int, int*, int*, double_t*);
//     protected:
//         void addNewVertex(int, int, int*, int*, double_t*);
//         int size;
//         bool uniform_distr;
//         long double total_weight; // sum of the weights of all vertices in the network
//         long double beta_constant;

//         map<int, WeightTable *> weight_distribution;                       // vector of WeightTable objects which include pointers to root branches
//         unordered_map<int, unordered_map<int, WeightBranch*>> level_table; // separates weight ranges of vertices by level, root or not

//         vector<Vertex> all_vertices; // vector container for vertices, oldest ones are at the beginning of the vector
// };


#endif
#ifndef __NETWORKCOMPONENTS_H__
#define __NETWORKCOMPONENTS_H__
// basic components of a network, vertices and edges
#include <iostream>
#include <vector>
#include <array>
#include <thread>

using std::vector, std::array, std::cout;

class Vertex{
    public:
        Vertex(int , long double , int ); // constructor
        long double calculateWeight(); // calculates the weight of the node, in the case of Bianconi-Barabasi model this is degree * fitness
        long double getFitness(); // returns the fitness of the vertex
        void increaseDegree(); // increments the degree of the vertex by one
        int getName(); // returns the name of the vertex
        int getDegree(); // returns the degree of the vertex
    private:
        const int name; // vertex is named with an int which denotes the "time" it was created, meaning that an older vertex will have a lower integer name
        const long double fitness; // fitness of the vertex, usually chosen randomly according to some distribution
        int degree; // degree of the vertex
};
std::ostream& operator<<(std::ostream&, Vertex& );

struct Edge{
    // describes an edge between two vertices
    // in a directed network a descendant vertex explicitly points to the ancestor
    Edge(int, int); // constructor that takes the names of two vertices as inputs
    int descendant_vertex;
    int ancestor_vertex;
};

#endif
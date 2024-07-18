#ifndef __NETWORKCOMPONENTS_H__
#define __NETWORKCOMPONENTS_H__
// basic components of a network, vertices and edges
#include <iostream>
#include <vector>
#include <array>
#include <thread>

using std::vector, std::array, std::cout;

class Vertex{
    Vertex(int , double , int ); // constructor
    double calculateWeight(); // calculates the weight of the node, in the case of Bianconi-Barabasi model this is degree * fitness
    double getFitness(); // returns the fitness of the vertex
    void increaseDegree(); // increments the degree of the vertex by one
    private:
        const int name; // vertex is named with an int which denotes the "time" it was created, meaning that an older vertex will have a lower integer name
        const double fitness; // fitness of the vertex, usually chosen randomly according to some distribution
        vector<int> edges; // just in case, vector for storing the names of vertices this vertex is connected to
        int degree; // degree of the vertex
};
Vertex::Vertex(int Name, double Fit, int init_degree) : name{Name}, fitness{Fit}, degree{init_degree} {}; // assigns name, fitness and an initial degree to a vertex when it is created

double Vertex::calculateWeight(){
    return degree * fitness;
}
double Vertex::getFitness(){
    return fitness;
}
void Vertex::increaseDegree(){
    Vertex::degree++;
}

struct Edge{
    // describes an edge, a link, between two vertices
    // in a directed network a descendant vertex points to the ancestor
    Edge(int, int); // constructor that takes the names of two vertices as inputs
    int descendant_vertex;
    int ancestor_vertex;
};

#endif
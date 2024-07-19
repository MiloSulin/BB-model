#include <iostream>
#include <vector>
#include <array>
#include <thread>
#include "NetworkComponents.hpp"

using std::vector, std::array, std::cout;

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
int Vertex::getName(){
    return name;
}
int Vertex::getDegree(){
    return degree;
}

std::ostream& operator<<(std::ostream& stream, Vertex& v_printable){
    stream << v_printable.getName() << ' ' << v_printable.getFitness() << ' ' << v_printable.getDegree();
    return stream;
};


Edge::Edge(int a_v, int d_v): ancestor_vertex{a_v}, descendant_vertex{d_v} {}; // a_v is the name of the ancestor vertex, d_v is the descendant vertex
// bianconi barabasi model
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include "Network.hpp"

using namespace std::chrono;
using std::unordered_map, std::unordered_set, std::vector, std::array, std::cout;

int main() {
    const auto start_t = high_resolution_clock::now();

    string type = "BEC";
    double_t beta = 0; 
    Network test = Network(type, beta);
    test.growNetwork(1000000, 2);
    
    const auto end_t = high_resolution_clock::now();
    duration<double, std::milli> elapsed_time{end_t - start_t};
    cout << "Time taken: " << elapsed_time << std::endl;

    return 0;
}
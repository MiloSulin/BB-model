// bianconi barabasi model
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <chrono>
#include <cmath>
#include "Network.hpp"

using namespace std::chrono;
using std::unordered_map, std::unordered_set, std::vector, std::array, std::cout;

int main() {
    const auto start_t = high_resolution_clock::now();

    Network test = Network("BEC", 0.5);
    test.growNetwork(100000, 2);
    // cout << "Test\n";
    
    const auto end_t = high_resolution_clock::now();
    duration<double, std::milli> elapsed_time{end_t - start_t};
    cout << "Time taken: " << elapsed_time << std::endl;

    return 0;
}
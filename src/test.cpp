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
    duration<double, std::milli> total{};
    int range{20};
    for (int i=0; i<range; ++i){
        const auto start_t = high_resolution_clock::now();

        Network test = Network("BEC", 0.5);
        test.growNetwork(100000, 2);
        // cout << "Test\n";

        const auto end_t = high_resolution_clock::now();
        total += end_t - start_t;
    }
    cout << "Time taken on average: " << total / range << std::endl;

    return 0;
}
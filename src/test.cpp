// bianconi barabasi model
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <vector>
#include <iostream>
#include <chrono>
#include <cmath>
#include "Network.hpp"

using namespace std::chrono;
using std::unordered_map, std::unordered_set, std::vector, std::array, std::cout;

// struct TestStruct {
//     TestStruct(int, int*);
//     int index;
//     int* ptr;
// };
// TestStruct::TestStruct(int i, int* p): index{i}, ptr{p} {}

// struct TestCompare{
//     bool operator()(const TestStruct* ts1, const TestStruct* ts2) const {
//         return ( (ts1->ptr != ts2->ptr) && ( (ts1->index >= ts2->index) ) );
//     }
// };

int main() {
    duration<double, std::milli> total{};
    int range{20};
    for (int i=0; i<range; ++i){
        const auto start_t = high_resolution_clock::now();

        Network test = Network("uniform", 2.0);
        test.growNetwork(10000, 3);
        // cout << "Test\n";

        const auto end_t = high_resolution_clock::now();
        total += end_t - start_t;
    }
    cout << "Time taken on average: " << total / range << std::endl;

    return 0;
}
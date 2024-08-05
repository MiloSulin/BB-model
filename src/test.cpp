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

    string type{};
    double_t beta {};
    cout << "Give type (uniform or BEC): ";
    std::cin >> type;
    cout << "Give (double typed) value for beta constant: ";
    std::cin >> beta;
    cout << "Thank you!\n";
    cout << "Beta was converted to: " << std::setprecision(15) << (long double)beta <<'\n';
    Network test = Network(type, beta);
    test.growNetwork(10000, 2);
    
    const auto end_t = high_resolution_clock::now();
    duration<double, std::milli> elapsed_time{end_t - start_t};
    cout << "Time taken: " << elapsed_time << std::endl;

    return 0;
}
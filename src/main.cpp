// bianconi barabasi model
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <vector>
#include <array>
#include <chrono>
#include <cmath>
#include "Network.hpp"

using namespace std::chrono;
using std::unordered_map, std::unordered_set, std::vector, std::array, std::cout;


int main() {
    string type = "BEC";
    Network test = Network(type);
    test.printVertices();
    test.initWeightDistribution();
    test.checkWeights();
    test.checkLevels();

    return 0;
}
#include <iostream>
#include "../include/Network.hpp"

#ifdef _WIN32
#  define API __declspec(dllexport)
#else
#  define API
#endif

extern "C" API void generateBianconiBarabasiPy(int32_t v_amount, int32_t e_amount, char* type, double_t beta_constant, int32_t* edge_list1, int32_t* edge_list2, double_t* vertex_fit_list) {
    std::string type_str{type};
    int vertices_to_generate = v_amount - 10;
    if (vertices_to_generate < 0){
        throw std::invalid_argument("Network size smaller than minimum network generated at initialisation! Please input a total network size >= 10!\n");
    }
    auto bb_network_for_py = new Network(type_str, beta_constant, edge_list1, edge_list2, vertex_fit_list, true);
    bb_network_for_py->growNetworkPy(vertices_to_generate, e_amount, edge_list1, edge_list2, vertex_fit_list);
    delete bb_network_for_py;
}

// these existed for testing purposes

// extern "C" API void cPyTest2(int32_t* test_arr, std::size_t size) {
//     for (int i=0; i<size; ++i){
//         test_arr[i] = i;
//     }
// }

// extern "C" API void cPyTest() {
//     std::cout << "This should work.\n";
// }

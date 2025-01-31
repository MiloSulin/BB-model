#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <exception>
#include "../include/Network.hpp"

using std::unordered_map, std::unordered_set, std::vector, std::cout, std::string;


Network::Network(string type, double_t beta, int* e_list1, int* e_list2, double_t* fit_list, bool for_py) : uniform_distr{true}, total_weight{}, beta_constant{beta}, weight_distribution{}, level_table{} {
    if (for_py == false){
        all_vertices.reserve(10);
        all_edges.reserve(10);
        if(type == "BEC"){
            uniform_distr = false;
            for (int i = 0; i < 10; ++i){
                all_vertices.emplace_back(Vertex(i, generateFitness(), 2));
                if (i != 9){
                    all_edges.emplace_back(Edge(i, i+1));
                }else {
                    all_edges.emplace_back(Edge(i, 0));
                }
            }
        }else if(type == "uniform"){
            for (int i = 0; i < 10; ++i){
                all_vertices.emplace_back(Vertex(i, generateFitness(), 2));
                if (i != 9){
                    all_edges.emplace_back(Edge(i, i+1));
                }else {
                    all_edges.emplace_back(Edge(i, 0));
                }
            }
        }else {
            throw std::invalid_argument("Invalid argument for network type!");
        }

        for (auto& v : all_vertices){
            total_weight += v.calculateWeight();
        }
    } else if (for_py == true){
        if (type == "BEC"){
            uniform_distr = false;
            for (int i = 0; i < 10; ++i){
                all_vertices.emplace_back(Vertex(i, generateFitness(), 2));
                (fit_list[i]) = double_t(all_vertices.back().getFitness());
                if (i != 9){
                    // all_edges.emplace_back(Edge(i, i+1));
                    (e_list1[i]) = i;
                    (e_list2[i]) = i+1;
                }else {
                    // all_edges.emplace_back(Edge(i, 0));
                    (e_list1[i]) = i;
                    (e_list2[i]) = 0;
                }
            }
        }else if (type == "uniform"){
            uniform_distr = true;
            for (int i = 0; i < 10; ++i){
                all_vertices.emplace_back(Vertex(i, generateFitness(), 2));
                (fit_list[i]) = double_t(all_vertices.back().getFitness());
                if (i != 9){
                    // all_edges.emplace_back(Edge(i, i+1));
                    (e_list1[i]) = i;
                    (e_list2[i]) = i+1;
                }else {
                    // all_edges.emplace_back(Edge(i, 0));
                    (e_list1[i]) = i;
                    (e_list2[i]) = 0;
                }
            }
        }else {
            throw std::invalid_argument("Invalid argument for network type!");
    }
        for (auto& v : this->all_vertices){
            this->total_weight += v.calculateWeight();
        }
    }
    initWeightDistribution();
}


Network::~Network() {
    for (auto& e : weight_distribution){
        delete e.second;
    }
    for (auto& e : level_table){
        for (auto& [key, el] : e.second){
            delete el;
        }
    }
}

void Network::printVertices() {
    // for debug purposes
    for(auto& v : all_vertices){
        cout << v << '\n';
    }
}

void Network::constructLevel(int level, vector<WeightBranch*> &lower_ranges) {
    long double table_weight{0.0};
    long double table_roots{0.0};
    vector<WeightBranch*> higher_ranges{};
    for (auto& range : lower_ranges) {
        if (range->checkChanges() == false){
            continue;
        }
        if(range->getSize() > 1) {
            int range_of_weight = std::floor( std::log2(range->getWeight()) ) +1;
            auto ptr_new_range = findRange(range_of_weight, level+1, &level_table);
            ptr_new_range->insertElement(range);
            higher_ranges.push_back(ptr_new_range);
        }else if(range->getSize() == 1){
            range->toggleRoot();
            table_weight += range->getWeight();
            table_roots += std::pow(2, range->getName());
        }
        range->setChangesFalse();
    }
    if (table_weight != 0.0 && table_roots != 0.0){
        WeightTable* new_table = new WeightTable(table_weight, table_roots);
        weight_distribution.insert({level, new_table});
    }
    if (!higher_ranges.empty()) {constructLevel(level+1, higher_ranges);}
}

void Network::initWeightDistribution() {
    vector<WeightBranch*> changed_ranges{};
    for (auto& vertex : all_vertices){
        long double v_weight = vertex.calculateWeight();
        int v_name = vertex.getName();

        int range_of_weight = std::floor( std::log2(v_weight) ) +1;
        auto ptr_to_range = findRange(range_of_weight, 1, &level_table);
        ptr_to_range->setLevelOne();

        WeightLeaf* new_leaf = new WeightLeaf(v_name, v_weight);
        ptr_to_range->insertElement(new_leaf);

        changed_ranges.push_back(ptr_to_range);
    }
    constructLevel(1, changed_ranges);
}

long double Network::generateFitness() {
    if (this->uniform_distr){
        std::uniform_real_distribution<> area(0.0, 1.0);
        return area(gen);
    } else{
        std::uniform_real_distribution<> nrg_distr(0.0, 1.0);
        std::uniform_real_distribution<> area(0.0, 2.0);
        // generate variate between 0 and max value of energy distribution according to article
        long double variate = area(gen);
        // generate energy
        long double energy = nrg_distr(gen);
        long double prob = 2*std::pow(energy, 1);
        while (variate >= prob){
            variate = area(gen);
            energy = nrg_distr(gen);
            prob = 2*std::pow(energy, 1);
        }
        long double fitness = std::exp( -(beta_constant*energy) );
        return fitness;
    }
}

int Network::chooseVertex(set<LeafResult, LeafCompare> &changed_leafs) {
    std::uniform_real_distribution<> area(0.0, 1.0);
    int chosen_level{};
    long double variate = area(gen);
    long double weight_sum{};
    
    // choose table
    for (auto& table : weight_distribution){
        weight_sum += table.second->total_weight;
        if (variate < weight_sum / this->total_weight){
            chosen_level = table.first;
            break;
        }
    }
    
    // choose root from table
    auto chosen_table = weight_distribution[chosen_level];
    long double table_weight = chosen_table->total_weight;
    long double roots_range = chosen_table->roots;

    variate = area(gen); // generate new variate
    int chosen_root_range = std::floor( std::log2(roots_range) ); // integer representing the range moniker of the chosen root
    long double cumul_weight = level_table[chosen_level][chosen_root_range]->getWeight();

    while ((variate > cumul_weight / table_weight) && roots_range != 0){ // iterate until the ratio of the sum and table weight is bigger than variate, start from highest range
        roots_range -= std::pow(2, chosen_root_range);
        chosen_root_range = std::floor( std::log2(roots_range) );
        cumul_weight += level_table[chosen_level][chosen_root_range]->getWeight();
    }
    auto chosen_root_ptr = level_table[chosen_level][chosen_root_range];

    if(!chosen_root_ptr->checkRoot()){ // check that chosen branch actually is a root
        std::cerr << "Chosen root range isn't a root! Something has gone very wrong...\n";
        throw std::runtime_error("");
    }
    // perform recursive rejection method until a leaf is reached and therefore a vertex chosen
    LeafResult leaf_result = chosen_root_ptr->recurRejection();
    int chosen_vertex = leaf_result.chosen_leaf->name;
    changed_leafs.insert(leaf_result);
    all_vertices[chosen_vertex].increaseDegree();

    return chosen_vertex;
}

void Network::updateLevel(int level, vector<WeightBranch*> &lower_branches) {

    vector<WeightBranch*> higher_branches{};
    higher_branches.reserve(lower_branches.size() * 2);
    long double table_weight_delta{0.0};
    long double table_roots_delta{0.0};
    bool level_has_roots{weight_distribution.contains(level)};

    for (auto& branch : lower_branches){
        if (branch->checkChanges() == false){ // check if branch has been added to the queue twice and skip if updates have already been performed
            continue;
        }
        int size_old = branch->getSizeOld();
        // int range_old = branch->getRange();
        long double weight_old = branch->getWeightOld();
        int range_old = std::floor( std::log2(weight_old) ) +1;
        // if (weight_old < 1e-10){ // if the old weight is exceptionally small we set it to 0.0, if we don't do this the whole thing will crash due to floating point errors
        //     weight_old = 0.0;
        // }

        long double weight_new = branch->getWeight();
        int range_new = std::floor( std::log2(weight_new) ) +1;
        int branch_name = branch->getName();
        int branch_size = branch->getSize();

        if (size_old != 0 && !branch->checkRoot()){ // those branches which had a weight above 0 before changes took place and were not roots will have a parent
            if(!level_table[level+1].contains(range_old)){ // print information about state to command line if an impossible state is reached
                this->checkWeights();
                this->checkLevels();
                cout << '\n';
                cout << "Level: " << level << '\n';
                cout << "Level+1: " << level+1 << '\n';
                cout << "Looked for range: " << range_old << '\n';
                cout << "Compared to range new: " << range_new << '\n';
                cout << "Weight old: " << branch->getWeightOld() << '\n';
                cout << "New weight: " << weight_new << '\n';
                throw std::runtime_error("By all means this shouldn't be able to happen.");
            }
            auto ptr_old_branch = level_table[level+1][range_old]; // get pointer to parent branch

            if (range_old != range_new && branch_size > 1){ // if range has changed we will have to move the branch to a new parent
                ptr_old_branch->extractElement(branch_name);

                auto ptr_new_branch = findRange(range_new, level+1, &level_table);
                ptr_new_branch->insertElement(branch);
                higher_branches.push_back(ptr_new_branch);
            } else if(range_old == range_new && branch_size > 1){ // if range hasn't changed we still need to update the weight of the parent
                ptr_old_branch->addWeight(weight_new - weight_old);
            } else if(branch_size == 1){ // if the branch has only one child it will become a root
                ptr_old_branch->extractElement(branch_name);
                if (level_has_roots){
                    weight_distribution[level]->roots += std::pow(2, branch_name);
                    weight_distribution[level]->total_weight += weight_new;
                } else{
                    table_roots_delta += std::pow(2, branch_name);
                    table_weight_delta += weight_new;
                }
            } else if(branch_size == 0){ // the case of a branch losing all its children
                ptr_old_branch->extractElement(branch_name);
            }
            higher_branches.push_back(ptr_old_branch);
        } else if(size_old != 0 && branch->checkRoot()){ // case of a branch not being empty at the start and being a root
            if (branch_size > 1){ // branch has more than one child and so it stops being a root
                if(level_has_roots){
                    weight_distribution[level]->roots -= std::pow(2, branch_name);
                    weight_distribution[level]->total_weight -= weight_old;
                } else{
                    std::cerr << "THIS SHOULDN'T HAPPEN!\n"; // THIS SHOULDN'T HAPPEN
                }

                auto ptr_new_range = findRange(range_new, level+1, &level_table);
                ptr_new_range->insertElement(branch);
                higher_branches.push_back(ptr_new_range);
            } else if(branch_size == 1){ // branch stays as root, only the level table weight needs to be updated
                weight_distribution[level]->total_weight += (weight_new - weight_old);
            } else if(branch_size == 0){ // branch lost its child and so will be removed from the level table
                weight_distribution[level]->roots -= std::pow(2, branch_name);
                weight_distribution[level]->total_weight -= weight_old;
            }
        } else if(size_old == 0){ // case of branch being empty before updates
            if (branch_size > 1){
                auto ptr_new_range = findRange(range_new, level+1, &level_table);
                ptr_new_range->insertElement(branch);
                higher_branches.push_back(ptr_new_range);
            } else if(branch_size == 1){ // branch becomes a root
                if (level_has_roots){
                    weight_distribution[level]->roots += std::pow(2, branch_name);
                    weight_distribution[level]->total_weight += weight_new;
                } else{
                    table_roots_delta += std::pow(2, branch_name);
                    table_weight_delta += weight_new;
                }
            } // case of branch being empty again (size==0) shouldn't happen
        } else{
            std::cerr << "A case that was not accounted for during weight updates occurred! This is very bad!\n";
        }

        // handle branch's root state
        if (branch->checkRoot() && branch_size > 1){
            branch->toggleRoot();
        } else if(!branch->checkRoot() && branch_size == 1){
            branch->toggleRoot();
        } else if(branch_size == 0 && branch->checkRoot()){
            branch->toggleRoot();
        }
        branch->setChangesFalse();
    }
    // update the level table according to the changes that were recorded or create a new one if no roots have yet existed on this level
    if (table_roots_delta != 0.0 && table_weight_delta != 0.0){
        WeightTable* new_level = new WeightTable(table_weight_delta, table_roots_delta);
        weight_distribution.insert({level, new_level});
    }

    if (!higher_branches.empty()) {updateLevel(level+1, higher_branches);} // if branches on the above level were changed update recursively
}

void Network::updateWeights(set<LeafResult, LeafCompare> &changed_leafs, vector<WeightBranch*> &higher_branches) {
    for (auto& leaf_result : changed_leafs){
        auto leaf = leaf_result.chosen_leaf;
        auto leaf_index = leaf_result.leaf_index;
        // get range before changing weight
        int range_old = std::floor( std::log2(leaf->weight) ) +1;

        // find vertex modeled by current leaf
        auto vertex_ptr = &(all_vertices[leaf->name]);
        long double old_weight = leaf->weight;
        long double new_weight = vertex_ptr->calculateWeight(); // calculate new weight
        // calculate new range
        int range_new = std::floor( std::log2(new_weight) ) +1;

        // check if range has changed and update if needed
        if (range_old != range_new){
            level_table[1][range_old]->extractElement(leaf, leaf_index);
            higher_branches.push_back(level_table[1][range_old]);
            auto ptr_new_range = findRange(range_new, 1, &level_table);
            ptr_new_range->setLevelOne();
            
            leaf->weight = new_weight;
            ptr_new_range->insertElement(leaf);
            higher_branches.push_back(ptr_new_range);
        } else{ // if range hasn't changed update the total weight of the branch
            leaf->weight = new_weight;
            level_table[1][range_old]->addWeight(new_weight - old_weight);
            higher_branches.push_back(level_table[1][range_old]);
        }

        this->total_weight += (new_weight - old_weight); // update total weight of the whole network
    }

    /*for (auto& e : changed_leafs){*/
    /*    delete e;*/
    /*}*/
    
    updateLevel(1, higher_branches);
}

void Network::addNewVertex(int name, int degree) {
    // create new vertex
    all_vertices.emplace_back(Vertex(name, generateFitness(), degree));
    // container for the new leaf and the leaves of vertices that the new vertex connects to
    set<LeafResult, LeafCompare> changed_leafs{};
    vector<WeightBranch*> changed_branches{};
    changed_branches.reserve(degree);

    // choose degree amount of vertices according to the attachment mechanism and add their leaves to the set of changed weights (leafs)
    for (int i=0; i<degree; ++i){
        all_edges.emplace_back(Edge(chooseVertex(changed_leafs), name));
    }

    // create a leaf for the new vertex
    Vertex* new_vertex = &(all_vertices[name]);
    long double v_weight = new_vertex->calculateWeight();
    int range_of_weight = std::floor(std::log2(v_weight)) + 1;
    auto ptr_to_range = findRange(range_of_weight, 1, &level_table);
    ptr_to_range->setLevelOne();
    WeightLeaf* new_leaf = new WeightLeaf(name, v_weight);
    ptr_to_range->insertElement(new_leaf);
    changed_branches.push_back(ptr_to_range);

    total_weight += v_weight;

    // update the weights of the leafs that were changed and the weights of upper range branches when necessary
    updateWeights(changed_leafs, changed_branches);
}

void Network::addNewVertexPy(int name, int degree, int* e_list1, int* e_list2, double_t* fit_list, int edge_index) {
    // create new vertex
    all_vertices.emplace_back(Vertex(name, generateFitness(), degree));
    (fit_list[name]) = double_t(all_vertices.back().getFitness());
    // container for the new leaf and the leafs of vertices that the new vertex connects to
    set<LeafResult, LeafCompare> changed_leafs{};
    vector<WeightBranch*> changed_branches{};
    changed_branches.reserve(degree);

    // choose degree amount of vertices according to the attachment mechanism and add their leafs to the set of changed weights (leafs)
    for (int i=0; i<degree; ++i){
        // all_edges.emplace_back(Edge(chooseVertex(&changed_leafs), name));
        int ancestor_name = chooseVertex(changed_leafs);
        (e_list1[edge_index+i]) = ancestor_name;
        (e_list2[edge_index+i]) = name;
    }

    // create a leaf for the new vertex
    Vertex* new_vertex = &(all_vertices.back());
    long double v_weight = new_vertex->calculateWeight();
    int range_of_weight = std::floor(std::log2(v_weight)) + 1;
    auto ptr_to_range = findRange(range_of_weight, 1, &level_table);
    ptr_to_range->setLevelOne();
    WeightLeaf* new_leaf = new WeightLeaf(name, v_weight);
    ptr_to_range->insertElement(new_leaf);
    changed_branches.push_back(ptr_to_range);

    total_weight += v_weight;

    // update the weights of the leafs that were changed and the weights of upper range branches when necessary
    updateWeights(changed_leafs, changed_branches);
}

void Network::growNetwork(int v_amount, int e_amount) {
    int first_name = all_vertices.size();
    all_vertices.reserve(v_amount);
    all_edges.reserve(v_amount*e_amount -(first_name*e_amount - 1) ); // Each new vertex adds e_amount new edges to the system except the first vertices created during init which only add 1 per vertex
    for (int i=0; i<v_amount - first_name; ++i){ // generate vertices until desired network size is reached
        addNewVertex(first_name+i, e_amount);
    }
    // cout << "All done\n";
}

void Network::growNetworkPy(int v_amount, int e_amount, int* e_list1, int* e_list2, double_t* fit_list) {
    int first_name = all_vertices.size();
    int edge_index = first_name;
    all_vertices.reserve(v_amount);
    for (int i=0; i<v_amount - first_name; ++i){ // generate vertices until desired network size is reached
        addNewVertexPy(first_name+i, e_amount, e_list1, e_list2, fit_list, edge_index);
        edge_index += e_amount;
    }
}

// these print state info to command line

void Network::checkWeights() {
    cout << "Total weight of network: " << total_weight << '\n';
    cout << "Weights on tables:\n";
    for (auto& e : weight_distribution){
        cout << e.first << ": " << e.second->total_weight << '\n';
    }
}

void Network::checkLevels() {
    for (auto& e : level_table){
        cout << "Level: " << e.first << '\n';
        for (const auto& [key, branch] : e.second){
            cout << "Range: " << branch->getName() << " Weight: " << branch->getWeight() << " Children: " << branch->getSize() << '\n';
        }
        cout << '\n';
    }
}

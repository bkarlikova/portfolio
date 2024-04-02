#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <random>

using namespace std;

class TSP {
    /**
     * @brief _matrix is a container storing the path cost from city with index i to j
     */
    vector<vector<int>> _matrix;
    /**
     * @brief list of names of cities/vertices
     */
    vector<string> _cities;
    /**
     * @brief Currently the best solution found. When the algorithm starts it's initialized with the fist init state, after finding some local maximum it's updated if better solution was found
     */
    vector<int> _best_from_all_searched;

public:
    /**
     * @brief Function that loads data from .csv file stored in directory ../data/
     * @param file_name name of the file stored in mentioned directory
     */
    void load_data(const string &file_name);
    /**
    * @brief simple function that prints the best path in string with names of the cities and the length of the path between them
    * @param result vector of indices of cities
    */
    void print_result(const vector<int> &result) const;
    /**
     * @brief This function generates a new init state from sequence 0-num_of_cities or from last state, so the states are not the same
     * @param last_state previous state
     * @return random sequence of numbers representing the cities indices.
     */
    vector<int> get_new_init_state(const optional<vector<int>> &last_state = nullopt) const;
    /**
     * @brief Function that finds the best neighbour from generated neighbours
     * @param neighbours some possible neighbour states
     * @return the best next state from given offer
     */
    [[nodiscard]] vector<int> find_best_neighbour(const vector<vector<int>> &neighbours) const;
    /**
     * @brief Function that generates n possible neighbour states. It swaps two different vertices which is not the first/last
     * @param current some solution, doesn't need to be the currently best one, it's just some current solution
     * @param n the maximum of neighbours to be generated (max is 9*8);
     * @return n possible neighbour states
     */
    vector<vector<int> > generate_neighbours(const vector<int> &current, size_t n = 20);
    /**
     * @brief Function that calculates the cost of given path using the matrix of costs
     * @param path vector of indices representing the cities
     * @return the cost of a path
     */
    [[nodiscard]] int calculate_path(const vector<int> & path) const;
    /**
     * @brief This function tries to find the best solution until restart threshold is fulfilled.
     * @param not_better_threshold Threshold determines the maximum tries of finding a better neighbour.
     * @param restart_threshold This determines the maximum number of attempts with new initial state
     * @return the best path found by local search
     */
    vector<int> hill_climbing(int restart_threshold = 10, int not_better_threshold = 2);
};

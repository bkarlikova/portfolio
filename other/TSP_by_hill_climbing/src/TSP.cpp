#include "TSP.h"

void TSP::load_data(const string &file_name) {
    ifstream ifs;
    ifs.open("../data/" + file_name, ifstream::in);
    if (!ifs.is_open()) {
        throw runtime_error("file is not opened");
    }
    string line;
    while (getline(ifs, line)) {
        stringstream iss(line);
        string tmp;
        if (getline(iss, tmp, ',')) {
            _cities.emplace_back(tmp);
        }
        vector<int> v_tmp;
        while (getline(iss, tmp, ',')) {
            v_tmp.emplace_back(stoi(tmp));
        }
        _matrix.emplace_back(v_tmp);
        v_tmp.clear();
    }
    if (_matrix.size() != _matrix.at(0).size()) {
        throw runtime_error("incorrect size of matrix");
    }

    cout << "loading successful" << endl;
}

vector<vector<int> > TSP::generate_neighbours(const vector<int> &current, size_t n) {
    vector<vector<int>> neighbors;
    vector<pair<int, int> > used_combination;
    while (neighbors.size() < n) {
        pair<size_t, size_t> tmp = make_pair((rand() % _matrix.size()), (rand() % _matrix.size()));
        if (tmp.first != tmp.second
            && find_if(used_combination.begin(), used_combination.end(),
                       [&tmp](const auto &el) { return tmp.first == el.first && el.second == tmp.second;}) == used_combination.end()) {
            used_combination.emplace_back(tmp);
            used_combination.emplace_back(tmp.second, tmp.first);
            vector<int> new_neighbour = current;
            swap(new_neighbour.at(tmp.first), new_neighbour.at(tmp.second));
            neighbors.emplace_back(new_neighbour);
            try{
                //swap a pair of vertices
                new_neighbour = current;
                swap(new_neighbour.at(tmp.first), new_neighbour.at(tmp.second));
                swap(new_neighbour.at(tmp.first + 1), new_neighbour.at(tmp.second + 1));
                neighbors.emplace_back(new_neighbour);
                //swap a triplet of vertices
                new_neighbour = current;
                swap(new_neighbour.at(tmp.first), new_neighbour.at(tmp.second));
                swap(new_neighbour.at(tmp.first + 1), new_neighbour.at(tmp.second + 1));
                swap(new_neighbour.at(tmp.first + 2), new_neighbour.at(tmp.second + 2));
                neighbors.emplace_back(new_neighbour);
                //swap two vertices
                new_neighbour = current;
                swap(new_neighbour.at(tmp.first), new_neighbour.at(tmp.second));
                swap(new_neighbour.at(tmp.first + 3), new_neighbour.at(tmp.second + 3));
                swap(new_neighbour.at(tmp.first + 4), new_neighbour.at(tmp.second + 4));
                neighbors.emplace_back(new_neighbour);
            }
            catch (...){
                continue;
            }
        }
    }
    return neighbors;
}


int TSP::calculate_path(const vector<int> & path) const {
    int path_cost = 0;
    for (size_t i = 0; i < path.size() - 1; i++) {
        path_cost += _matrix.at(path.at(i)).at(path.at(i + 1));
    }
    path_cost += _matrix.at(path.back()).at(path.at(0));
    return path_cost;
}


vector<int> TSP::find_best_neighbour(const vector<vector<int>> &neighbours) const {
    vector<int> best_neighbour = neighbours.front();
    for (auto &neighbour: neighbours) {
        if (calculate_path(best_neighbour) > calculate_path(neighbour)) {
            best_neighbour = neighbour;
        }
    }
    return best_neighbour;
}

vector<int> TSP::hill_climbing(int restart_threshold, int not_better_threshold) {
    vector<int> current_best = get_new_init_state();
    _best_from_all_searched = current_best;
    int not_better = 0;
    int restart = 0;
    while (restart < restart_threshold) {
        while(not_better < not_better_threshold){
            vector<int> try_state = find_best_neighbour(generate_neighbours(current_best, (current_best.size() - 1) * (current_best.size() - 2)));
            if (calculate_path(try_state) < calculate_path(current_best)) {
                current_best = try_state;
                if (calculate_path(_best_from_all_searched) > calculate_path(current_best)) {
                    _best_from_all_searched = current_best;
                    break;
                }
            }
            else{
                not_better++;
            }
        }
        cout << "Best current state before restart:" << endl;
        print_result(current_best);
        current_best = get_new_init_state(current_best);
        restart++;
    }
    return _best_from_all_searched;
}

vector<int> TSP::get_new_init_state(const optional<vector<int>> &last_state) const {
    vector<int> tmp(_matrix.size());
    if (last_state) {
        tmp = *last_state;
    }
    iota(tmp.begin(), tmp.end(), 0);
    random_device rd;
    mt19937 g(rd());
    shuffle(begin(tmp), end(tmp), g);
    return tmp;
}

void TSP::print_result(const vector<int> &result) const {
    cout << "Found path:" << endl;
    for (size_t i = 0; i < result.size() - 1; i++) {
        cout << _cities.at(result.at(i))
             << " --"
             << _matrix.at(result.at(i)).at(result.at(i + 1))
             << "--> ";
    }
    cout << _cities.at(result.back())
         << " --"
         << _matrix.at(result.back()).at(result.front())
         << "--> "
         << _cities.at(result.front()) << endl;
    cout << "cost: " << calculate_path(result) << endl;
}

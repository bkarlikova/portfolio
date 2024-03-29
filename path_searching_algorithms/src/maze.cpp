#include "maze.h"

Maze::Maze() = default;

Maze::~Maze() = default;

bool Maze::load_maze(const string &fileName) {
    ifstream ifs;
    ifs.open(fileName, ifstream::in);
    if (!ifs.is_open()) {
        cout << "not open" << endl;
    }
    string line;
    size_t i = 0;
    maze.resize(1);
    while (getline(ifs, line)) {
        stringstream iss(line);
        char tmp;
        while (iss >> noskipws >> tmp) {
            if (tmp == 's') {
                start = load_start_finish(line);
                break;
            }
            if (tmp == 'e') {
                finish = load_start_finish(line);
                //resize to actuall size of the maze, 2 ifs just random constant, that works
                maze.resize(maze.size() - 2);
                return true;
            }
            maze.at(i).emplace_back(tmp);
        }
        i++;
        maze.resize(i + 1);
    }
    return false;
}

pair<int, int> Maze::load_start_finish(const string &line) {
    stringstream ss(line);
    string tmp;
    int num, cnt = 0;
    pair<int, int> output;
    while (!ss.eof()) {
        ss >> tmp;
        if (stringstream(tmp) >> num) {
            if (cnt == 0) {
                output.second = num;
                cnt++;
            } else if (cnt == 1) {
                output.first = num;
                return output;
            }
        }
    }
    return output;
}

bool Maze::initialize_maze() {
    /*cout << maze.size() << " " << maze.at(0).size() << endl;
    cout << start.first << " " << start.second << endl;
    cout << finish.first << " " << finish.second << endl;*/
    /// this function is checking the start and finish node, initializing them into maze///
    if (start.first == finish.first && start.second == finish.second) {
        throw invalid_argument("Invalid maze: Start is finish");
    } else if (0 >= start.first || 0 >= start.second || int(maze.size()) <= start.first ||
               int(maze.at(0).size()) <= start.second) {
        throw invalid_argument("Invalid maze: Start is out of range");
    } else if (0 >= finish.first || 0 >= finish.second || int(maze.size()) <= finish.first ||
               int(maze.at(0).size()) <= finish.second) {
        throw invalid_argument("Invalid maze: Finish is out of range");
    }
    maze.at(start.first).at(start.second) = START;
    maze.at(finish.first).at(finish.second) = END;
    /*if (maze.size() > maze.at(0).size()) {
        flip_maze();
    }*/
    return true;
}

void Maze::print_maze() const {
    for (const auto &i: maze) {
        for (char j: i) {
            switch (j) {
                case WALL:
                    cout << WALL_STYLE << j << RESET;
                    break;
                case PATH:
                    cout << PATH_STYLE << j << RESET;
                    break;
                case START:
                    cout << START_STYLE << j << RESET;
                    break;
                case END:
                    cout << END_STYLE << j << RESET;
                    break;
                case OPEN_NODE:
                    cout << OPEN_NODE_STYLE << j << RESET;
                    break;
                default:
                    cout << j;
                    break;
            }
        }
        cout << "" << endl;
    }
}

void Maze::print_stats(const string &algorithm) const {
    cout << "---------------------------------------\n"
         << algorithm << "\n---------------------------------------\n"
         << START_STYLE << START << RESET << " Start\n"
         << END_STYLE << END << RESET << " End\n"
         << OPEN_NODE_STYLE << OPEN_NODE << RESET << " Opened node\n"
         << PATH_STYLE << PATH << RESET << " Path\n"
         << WALL_STYLE << WALL << RESET << " Wall\n"
                    "space Fresh node\n"
                    "---------------------------------------\n";
    cout << "Nodes expanded: " << nodes_expanded << endl;
    cout << "Path length: " << path_length << endl;
}

/// There are three restore_path function for specific inputs. It restores the path rom 2D array,
/// where is stored the pair of coordinates of its predecessor

void Maze::restore_path(vector<vector<pair<int, int> > > &arr) {
    pair<int, int> current = finish;
    while (maze.at(arr.at(current.first).at(current.second).first).at(
            arr.at(current.first).at(current.second).second) != START) {
        maze.at(arr.at(current.first).at(current.second).first).at(
                arr.at(current.first).at(current.second).second) = PATH;
        system("clear");
        print_maze();
        usleep(DELAY);
        path_length++;
        current = arr.at(current.first).at(current.second);
    }
}

/*char return_curve(const pair<int, int> &prev_prev, const pair<int, int> &current) {
    if (prev_prev.first + 1 == current.first && prev_prev.second + 1 == current.second) {
        return char(192);
    }
    if (prev_prev.first - 1 == current.first && prev_prev.second + 1 == current.second) {
        return char(218);
    }
    if (prev_prev.first - 1 == current.first && prev_prev.second - 1 == current.second) {
        return char(191);
    }
    if (prev_prev.first + 1 == current.first && prev_prev.second - 1 == current.second) {
        return char(217);
    }
    return '_';
}*/


/*void Maze::restore_path(vector<vector<pair<pair<double,double>, pair<int, int> > > > &arr) {
    pair<int, int> current = finish;
    pair<int, int> prev = finish;
    pair<int, int> prev_prev = finish;
    while(maze.at(arr.at(current.first).at(current.second).second.first).at(arr.at(current.first).at(current.second).second.second) != 'S') {
        if (prev.first-1 == current.first || prev.first+1 == current.first || prev == current){
            maze.at(arr.at(current.first).at(current.second).second.first).at(arr.at(current.first).at(current.second).second.second) = '|';
        }
        if (prev.second-1 == current.second || prev.second+1 == current.second || prev == current){
            maze.at(arr.at(current.first).at(current.second).second.first).at(arr.at(current.first).at(current.second).second.second) = '-';
        }
        if (return_curve(prev_prev, current) != '_'){
            maze.at(arr.at(prev_prev.first).at(prev_prev.second).second.first)
                .at(arr.at(prev_prev.first).at(prev_prev.second).second.second) = return_curve(prev_prev, current);
        }
        system("clear");
        print_maze();
        usleep(6000);
        path_length++;
        prev_prev = prev;
        prev = current;
        current = arr.at(current.first).at(current.second).second;
    }
}*/

void Maze::restore_path(vector<vector<pair<pair<double, double>, pair<int, int> > > > &arr) {
    pair<int, int> current = finish;
    while (maze.at(arr.at(current.first).at(current.second).second.first).at(
            arr.at(current.first).at(current.second).second.second) != START) {
        maze.at(arr.at(current.first).at(current.second).second.first).at(
                arr.at(current.first).at(current.second).second.second) = PATH;
        system("clear");
        print_maze();
        usleep(DELAY);
        path_length++;
        current = arr.at(current.first).at(current.second).second;
    }
}

void Maze::restore_path(vector<vector<pair<double, pair<int, int> > > > &arr) {
    pair<int, int> current = finish;
    while (maze.at(arr.at(current.first).at(current.second).second.first).at(
            arr.at(current.first).at(current.second).second.second) != START) {
        maze.at(arr.at(current.first).at(current.second).second.first).at(
                arr.at(current.first).at(current.second).second.second) = PATH;
        system("clear");
        print_maze();
        usleep(DELAY);
        path_length++;
        current = arr.at(current.first).at(current.second).second;
    }
}

bool Maze::is_not_visited(const pair<int, int> &node) {
    if (maze.at(node.first).at(node.second) != OPEN_NODE &&
        maze.at(node.first).at(node.second) != WALL &&
        maze.at(node.first).at(node.second) != START) {
        nodes_expanded++;
        return true;
    }
    return false;
}

bool Maze::is_finish(const pair<int, int> &node) {
    return (maze.at(node.first).at(node.second) == END);
}

void Maze::end_the_algo(const string &algorithm) const {
    usleep(DELAY);
    system("clear");
    print_maze();
    print_stats(algorithm);
}

void Maze::flip_maze() {
    pair<int, int> tmp = start;
    start.first = tmp.second;
    start.second = tmp.first;
    tmp = finish;
    finish.first = tmp.second;
    finish.second = tmp.first;
    //storage
    vector<vector<char> > tmp_maze;
    tmp_maze.reserve(maze.size());
    for (auto &line: maze) {
        tmp_maze.emplace_back(line);
    }
    maze.clear();
    for (size_t i = 0; i < tmp_maze.at(0).size(); i++) {
        maze.resize(i + 1);
        for (auto &line: tmp_maze) {
            maze.at(i).emplace_back(line.at(i));
        }
    }
}

double Maze::calculate_metric(const pair<int, int> &node) const {
    return ((double) sqrt(
            pow(node.first - finish.first, 2) + pow(node.second - finish.second, 2))); //Euklidovská metrika
    //return ((double) abs(node.first - finish.first) + abs(node.second - finish.second)); //Manhattanská metrika
    /*return ((double)(abs(node.first - finish.first) / (double)((abs(node.first) + abs(finish.first))))
        + (double)(abs(node.second - finish.second) / (double)((abs(node.second) + abs(finish.second))))); //Canberrská metrika*/
}







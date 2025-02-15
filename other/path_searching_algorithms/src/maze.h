#include <list>
#include <array>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fstream>
#include <cstdlib>
using namespace std;

#define WALL 'X'
#define START 'S'
#define END 'F'
#define OPEN_NODE '#'
#define PATH 'o'
#define RESET "\033[0;0m"
#define WALL_STYLE "\033[37;47m"
#define END_STYLE "\033[33;43m"
#define START_STYLE "\033[33;43m"
#define OPEN_NODE_STYLE "\033[34m"
#define PATH_STYLE "\033[32;42m"
#define DELAY 500

class Maze {
public:
    //constructor, destructor
    Maze();

    ~Maze();

    //searching algorithms
    void BFS();

    void DFS();

    void random_search();

    void A_star();

    void greedy_search();

    //restoring path
    void restore_path(vector<vector<pair<int, int> > > &arr);

    void restore_path(vector<vector<pair<double, pair<int, int> > > > &arr);

    void restore_path(vector<vector<pair<pair<double, double>, pair<int, int> > > > &arr);

    //helper functions
    double calculate_metric(const pair<int, int> &node) const;

    bool is_not_visited(const pair<int, int> &node);

    bool is_finish(const pair<int, int> &node);

    void end_the_algo(const string & algorithm) const;

    //handling maps
    bool load_maze(const string &fileName);

    static pair<int, int> load_start_finish(const string &line);

    bool initialize_maze();

    void print_maze() const;

    void print_stats(const string & algorithm) const;

    void flip_maze();

private:
    //storage
    pair<int, int> start;
    pair<int, int> finish;
    int path_length = 1;
    int nodes_expanded = 0;
    vector<vector<char> > maze;
    vector<pair<int, int>> directions = {{1,  0},
                                         {0,  1},
                                         {-1, 0},
                                         {0,  -1}};
};


inline pair<int, int> operator+(const pair<int, int> &lhs, const pair<int, int> &rhs) {
    return {rhs.first + lhs.first, rhs.second + lhs.second};
}

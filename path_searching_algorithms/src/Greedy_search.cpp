#include "maze.h"

struct compare_greedy {
    bool operator()(const pair<double, pair<int, int> > &a, const pair<double, pair<int, int> > &b) {
        return a.first > b.first;
    }
};


void Maze::greedy_search() {
    vector<vector<pair<double, pair<int, int> > > > predecessor;
    predecessor.resize(maze.size());
    for (auto &i: predecessor) {
        i.resize(maze.at(0).size());
    }
    priority_queue<pair<double, pair<int, int> >,
            vector<pair<double, pair<int, int> > >,
            compare_greedy> priory_queue;
    pair<double, pair<int, int> > current = make_pair(calculate_metric(start), start);
    priory_queue.emplace(current);
    while (!priory_queue.empty()) {
        priory_queue.pop();
        for (auto direction: directions) {
            if (is_not_visited(current.second + direction)) {
                if (is_finish(current.second + direction)) {
                    predecessor.at((current.second + direction).first).at(
                            (current.second + direction).second) = current;
                    restore_path(predecessor);
                    end_the_algo("Greedy search");
                    return;
                }
                maze.at((current.second + direction).first).at((current.second + direction).second) = OPEN_NODE;
                priory_queue.emplace(calculate_metric(current.second + direction), (current.second + direction));
                predecessor.at((current.second + direction).first).at((current.second + direction).second) = current;
            }
        }
        current = priory_queue.top();
        if (priory_queue.empty()) {
            cout << "Sorry, I couln't find the path" << endl;
            return;
        }
        system("clear");
        print_maze();
        usleep(DELAY);
    }
}

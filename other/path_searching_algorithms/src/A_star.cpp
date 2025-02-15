#include "maze.h"


struct compare_A_star {
    bool operator()(const pair<pair<double, double>, pair<int, int> > &a,
                    const pair<pair<double, double>, pair<int, int> > &b) {
        // counting the f_function and compare node in priory que by it
        return (a.first.first + a.first.second) > (b.first.first + b.first.second);
    }
};


void Maze::A_star() {
    //a map, where is a predecessor with its g-func and h-func
    vector<vector<pair<pair<double, double>, pair<int, int> > > > predecessors;
    predecessors.resize(maze.size());
    for (auto &i: predecessors) {
        i.resize(maze.at(0).size());
    }
    //priory que of nodes with their g-func(distance so far) and h-func(heuristic), sorted by f-func
    priority_queue<pair<pair<double, double>, pair<int, int> >,
            vector<pair<pair<double, double>, pair<int, int> > >,
            compare_A_star> prior_queue;
    pair<pair<double, double>, pair<int, int> > current = make_pair(make_pair(0, calculate_metric(start)), start);
    prior_queue.emplace(current);
    while (!prior_queue.empty()) {
        prior_queue.pop();
        for (auto direction: directions) {
            if (is_not_visited(current.second + direction)) {
                if (is_finish(current.second + direction)) {
                    predecessors.at((current.second + direction).first).at(
                            (current.second + direction).second) = current;
                    restore_path(predecessors);
                    end_the_algo("A *");
                    return;
                }
                maze.at((current.second + direction).first).at((current.second + direction).second) = OPEN_NODE;
                //add new node to queue
                prior_queue.emplace(
                        make_pair(current.first.first + 1, calculate_metric(current.second + direction)),
                        (current.second + direction));
                predecessors.at((current.second + direction).first).at((current.second + direction).second) = current;
            }
        }
        if (prior_queue.empty()) {
            cout << "Sorry, I couln't find the path" << endl;
            return;
        }
        current = prior_queue.top();
        system("clear");
        print_maze();
        usleep(DELAY);
    }
}




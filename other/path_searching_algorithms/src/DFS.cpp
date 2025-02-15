#include "maze.h"

void Maze::DFS() {
    stack<pair<int, int> > stack;
    set<pair<int, int >> visited;
    vector<vector<pair<int, int> > > predecessors;
    predecessors.resize(maze.size());
    for (auto &i: predecessors) {
        i.resize(maze.at(0).size());
    }
    stack.emplace(start);
    visited.insert(start);
    pair<int, int> current;
    while (!stack.empty()) {
        current = stack.top();
        stack.pop();
        for (auto direction: directions) {
            if (maze.at((current + direction).first).at((current + direction).second) != WALL &&
                visited.find(current + direction) == visited.end()) {
                if (is_finish(current + direction)) {
                    predecessors.at((current + direction).first).at((current + direction).second) = current;
                    restore_path(predecessors);
                    end_the_algo("DFS");
                    return;
                }
                visited.insert(current + direction);
                stack.emplace(current + direction);
                predecessors.at((current + direction).first).at((current + direction).second) = current;
            }
        }
        if (maze.at(current.first).at(current.second) != START) {
            maze.at(current.first).at(current.second) = OPEN_NODE;
        }
        nodes_expanded++;
        if (stack.empty()) {
            cout << "Sorry, I couln't find the path" << endl;
            return;
        }
        system("clear");
        print_maze();
        usleep(DELAY);
    }
}
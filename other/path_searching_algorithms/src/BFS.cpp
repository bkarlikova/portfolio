#include "maze.h"

void Maze::BFS() {
    queue<pair<int, int> > q;
    q.emplace(start);
    vector<vector<pair<int, int> > > predecessors;
    predecessors.resize(maze.size());
    for (size_t i = 0; i < predecessors.size(); i++) {
        predecessors.at(i).resize(maze.at(i).size());
    }
    pair<int, int> current = start;
    while (!q.empty()) {
        q.pop();
        for (auto direction: directions) {
            if (is_not_visited(current + direction)) {
                if (is_finish(current + direction)) {
                    predecessors.at((current + direction).first).at((current + direction).second) = current;
                    restore_path(predecessors);
                    end_the_algo("BFS");
                    return;
                }
                maze.at((current + direction).first).at((current + direction).second) = OPEN_NODE;
                q.emplace((current + direction));
                predecessors.at((current + direction).first).at((current + direction).second) = current;
            }
        }
        if (q.empty()) {
            system("clear");
            cout << "Sorry, I couln't find the path" << endl;
            return;
        }
        current = q.front();
        system("clear");
        print_maze();
        usleep(DELAY);
    }
}
#include "maze.h"

void Maze::random_search() {
    vector<vector<pair<int, int> > > predecessors;
    predecessors.resize(maze.size());
    for (auto &i: predecessors) {
        i.resize(maze.at(0).size());
    }
    vector<pair<int, int> > opened;
    pair<int, int> current;
    opened.emplace_back(start);
    while (!opened.empty()) {
        int num = rand() % opened.size();
        current = opened.at(num);
        opened.erase(opened.begin() + num);
        for (auto direction: directions) {
            if (is_not_visited(current + direction)) {
                if (is_finish(current + direction)) {
                    predecessors.at((current + direction).first).at((current + direction).second) = current;
                    restore_path(predecessors);
                    end_the_algo("Random search");
                    return;
                }
                maze.at((current + direction).first).at((current + direction).second) = OPEN_NODE;
                opened.emplace_back((current + direction).first, (current + direction).second);
                predecessors.at((current + direction).first).at((current + direction).second) = current;
            }
        }
        if (opened.empty()) {
            cout << "Sorry, I couln't find the path" << endl;
            return;
        }
        system("clear");
        print_maze();
        usleep(DELAY);
    }
}

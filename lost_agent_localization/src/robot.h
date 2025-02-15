#include <vector>
#include <set>
#include <map>
#include <stack>
#include <string>
#include <queue>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <utility>
#include <stdexcept>
#include <unistd.h>
#include "cave.h"

using namespace std;

class Robot {
    pair<int, int> _originalPosition;
    pair<int, int> _currentPosition;
    Sensor _sensor;
    Cave _cave;
    pair<int, int> _relativePosition = {0,0};
    vector<pair<int, int>> _moves = {{1,  0},
                                     {0,  1},
                                     {-1, 0},
                                     {0,  -1}};
    size_t _numOfSteps = 0;
    map<size_t, set<pair<int, int> > > _hashTable = {};
    nanoseconds _searchTime = {};
public:
    Robot() = delete;

    Robot(pair<int, int> position, const string &filename, size_t mode, int range = INT32_MAX);

    ~Robot() = default;

    [[nodiscard]] pair<int, int> getCurrentPosition() const;

    pair<int, int> moveForward(const set<pair<int, int>> &prevMoves);

    void updateCurrentPosition(const pair<int, int> &move);

    set<pair<int, int> > findPosition();

    void printStats(const set<pair<int, int> > &results);
};





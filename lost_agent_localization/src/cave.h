#include <vector>
#include <set>
#include <map>
#include <stack>
#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <utility>
#include <stdexcept>
#include <chrono>

#define LIDAR 1
#define KERNEL 2

#define WALL 'X'
#define NOT_WALL ' '
#define START 's'
#define NOT_WALL_IN_MAP '.'
#define VISITED 'v'
#define CURRENT_POSSIBLE 'p'
#define PATH '*'

#define RESET "\033[0;0m"
#define WALL_STYLE "\033[37;47m"
#define CURRENT_POSSIBLE_STYLE "\033[36;46m"
#define PATH_STYLE "\033[36;0m"
#define START_STYLE "\033[33;43m"
#define VISITED_STYLE "\033[32;42m"
#define DELAY 500000

#define TIME_UNIT microseconds



#ifdef PRINT
#undef PRINT
#define PRINT(x) x
#else
#define PRINT(x)
#endif


using namespace std;
using namespace std::chrono;

struct Sensor{
    int mode = LIDAR;
    int range = INT64_MAX;
    Sensor() = default;
    Sensor(int m, int r);
};

class Cave {
    size_t _height = 0;
    size_t _width = 0;
    vector<vector<char> > mapToPrint;
public:
    TIME_UNIT timeOfMapPreparation = {};
    vector<vector<char> > caveMap = {};
    Cave() = default;
    ~Cave() = default;

    Cave(const string &fileName);

    void loadMap(const string &fileName);

    void printMap(const set<pair<int, int>> &currentPosition);

    bool isEdge(const pair<int, int> &pos) const;

    bool outOfBounds(const pair<int, int> &pos) const;

    set<pair<int, int>> setPossibleStartsAndPaths(const vector<pair<int, int> > &moves, const set<pair<int, int>> &possibleStarts);

    void printInfo() const;

    pair<int, int> getSize() const;

    [[nodiscard]] size_t getHash(const pair<int, int> &position, const Sensor & s) const;

    [[nodiscard]] map<size_t, set<pair<int, int> > > setHashTable(const Sensor & s);
};

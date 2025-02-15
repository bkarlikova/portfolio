#include "cave.h"

template<typename T_>
pair<T_, T_> operator+(pair<T_, T_> a, pair<T_, T_> b) {
    return make_pair(a.first + b.first, a.second + b.second);
}

template<typename T_>
pair<T_, T_> operator-(pair<T_, T_> a, pair<T_, T_> b) {
    return make_pair(a.first - b.first, a.second - b.second);
}

template<typename T_>
pair<T_, T_> operator-(pair<T_, T_> a) {
    return make_pair(-a.first, -a.second);
}

Cave::Cave(const string & fileName){
    loadMap(fileName);
    mapToPrint = caveMap;
}
void Cave::loadMap(const string &fileName) {
    ifstream ifs;
    ifs.open(fileName, ifstream::in);
    if (!ifs.is_open()) {
        throw invalid_argument("invalid file or filename");
    }
    string line;
    size_t i = 0;
    while (getline(ifs, line)) {
        caveMap.resize(i + 1);
        stringstream iss(line);
        char tmp;
        while (iss >> noskipws >> tmp) {
            if (tmp == NOT_WALL_IN_MAP) {
                caveMap.at(i).emplace_back(NOT_WALL);
            } else {
                caveMap.at(i).emplace_back(WALL);
            }
        }
        i++;
    }
    _height = caveMap.size();
    _width = caveMap.at(0).size();
}


void Cave::printMap(const set<pair<int, int>> &current_position) {
    for (size_t i = 0; i < mapToPrint.size(); i++) {
        for (size_t j = 0; j < mapToPrint.at(i).size(); j++) {
            if (current_position.end() != current_position.find(make_pair(i, j))) {
                mapToPrint.at(i).at(j) = VISITED;
                cout << CURRENT_POSSIBLE_STYLE << mapToPrint.at(i).at(j) << RESET;
                continue;
            }
            switch (mapToPrint.at(i).at(j)) {
                case WALL:
                    cout << WALL_STYLE << mapToPrint.at(i).at(j)<< RESET;
                    break;
                case NOT_WALL:
                    cout << mapToPrint.at(i).at(j);
                    break;
                case VISITED:
                    cout << VISITED_STYLE << mapToPrint.at(i).at(j) << RESET;
                    break;
                case START:
                    cout << START_STYLE << mapToPrint.at(i).at(j) << RESET;
                    break;
                case PATH:
                    cout << PATH_STYLE << mapToPrint.at(i).at(j) << RESET;
                    break;
                default:
                    cout << mapToPrint.at(i).at(j);
                    break;
            }
        }
        cout << "" << endl;
    }
}

bool Cave::isEdge(const pair<int, int> &pos)const{
    return pos.first == 0 || pos.first == int(_height - 1) || pos.second == 0 || pos.second == int(_width - 1);
}

bool Cave::outOfBounds(const pair<int, int> &pos)const{
    return pos.first >= int(_height) || pos.first < 0 || pos.second >= int(_width) || pos.second < 0;
}

set<pair<int, int>> Cave::setPossibleStartsAndPaths(const vector<pair<int, int> > &moves, const set<pair<int, int>> &starts){
    for (auto pos : starts){
        mapToPrint.at(pos.first).at(pos.second) = START;
        vector<pair<int, int> > tmpMoves = moves;
        while(!tmpMoves.empty()){
            pos = pos + tmpMoves.front();
            mapToPrint.at(pos.first).at(pos.second) = PATH;
            tmpMoves.erase(tmpMoves.begin());
        }
    }
    return starts;
}

void Cave::printInfo() const {
    PRINT(cout << "Wall: " << WALL_STYLE << WALL << RESET << endl;)
    PRINT(cout << "Not wall: " << "space" << endl;)
    PRINT(cout << "Visited: " << VISITED_STYLE << VISITED << RESET << endl;)
    PRINT(cout << "Start: " << START_STYLE << START << RESET << endl;)
    cout << "Size of map: " << _height << " * " << _width << " = " << _height*_width << endl;
}

pair<int, int> Cave::getSize() const {
    return make_pair(int(_height), int(_width));
}


size_t Cave::getHash(const pair<int, int> &position, const Sensor & s) const {
    if (caveMap.at(position.first).at(position.second) == WALL) {
        return SIZE_T_MAX;
    }
    size_t hash = 0;
    if (s.mode == LIDAR) {
        uint16_t up = 0, down = 0, left = 0, right = 0;
        for (int posInColumn = position.first; posInColumn > max(position.first - s.range, 0); posInColumn--) {
            if (caveMap.at(posInColumn).at(position.second) == WALL) {
                break;
            }
            up++;
        }
        for (int posInColumn = position.first; posInColumn < min(position.first + s.range, getSize().first); posInColumn++) {
            if (caveMap.at(posInColumn).at(position.second) == WALL) {
                break;
            }
            down++;
        }
        for (int posInRow = position.second; posInRow > max(position.second - s.range, 0); posInRow--) {
            if (caveMap.at(position.first).at(posInRow) == WALL) {
                break;
            }
            left++;
        }
        for (int posInRow = position.second; posInRow < min(position.second + s.range, getSize().second); posInRow++) {
            if (caveMap.at(position.first).at(posInRow) == WALL) {
                break;
            }
            right++;
        }
        vector<uint16_t> nums = {up, down, left, right};
        for (auto num : nums){
            hash <<= 16;
            hash += num;
        }
        return hash;
    } else if (s.mode == KERNEL) {

        for (int i = -s.range; i <= s.range; i++) {
            for (int j = -s.range; j <= s.range; j++) {
                hash <<= 1;
                if (outOfBounds(make_pair(position.first + i, position.second + j))) {
                    hash += 0;
                } else {
                    hash += int(caveMap.at(position.first + i).at(position.second + j) == NOT_WALL);
                }
            }
        }
        return hash;
    }
    return 0;
}
map<size_t, set<pair<int, int> > > Cave::setHashTable(const Sensor & s) {
    auto start = high_resolution_clock::now();
    map<size_t, set<pair<int, int> > > table;
    for (size_t i = 0; i < caveMap.size(); i++) {
        for (size_t j = 0; j < caveMap.at(i).size(); j++) {
            if (caveMap.at(i).at(j) == NOT_WALL) {
                size_t hash = getHash(make_pair(i, j), s);
                table[hash].emplace(make_pair(i, j));
            }
        }
    }
    auto end = high_resolution_clock::now();
    timeOfMapPreparation = duration_cast<TIME_UNIT>(end - start);
    return table;
}


Sensor::Sensor(int m, int r) : mode(m), range(r) {
    if (mode == KERNEL) {
        if (range % 2 == 0)
            throw invalid_argument("invalid size of kernel, I must be even number");
        if (range * range > 64) {
            throw invalid_argument("invalid size of kernel, the square of its size must be less than 64");
        }
        range /= 2;
    }
}
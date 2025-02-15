#include "robot.h"

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

Robot::Robot(pair<int, int> position, const string &filename, size_t mode, int range) {
    _originalPosition = std::move(position);
    _currentPosition = std::move(position);
    _sensor = Sensor(mode, range);
    _cave = filename;
    if (_cave.outOfBounds(_originalPosition) ||
        _cave.caveMap.at(_originalPosition.first).at(_originalPosition.second) == WALL) {
        throw invalid_argument("invalid start position");
    }
    _hashTable = _cave.setHashTable(_sensor);
}

pair<int, int> Robot::getCurrentPosition() const {
    return _currentPosition;
}

pair<int, int> Robot::moveForward(const set<pair<int, int> > &relativePositions) {
    for (auto move : _moves) {
        if (relativePositions.find(_relativePosition + move) == relativePositions.end() &&
            _cave.caveMap.at(_currentPosition.first + move.first).at(_currentPosition.second + move.second) != WALL) {
            _currentPosition = _currentPosition + move;
            _relativePosition = _relativePosition + move;
            return move;
        }
    }
    return {0,0};
}

void Robot::updateCurrentPosition(const pair<int, int> &movement) {
    _currentPosition = _currentPosition + movement;
    _relativePosition = _relativePosition + movement;
}

set<pair<int, int> > Robot::findPosition() {
    auto start = high_resolution_clock::now();
    set<pair<int, int> > possibleStartPositions = _hashTable.at(_cave.getHash(getCurrentPosition(), _sensor));
    set<pair<int, int> > relativeVisitedPositions = {_relativePosition};
    vector<pair<int, int> > moves;
    pair<int, int>  absoluteMove = {0,0};
    PRINT(_cave.printMap(possibleStartPositions);)
    while (possibleStartPositions.size() != 1) {
        pair<int, int> currentMove = moveForward(relativeVisitedPositions);
        relativeVisitedPositions.insert(_relativePosition);
        absoluteMove = absoluteMove + currentMove;
        pair<int, int> tmpMove = currentMove;
        vector<pair<int, int> > tmpMoves = moves;
        //vector<pair<int, int> > backMoves;
        while (tmpMove == make_pair(0,0)){
            updateCurrentPosition(-tmpMoves.back());
            //backMoves.push_back(-tmpMoves.back());
            absoluteMove = absoluteMove + -tmpMoves.back();
            tmpMoves.pop_back();
            currentMove = currentMove - moves.back();
            tmpMove = moveForward(relativeVisitedPositions);
            currentMove = tmpMove + currentMove;
            if (tmpMove != make_pair(0,0)){
                absoluteMove = absoluteMove + tmpMove;
                //move(moves.begin(), moves.end(), std::back_inserter(backMoves));
                moves.emplace_back(tmpMove);
            }
            else if (tmpMove == make_pair(0,0) && possibleStartPositions.find(_currentPosition) != possibleStartPositions.end()) {
                _numOfSteps = moves.size();
                auto end = high_resolution_clock::now();
                _searchTime = duration_cast<TIME_UNIT >(end - start);
                return _cave.setPossibleStartsAndPaths(moves, possibleStartPositions);
            }
        }
        size_t hash = _cave.getHash(getCurrentPosition(), _sensor);
        moves.emplace_back(currentMove);
        set<pair<int, int> > currentPossiblePositions = _hashTable.at(hash);
        auto it = possibleStartPositions.begin();
        while (it != possibleStartPositions.end()){
            auto currentIt = it++;
            if (currentPossiblePositions.end() == currentPossiblePositions.find(*currentIt + absoluteMove)){
                possibleStartPositions.erase(currentIt);
            }
        }
        PRINT(system("clear");)
        PRINT(_cave.printMap(possibleStartPositions);)
        PRINT(usleep(DELAY);)
    }
    _numOfSteps = moves.size();
    auto end = high_resolution_clock::now();
    _searchTime = duration_cast<TIME_UNIT >(end - start);
    return _cave.setPossibleStartsAndPaths(moves, possibleStartPositions);
}




void Robot::printStats(const set<pair<int, int> > &results){
    system("clear");
    PRINT(_cave.printMap({});)
    _cave.printInfo();
    cout << "----------------------------------------------" << endl;
    cout << "Duration of map preparation: " << _cave.timeOfMapPreparation.count() << " microseconds" << endl;
    cout << "Duration of search: " << _searchTime.count() << " microseconds" << endl;
    cout << "Number of steps: " << _numOfSteps << endl;
    for (auto res : results){
        cout << "Found position " << res.first << ", " << res.second << endl;
    }
    cout << "----------------------------------------------" << endl;
}




#include "maze.h"

using namespace std;
namespace fs = std::__fs::filesystem;

int main() {
    Maze maze;

    for (const auto & entry : fs::directory_iterator("dataset/")){
        fs::path tmp(entry.path());
        std::cout << tmp.filename() << std::endl;
    }
    string file_name;
    while(!maze.load_maze("dataset/" + file_name)){
        cout << "Write name of the file:" << endl;
        cin >> file_name;
    }
    /*system("clear");
    maze.print_maze();
    usleep(60000);
    system("clear");*/

    try {
        maze.initialize_maze();
    }
    catch (exception &ex) {
        cout << ex.what() << endl;
        return 0;
    }
    catch (...) {
        cout << "Uncaught exception" << endl;
        return 0;
    }
    int algorithm;
    do{
        cout << "Write number of algorithm:\n"
                "1. BFS\n"
                "2. DFS\n"
                "3. Greedy search\n"
                "4. Random search\n"
                "5. A *\n"<< endl;
        cin >> algorithm;
        switch (algorithm) {
            case 1:
                maze.BFS();
                return 0;
            case 2:
                maze.DFS();
                return 0;
            case 3:
                maze.greedy_search();
                return 0;
            case 4:
                maze.random_search();
                return 0;
            case 5:
                maze.A_star();
                return 0;
            default:
                system("clear");
                cin.clear();
                cin.ignore(INT8_MAX, '\n');
                cout << "Invalid number, try again" << endl;
                break;
        }
    }
    while(!isnumber(algorithm) || algorithm > 6 || algorithm < 1);

}

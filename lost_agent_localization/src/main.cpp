#include "robot.h"
#include <utility>
#include <stdexcept>


//pair<int, int > position = make_pair(1, 2);
//Robot r(position, "../data/map7.txt", 1, 2);
//
//pair<int, int > position = make_pair(38, 31);
//Robot r(position, "../data/map5.txt", 1, 2);
//
//pair<int, int > position = make_pair(38, 31);
//Robot r(position, "../data/map5.txt", 2, 3);

//pair<int, int > position = make_pair(394, 169);
//Robot r(position, "../data/map1.txt", 2, 3);

//pair<int, int > position = make_pair(11,11);
//Robot r(position, "data/map6.txt", 1, 2);

//pair<int, int > position = make_pair(1,3);
//Robot r(position, "data/map6.txt", 1, 3);

//pair<int, int > position = make_pair(1,3);
//Robot r(position, "data/map6.txt", 2, 3);

int main(){
    system("clear");
    try{
        pair<int, int > position = make_pair(38, 31);
        Robot r(position, "data/map5.txt", 1, 2);
        //pair<int, int > position = make_pair(1,3);
        //Robot r(position, "data/map6.txt", 1, 3);
        r.printStats(r.findPosition());
        cout << "Original position: " << position.first << ", " << position.second << endl;

//        pair<int, int > position = make_pair(38, 31);
//        Robot r(position, "data/map5.txt", 1, 2);
//        r.printStats(r.findPosition());
//        cout << "Original position: " << position.first << ", " << position.second << endl;

//        pair<int, int > position = make_pair(394, 169);
//        Robot r1(position, "../data/map1.txt", 1, 1);
//        cout << "Original position: " << position.first << ", " << position.second << endl;
//        r1.printStats(r1.findPosition());
//        Robot r2(position, "../data/map1.txt", 1, 3);
//        cout << "Original position: " << position.first << ", " << position.second << endl;
//        r2.printStats(r2.findPosition());
//        Robot r3(position, "../data/map1.txt", 1, 7);
//        cout << "Original position: " << position.first << ", " << position.second << endl;
//        r3.printStats(r3.findPosition());
//        Robot r4(position, "../data/map1.txt", 1, 10);
//        cout << "Original position: " << position.first << ", " << position.second << endl;
//        r4.printStats(r4.findPosition());
//        Robot r5(position, "../data/map1.txt", 1, INT16_MAX);
//        cout << "Original position: " << position.first << ", " << position.second << endl;
//        r5.printStats(r5.findPosition());
//        Robot r6(position, "../data/map1.txt", 2, 3);
//        cout << "Original position: " << position.first << ", " << position.second << endl;
//        r6.printStats(r6.findPosition());
//        Robot r7(position, "../data/map1.txt", 2, 5);
//        cout << "Original position: " << position.first << ", " << position.second << endl;
//        r7.printStats(r7.findPosition());
//        Robot r8(position, "../data/map1.txt", 2, 7);
//        cout << "Original position: " << position.first << ", " << position.second << endl;
//        r8.printStats(r8.findPosition());
//        cout << "Original position: " << position.first << ", " << position.second << endl;
    }
    catch (invalid_argument const &e){
        cout << "EXCEPTION: " << e.what() << endl;
    }
    catch (logic_error const &e){
        cout << "EXCEPTION: " << e.what() << endl;
    }
    catch (...){
        cout << "Unrecognized exception" << endl;
    }
    return 0;
}
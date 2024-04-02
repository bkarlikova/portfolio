#include "TSP.h"


int main(){
    TSP problem;
    vector<int> best_result;
    problem.load_data("distances-10.csv");
    best_result = problem.hill_climbing(300);
    cout << "----------------------------------------------------\n"
            "Final result:" << endl;
    problem.print_result(best_result);
    return EXIT_SUCCESS;
}
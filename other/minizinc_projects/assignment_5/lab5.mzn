include "globals.mzn";
int: n;
int: n_commercial;
int: n_residential;
array[1..n+1] of int: point_distribution;
array[1..n, 1..n] of var int: lots; % the grid of n*n lots
array[1..n] of var int: row_points; % points per row
array[1..n] of var int: col_points; % points per column

% Each location in the grid can either be a 0 (commercial lot) or a 1 (residential lot).
constraint forall(i in 1..n, j in 1..n) (lots[i,j] in 0..1);

% Ensures that there are n_commercial commercial buildings and n_residential residential buildings,
% since we only have two types of buildings constraining on n_commercial is enough.
constraint sum(i, j in 1..n)(lots[i, j] = 0) = n_commercial; % /\ sum(i, j in 1..n)(lots[i, j] = 1) = n_residential;
 
% Sums the points per row and col in lots into two arrays.
constraint forall(i in 1..n) (row_points[i] = sum(j in 1..n)(lots[i, j]));
constraint forall(j in 1..n) (col_points[j] = sum(i in 1..n)(lots[i, j]));

% Finds the total points by indexing point_distribution with the point sum of a row or col + 1. 
% + 1 because lowest sum in a row or col starts at zero but index of point_distribution starts at 1. 
var int: points = sum([point_distribution[row_points[i]+1]| i in 1..n] ++ [point_distribution[col_points[i]+1]| i in 1..n]);

% lex2 is used to break symmetry, it requires adjacent rows and adjacent columns in the array x to be lexicographically ordered.
constraint lex2(lots);

% DFS search, want to maximize the total points.
solve :: int_search(lots, first_fail, indomain_min) maximize points;

output["Lot placement:\n\n"];
output[if j < n then show(lots[i, j]) ++ " " else show(lots[i, j]) ++ "\n" endif | i in 1..n, j in 1..n];

output [
    "\nPoints per row: ", show(row_points),
    "\nPoints per column: ", show(col_points),
    "\nTotal points: ", show(points),
];

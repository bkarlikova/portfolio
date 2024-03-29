include "globals.mzn";
%set given variables
int: n;
int: n_commercial;
int: n_residential;
array[1..n+1] of int: point_distribution;

% set grid
array[1..n, 1..n] of var 0..1: grid;
% set arrays of points for each row and column
array[1..n] of var 0..n: row_cost = [ sum(row(grid, i)) | i in 1..n];
array[1..n] of var 0..n: column_cost = [ sum(col(grid, i)) | i in 1..n];

% set cost variable
var int: cost;

% considering residential slots == 1 and shops == 0
constraint sum(grid) == n_residential;

% calculate the cost of given grid
constraint cost = sum(i in 1..n)(point_distribution[row_cost[i] + 1]) 
                + sum(i in 1..n)(point_distribution[column_cost[i] + 1]);
                
% breaking symmetry by vertical and diagonal axis
constraint forall(i in 1..n-1)(lex_lesseq(row(grid, i), row(grid, i+1)));
% breaking symmetry by horisontal and diagonal axis
constraint forall(i in 1..n-1)(lex_lesseq(col(grid, i), col(grid, i+1)));
% these symmetry breaking constraint will put the rows and colum with the least lexicografical order to the left top od the grid.

solve maximize cost;

output [ "\(grid[i,j]) " ++ 
         if j == n then "\n" else "" endif |
         i in 1..n, j in 1..n ] ++ 
        [ show(cost)]; 
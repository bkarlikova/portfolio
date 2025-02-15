include "network_flow.mzn";

% set variables from file
int: graph_size;
int: start;
int: n_dests;
array [int] of int: dests;
int: n_edges;
array[1..n_edges] of int:  cities_1;
array[1..n_edges] of int:  cities_2;
array[1..n_edges] of int:  costs;

%create new arrays for directed graph
array[1..(n_edges*2)] of int:  cost_for_all = costs ++ costs;
array[1..(n_edges*2)] of int:  cities_x = cities_1 ++ cities_2;
array[1..(n_edges*2)] of int:  cities_y = cities_2 ++ cities_1;

%set balances: start is the number of destinatinos and each destination is -1
array[1..graph_size] of int: balance = [ i : if i = start then -n_dests else if i in dests then 1 else 0 endif endif | i in 1..graph_size];

%set arcs
array[1..(n_edges*2), 1..2] of int: arcs = [ (i,j) : if j = 1 then cities_x[i] else cities_y[i] endif | i in 1..(n_edges*2), j in 1..2];

%set domain for flows
array[1..(n_edges*2)] of var 0..n_dests: flows;

var int: cost;
constraint cost = sum(i in 1..(n_edges*2))(if flows[i]!=0 then cost_for_all[i] else <> endif);

constraint network_flow(arcs, balance, flows);

solve minimize cost;
output[show(flows),"\n",show(cost)];

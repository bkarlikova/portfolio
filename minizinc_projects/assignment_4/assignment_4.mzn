include "cumulative.mzn";

int: del_add;
int: del_mul;
int: number_add;
int: number_mul;
int: n;

set of int : last;
set of int : add;
set of int : mul;

array[1..n] of set of int : dependencies;

array[1..n] of int: duration = [ i : if i in add then del_add else del_mul endif | i in 1..n];
array[1..n] of var 0..n*(max(del_add, del_mul)): start;
array[1..n] of int: requirements_add = [ i : if i in add then 1 else 0 endif | i in 1..n];
array[1..n] of int: requirements_mul = [ i : if i in mul then 1 else 0 endif | i in 1..n];
array[1..card(last)] of var 0..n*(max(del_add, del_mul)): end = [start[i] + duration[i] | i in last];

constraint forall(i in 1..n, j in dependencies[i])(start[i] + duration[i] <= start[j]);

constraint cumulative(start, duration, requirements_add, number_add);

constraint cumulative(start, duration, requirements_mul, number_mul);

solve minimize max(end);

output["Number orf clocks: \(max(end))\n", 
    "starts = ", show(start), "\n", 
     "duration = ", show(duration), "\nadd = "] 
     ++ [if requirements_add[i] == 1 then "\(start[i]), " else "" endif | i in 1..n] 
     ++ ["\nmul = "] 
     ++ [if requirements_mul[i] == 1 then "\(start[i]), " else "" endif | i in 1..n];

%output ["adder: \n"] ++ [if requirements_add[i] == 1 then "\(i) : \(start[i])\n" else "" endif | i in 1..n] ++
%      ["mul: \n"] ++ [if requirements_mul[j] == 1 then "\(j) : \(start[j])\n" else "" endif | j in 1..n] ;


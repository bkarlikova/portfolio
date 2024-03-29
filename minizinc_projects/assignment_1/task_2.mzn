include "alldifferent.mzn";

int: n;
int: n_pref;

array[1..n_pref, 1..2] of int: preferences;
array[1..n] of var 1..n: photo;

array[1..n_pref] of var 0..1000: satisfied;
var 0..100000: max_distance;

constraint alldifferent(photo);

%Task 2:
constraint forall(i in 1..n_pref)(
            satisfied[i] = abs(photo[preferences[i,1]] - photo[preferences[i,2]])
       );
       
constraint max_distance = max([satisfied[i] | i in 1..n_pref]);
       
solve minimize max_distance;

output [ "photo = "] ++ ["\(photo[i])," | i in 1..n] ++  ["\n with the smallest largest distances \(max_distance)\n",];
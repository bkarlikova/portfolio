include "alldifferent.mzn";

int: n;
int: n_pref;

array[1..n_pref, 1..2] of int: preferences;
array[1..n] of var 1..n: photo;

array[1..n_pref] of var 0..1000: satisfied;
var 0..100000: sum_satisfied;

constraint alldifferent(photo);

%Task 1:
constraint forall(i in 1..n_pref)(
            satisfied[i] = bool2int(abs(photo[preferences[i,1]] - photo[preferences[i,2]]) == 1 )
       );
       
constraint sum(i in 1..n_pref)(satisfied[i]) = sum_satisfied;
       
solve maximize sum_satisfied;

output [ "photo = "] ++ ["\(photo[i])," | i in 1..n] ++  ["\nwith \(sum_satisfied) conditions satisfied.\n",];
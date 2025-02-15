module PPA_semestral.Unification
open PPA_semestral.Types



let rec apply (bindings: Bindings) (term: Term) : Term =
    match term with
    | Variable v when bindings.ContainsKey(v) -> apply bindings bindings[v]
    | Fact f ->
        Fact
            { name = f.name
              body = List.map (apply bindings) f.body }
    | _ -> term

let rec substituteBody (binding: string * Term) (body: Term list) : Term list =
    body
    |> List.map (fun x ->
        match x with
        | Variable v when fst(binding) = v -> snd(binding)
        | Fact f ->
            Fact
                { name = f.name
                  body = substituteBody binding f.body }
        | _ -> x)
    
let substituteFact (f: FactType) (substitution: Bindings) : FactType =
    { name = f.name
      body = List.fold (fun accBody binding -> substituteBody binding accBody) f.body (Map.toList substitution) }

let substitute (t: Term) (substitution: Bindings) : Term =
    match t with
    | Variable v ->
        match substitution.TryFind(v) with
        | Some value -> value
        | None -> t
    | Fact f -> Fact (substituteFact f substitution)
    | _ -> t
    
let applyBindingToBindings (binding: string * Term) (bindings: Bindings) : Bindings =
    bindings
    |> Map.toList
    |> List.map (fun (x, y) ->
        match x, y with
        | a, Variable b when a = fst(binding) && Variable b <> snd(binding) -> (b, snd(binding))
        | a, Variable b when b = fst(binding) && Variable a <> snd(binding) -> (a, snd(binding))
        | a, Fact f ->
            (a,
             Fact
                 { name = f.name
                   body = substituteBody binding f.body })
        | _ -> (x, y))
    |> Seq.map (fun (x, y) -> x, y)
    |> Map.ofSeq
    
let applyEverythingWithEverything (bindings: Bindings) =
    Map.map (fun x y ->
        match y with
        | Fact _ -> substitute y bindings
        | _ -> y
        ) bindings 
    
let rec checkBeforeConcat (binding: string * Term) (bindings: Bindings) : bool =
    match bindings |> Map.toList with
    | [] -> true
    | (variable, Atom a) :: tail ->
        if fst(binding) = variable && not((Atom a).StrictEqualTerms(snd(binding))) then
            false
        else
            checkBeforeConcat binding (tail |> Map.ofList)
    | _ :: tail -> checkBeforeConcat binding (tail |> Map.ofList)
        

let rec occurs (v: string) (t: Term) : bool =
    match t with
    | Variable x -> x = v
    | Fact f -> List.exists (occurs v) f.body
    | _ -> false
    
let checkArityAndName (f1: FactType) (f2: FactType) =
    f1.name.Equals(f2.name) && f1.body.Length = f2.body.Length
    
let rec unifyArgs (args1: Term list) (args2: Term list) (bindings: Bindings) : ReturnType =
    match args1, args2 with
    | [], [] -> Success(bindings)
    | t1 :: rest1, t2 :: rest2 ->
        match unify t1 t2 bindings with
        | Success bindings' -> unifyArgs rest1 rest2 bindings'
        | Fail -> Fail
    | _ -> Fail

and unify (t1: Term) (t2: Term) (bindings: Bindings) : ReturnType =
    match t1, t2 with
    | Any, _ | _ , Any -> Success(bindings)
    | a, b when a = b -> Success(bindings)
    | Variable v, term
    | term, Variable v ->
        if occurs v term then
            Fail
        elif fst(bindings.TryGetValue v) && bindings[v].IsAtom && term.IsAtom && bindings[v] <> term then
            Fail
        else
            let newBindings = applyBindingToBindings (v, term) bindings
            let newValue = substitute term newBindings
            Success(applyEverythingWithEverything (newBindings.Add(v, newValue)))
    | Fact f1, Fact f2 when checkArityAndName f1 f2 ->
        unifyArgs f1.body f2.body bindings
    | _ -> Fail

let rec concatBindings (b1: Bindings) (b2:Bindings) : ReturnType =
    match Map.toList b2 with
    | [] -> Success(b1)
    | (variable,term) :: rest ->
        if checkBeforeConcat (variable, term) b1 then
            let newB1 = (applyBindingToBindings (variable, term) b1).Add(variable, term)
            concatBindings newB1 (rest |> Map.ofList)
        else
            Fail
        



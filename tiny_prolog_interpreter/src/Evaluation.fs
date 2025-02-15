module PPA_semestral.Evaluation

open System
open Microsoft.FSharp.Core
open PPA_semestral.Types

let VARIABLE_OFFSET = 7

let rec getTermAsString (t: Term) =
    match t with
    | Atom a -> match a with
                | StrAtom sa -> sa
                | NumAtom na -> string(na)
                | Nil -> "nil"
    | Any -> "_"
    | Variable v -> v[VARIABLE_OFFSET..]
    | Fact f -> f.name + "(" + (f.body
                                |> List.map (fun x -> getTermAsString x)
                                |> String.concat ",") + ")"

let printableOneSetOfBindings (oneSet: Bindings) =
    oneSet
    |> Map.toList
    |> List.map (fun (k,v) ->
        let k = k[VARIABLE_OFFSET..]
        match v with
        | Variable _ -> k + " = " + (getTermAsString v) + ", " + (getTermAsString v) + " = " + k
        | _ -> k + " = " + getTermAsString v)
    |> String.concat ",\n" 
let printableSolution(finalSolution: Map<string,Term> list) =
    finalSolution
    |> List.map printableOneSetOfBindings
    |> String.concat ";\n"
    
let getQueryAsFactType (query: Term) =
    match query with
    | Fact f -> f
    | _ -> raise (Exception "this should not have happend")
let evaluate(queryVariables: Set<String>)(finalSolutions: ReturnType list) =
    if finalSolutions.IsEmpty || finalSolutions |> List.forall (fun x -> x = Fail) then
        "False."
    else
        let outputStr = finalSolutions
                            |> List.collect (fun x -> match x with
                                                      | Success s -> [s]
                                                      | Fail -> [])
                            |> List.map (fun x -> x |> Map.filter (fun key _  -> Set.contains key queryVariables))
                            |> List.map (fun x -> x |> Map.map (fun _ value ->
                                                        match value with
                                                            | Variable v ->
                                                                match Seq.tryFind (fun y ->
                                                                         match x.TryFind(y) with
                                                                         | Some s when s.Equals(value) -> true
                                                                         | _ -> false) queryVariables with
                                                                | Some newVar -> Variable newVar
                                                                | None -> value
                                                            | _ -> value))
                            |> List.map (fun x -> x |> Map.filter (fun key value  -> not((Variable key).StrictEqualTerms(value))))
                            |> printableSolution 
                            
        if String.IsNullOrEmpty(outputStr) then
            "True."
        else
            outputStr + "."
                    

module PPA_semestral.Search

open PPA_semestral.Types
open PPA_semestral.Unification

let mutable varCounter = 100000
let rec renameTerm (t: Term) =
    match t with
    | Variable v -> Variable(string(varCounter) + "-" + v)
    | Fact f -> 
        Fact { 
            name = f.name
            body = f.body |> List.map renameTerm 
        }
    | _ -> t
let renameRule (r: Rule) =
    varCounter <- varCounter + 1
    Rule (renameTerm (fst r), (snd r) |> List.map renameTerm)
    
type SearchState =
    { Goals: Term list
      Database: Rule list
      Bindings: Bindings
      Choices: (Term list * Bindings ) list }


let rec searchBase (database: Rule list) (goal: Term) (binding: Bindings) (acc: (Term list * Bindings ) list) =
    match database with
    | [] -> acc
    | head::tail ->
        let renamedRule = renameRule head
        match unify goal (fst renamedRule) binding with
        | Fail -> searchBase tail goal binding acc
        | Success newBinding ->
            let subRhs = List.map (fun x -> substitute x newBinding) (snd renamedRule)
            searchBase tail goal binding acc @ [(subRhs,newBinding)]

let rec step (state: SearchState) (solutions: ReturnType list)=
    if state.Goals.IsEmpty then
        if state.Choices.IsEmpty then
            solutions
        else
            let newGoals, bindings = state.Choices.Head
            if newGoals.IsEmpty then
                step { Goals = newGoals
                       Database = state.Database
                       Bindings = bindings
                       Choices = state.Choices.Tail } (solutions @ [Success bindings])
            else
                step { Goals = newGoals
                       Database = state.Database
                       Bindings = bindings
                       Choices = state.Choices.Tail } solutions
    else
        let goalToSolve = state.Goals.Head             
        match searchBase state.Database goalToSolve state.Bindings [] with
        // no rule to be matched -> wrong branch
        | [] ->  step { Goals = []
                        Database = state.Database
                        Bindings = state.Bindings
                        Choices = state.Choices } solutions
        // there are new possibilities
        | lst ->
            let newChoices =
                lst
                |> List.map (fun (x:Term list,y:Bindings) ->
                        let newTail = List.map (fun z -> substitute z y ) state.Goals.Tail
                        (x @ newTail, y) )
            step { Goals = []
                   Database = state.Database
                   Bindings = state.Bindings
                   Choices = newChoices @ state.Choices } solutions
            
            
            
        
let rec getQueryVars (t: Term) =
    match t with
    | Variable v -> [v]
    | Fact f -> List.fold (fun acc x -> (getQueryVars x) @ acc ) [] f.body
    | _ -> []
    |> Seq.distinct
    |> List.ofSeq                
        
let run (plFile: PrologFile) =
    let renameQuery = renameTerm plFile.PrologQuery
    varCounter <- varCounter + 1
    let initialState = {
        Goals = [renameQuery]
        Database = plFile.PrologRules
        Bindings = Map.empty
        Choices = []
    }
    let results = step initialState []
    (results, getQueryVars renameQuery |> Set)
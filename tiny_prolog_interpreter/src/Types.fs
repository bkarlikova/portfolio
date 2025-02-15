module PPA_semestral.Types

type Term =
    | Atom of AtomType
    | Variable of string
    | Fact of FactType
    | Any
    member this.EqualTerms(other: Term) : bool =
        match this, other with
        | Atom a, Atom b -> a.EqualsAtoms(b)
        | Fact _, Fact _ -> this.EqualFacts(other)
        | Variable _, _ -> true
        | _, Variable _ -> true
        | Any, _ -> true
        | _, Any -> true
        | _ -> false
    member this.StrictEqualTerms(other: Term) : bool =
        match this, other with
        | Atom a, Atom b -> a.EqualsAtoms(b)
        | Fact _, Fact _ -> this.EqualFacts(other)
        | Variable a, Variable b -> a.Equals(b)
        | Any, Any -> true
        | _ -> false
    member this.EqualFacts(other: Term) : bool =
        match this, other with
        | Fact a, Fact b when (a.name.Equals(b.name) && a.body.Length = b.body.Length) ->
                              List.zip a.body b.body |> List.map (fun (x, y) -> x.EqualTerms(y)) |> List.forall id
        | _ -> false
        
    
and FactType = { name: string; body: Term list }

and AtomType =
    | StrAtom of string 
    | NumAtom of int
    | Nil
    member this.EqualsAtoms(other: AtomType) : bool =
        match this, other with
        | StrAtom x, StrAtom y when System.Char.IsLower(x[0]) && System.Char.IsLower(y[0]) && x.Equals(y) -> true
        | NumAtom x, NumAtom y when x = y -> true
        | Nil, Nil -> true
        | _ -> false
     
type Rule = Term * Term list
type Query = Term
type PrologFile = { PrologRules: Rule list; PrologQuery: Query }

type ReturnType =
    | Fail
    | Success of Map<string, Term>

type Bindings = Map<string, Term>
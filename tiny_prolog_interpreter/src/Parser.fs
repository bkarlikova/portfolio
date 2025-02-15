module PPA_semestral.Parser

open System
open System.Data
open Microsoft.FSharp.Collections
open PPA_semestral.Types
open PPA_semestral.Tokenizer

type Result<'T,'S> =
    | Accept of value: 'T * rest: 'S
    | Reject of msg: string


let rec isRule =
    fun x ->
        match x with
        | Implication::_ -> true
        | [] -> false
        | _::rest -> isRule rest
        
let isQuery =
    fun x ->
        match x with
        | QuestionMark::_ -> true
        | _ -> false
        
let isFact = fun x -> (not (isQuery x)) && (not (isRule x))

let parseTail : Token -> Result<Term,Token list>  =
    fun x ->
        match x with
        | Underscore -> Accept(Any, [])
        | VarToken v -> Accept(Variable v, [])
        | _ -> Reject  "invalid tail"

let rec factParser: Token list -> Result<Term, Token list> =
    fun tokens ->
        match tokens with
        | Identifier(name) :: LeftParen :: rest -> 
            let rec parseArguments acc tokens =
                match termParser tokens with
                | Accept(value, Comma :: rest') -> 
                    parseArguments (value :: acc) rest'
                | Accept(value, RightParen :: rest') -> 
                    Accept(List.rev (value :: acc), rest')
                | Reject msg -> Reject msg
                | _ -> Reject "Invalid syntax in arguments"
            match parseArguments [] rest with
            | Accept(arguments, rest') -> 
                Accept(Term.Fact{ name = name; body = arguments }, rest')
            | Reject msg -> Reject ("Invalid fact arguments: " + msg)
        | _ -> Reject "Invalid fact syntax"

        
and termParser: Token list -> Result<Term,Token list>  =
    fun x ->
        match x with
        | Underscore :: rest  -> Accept(Any, rest)
        | Number i :: rest  -> Accept(Atom(NumAtom i), rest)
        | VarToken v :: rest  -> Accept( Variable v, rest)
        | Identifier _ :: LeftParen :: _ ->
            match factParser x with
            | Accept (fact, rest') -> Accept(fact, rest')
            | Reject msg -> Reject (msg + " ,even bodyTypeParser thinks this is invalid list")
        | LeftBracket :: rest -> 
            match parseList rest with
            | Accept(lst, rest') -> Accept(lst, rest')
            | Reject msg -> Reject (msg + " ,even bodyTypeParser thinks this is invalid list")
        | Identifier n :: rest -> Accept(Atom(StrAtom n), rest)
        | _ -> Reject "invalid token"

 
and parseList: Token list -> Result<Term, Token list> =
    fun x ->
        let rec helper (lst: Token list) (acc: Term) =
            match lst with
            | Pipe :: tail :: RightBracket :: rest ->
                match parseTail tail with
                | Accept(value, _) -> Accept(value, rest)
                | Reject msg -> Reject (msg + " invalid tail")
            | RightBracket :: rest -> Accept(acc, rest)
            | Comma :: rest -> helper rest acc
            | head ->
                match termParser head with
                | Accept(value, rest') ->
                    match helper rest' acc with
                    | Accept (newAcc, rest'') ->  Accept( Fact { name = "cons"; body = [value] @ [newAcc] }, rest'')
                    | Reject msg -> Reject (msg + " invalid list")
                | Reject msg -> Reject msg
        helper x (Atom Nil)

let parseRhs: Token list -> Term list =
    fun x ->
        let rec helper (acc: Term list) (rest: Token list) =
            match termParser rest with
            | Accept(factType, tokens) ->
                let newAcc = List.append acc [factType]
                match tokens with
                | [] -> newAcc
                | Comma::rest' -> helper newAcc rest'
                | x -> raise (Exception ("I have no idea what this case is... " + string(x)))
            | Reject msg -> raise (Exception (msg + "you are a not a bad programmer, you're gonna fix it!"))
        helper [] x
      

let getQueryTokens =
    fun lst -> lst |> List.filter (fun x -> isQuery x) 

let getRulesTokens =
    fun lst -> lst |> List.filter (fun x -> isRule x)
 
let getFactTokens =
    fun lst -> lst |> List.filter (fun x -> isFact x)
    
let getQuery: Token list -> Query =
    fun x ->
        match x with
        | QuestionMark::rest ->
            match factParser rest with
            | Accept(value, _) -> value
            | Reject msg -> raise (SyntaxErrorException (msg + "you are a bad programmer, because there really should have been query"))
        | _ -> raise (SyntaxErrorException "you are a bad programmer, because there should have been query")
let getFact = fun x ->
    match factParser x with
    | Accept (value, _) -> (value,[])
    | Reject msg -> raise (SyntaxErrorException (msg + "this should have been a fact"))
    
    
let getRule: Token list -> Term * Term list =
    fun x ->
        let rec helper acc rest =
            match rest with
            | Implication::rest' ->
                match factParser acc with
                | Accept (value, _) ->
                                    let head = value
                                    let tail = parseRhs rest'
                                    (head, tail)
                | Reject msg -> raise (SyntaxErrorException (msg + "this should have been a fact"))
            | t::rest' -> helper (List.append acc [t]) rest'
            | [] -> raise (SyntaxErrorException "you are a bad programmer, because there should have been query")
        helper [] x
        
let prologConstructParser =
    fun x ->
        let rec helper acc rest =
            match rest with
            | Dot::rest' -> Accept(acc, rest')
            | any::rest' -> helper (List.append acc [any]) rest'
            | [] -> Reject "Invalid term, missing ."
        helper [] x

let dividePerConstruct =
    fun x ->
        let rec helper acc rest =
            match prologConstructParser rest with
            | Accept(value, []) -> (List.append acc [value])
            | Accept(value, rest') -> helper (List.append acc [value]) rest'
            | Reject msg -> raise (SyntaxErrorException ("Invalid file -" + msg))
        helper [] x



let getPrologFile: string -> PrologFile =
    fun lstAll ->
        let lst =  lstAll
                    |> Seq.toList
                    |> tokenize
                    |> dividePerConstruct
        let queryTokens = getQueryTokens lst
        let query = if not queryTokens.IsEmpty then
                        queryTokens
                        |> List.reduce List.append 
                        |> getQuery
                    else
                        printfn "Missing query,  woun't find any solution"
                        Term.Fact({name = ""; body = []})
        let rules = lst
                    |> getRulesTokens 
                    |> List.map (fun x ->
                        getRule x)
        let facts = lst
                    |> getFactTokens 
                    |> List.map (fun x ->
                        getFact x)
        {
            PrologQuery = query;
            PrologRules = facts @ rules
        }
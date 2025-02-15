module PPA_semestral.Tokenizer
open System

type Token =
    | Identifier of string //lowercase
    | VarToken of string //uppercase
    | Number of int
    | LeftParen
    | RightParen
    | LeftBracket
    | RightBracket
    | Underscore
    | Comma
    | Pipe
    | Implication
    | Dot
    | QuestionMark

let rec parseChars acc rem =
    match rem with
    | c::rest when Char.IsLetter(c) || c = '_' ->
        parseChars (acc + string c) rest
    | _ -> acc, rem
    
let rec parseDigit acc rem =
    match rem with
    | c::rest when Char.IsDigit(c) ->
        parseChars (acc + string c) rest
    | _ -> acc, rem
    
let rec tokenize input =
    match input with
    | [] -> []
    | '('::rest -> LeftParen :: tokenize rest
    | ')'::rest -> RightParen :: tokenize rest
    | '['::rest -> LeftBracket :: tokenize rest
    | ']'::rest -> RightBracket :: tokenize rest
    | ','::rest -> Comma :: tokenize rest
    | '.'::rest -> Dot :: tokenize rest
    | '_'::rest -> Underscore :: tokenize rest
    | '|'::rest -> Pipe :: tokenize rest
    | ':'::'-':: rest -> Implication :: tokenize rest
    | '?'::'-':: rest -> QuestionMark::tokenize rest
    | c::rest when Char.IsWhiteSpace(c) -> tokenize rest
    | c::rest when Char.IsUpper(c) -> 
        let variable, rest' = parseChars (string c) rest
        VarToken( variable ) :: tokenize rest'
    | c::rest when Char.IsLower(c) -> 
        let identifier, rest' = parseChars (string c) rest
        Identifier( identifier ) :: tokenize rest'
    | c::rest when Char.IsDigit(c) || c = '-' ->
        let number, rest' = parseDigit (string c) rest
        Number( int number ) :: tokenize rest'
    | _ -> failwith "Invalid character"
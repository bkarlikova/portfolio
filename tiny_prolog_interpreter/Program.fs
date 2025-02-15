module PPA_semestral.Program

open PPA_semestral.Search
open PPA_semestral.Parser
open PPA_semestral.Evaluation

open System
open System.IO

let readFile (path: string) =
    try
        if String.IsNullOrWhiteSpace(path) then
            Error "Path cannot be null or empty"
        elif not (File.Exists(path)) then
            Error $"File does not exist at: {path}"
        else
            let attributes = File.GetAttributes(path)
            if attributes.HasFlag(FileAttributes.Directory) then
                Error $"Path refers to a directory: {path}"
            else
                use testStream = new FileStream(path, FileMode.Open, FileAccess.Read)
                Ok (File.ReadAllText(path))
    with
    | :? UnauthorizedAccessException ->
        Error $"Access denied to file: {path}"
    | :? IOException as ex ->
        Error $"IO error: {ex.Message}"
    | ex ->
        Error $"Unexpected error: {ex.Message}"


[<EntryPoint>]
let main argv =
    try
        match argv with
        | [|path|] ->
            match readFile path with
            | Ok content ->
                try
                    let parsedPrologFile = 
                        try
                            getPrologFile content
                        with ex ->
                            eprintfn $"Parser error: {ex.Message}"
                            exit 2 
                    
                    let results, queryVars = 
                        try 
                            run parsedPrologFile
                        with ex ->
                            eprintfn $"Execution error: {ex.Message}"
                            exit 3 
                    
                    let answer = evaluate queryVars results
                    printfn "%s" answer
                    0 
                    
                with ex ->
                    eprintfn $"Unexpected error: {ex.Message}"
                    4 
                    
            | Error readErr ->
                eprintfn "File error: %s" readErr
                1 
                
        | _ ->
            eprintfn "Usage: prolog-interpreter <path-to-prolog-file>"
            1 
            
    with ex ->
        eprintfn "Critical error: %s\nStack trace: %s" ex.Message ex.StackTrace
        5

 



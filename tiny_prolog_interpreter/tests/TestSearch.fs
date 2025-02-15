module PPA_semestral.tests.TestSearch


open PPA_semestral.Types
open PPA_semestral.Parser
open PPA_semestral.Search
open PPA_semestral.Evaluation

open System
open System.IO
open Xunit



let atomAlice = Atom(StrAtom "alice")
let atomMary = Atom(StrAtom "mary")
let atomBob = Atom(StrAtom "bob")
let atomCharlie = Atom(StrAtom "charlie")
let atomJohn = Atom(StrAtom "john")
let atomLaura = Atom(StrAtom "Laura")


let atom23 = Atom(NumAtom 23)
let varX = Variable "X"
let varY = Variable "Y"
let varZ = Variable "Z"
let varW = Variable "W"

let varA = Variable "A"
let varB = Variable "B"
let varC = Variable "C"

let factParent1 =
    (Fact
        { name = "parent"
          body = [ atomAlice; atomBob ] },
     [])

let factParent2 =
    (Fact
        { name = "parent"
          body = [ atomBob; atomCharlie ] },
     [])

let factParent3 =
    (Fact
        { name = "parent"
          body = [ atomMary; atomBob ] },
     [])
let factParent4 =
    (Fact
        { name = "parent"
          body = [ atomJohn; atomAlice ] },
     [])

let ruleSibling =
    (Fact
        { name = "sibling"
          body = [ varX; varY ] },
     [ Fact
           { name = "parent"
             body = [ varZ; varX ] }
       Fact
           { name = "parent"
             body = [ varZ; varY ] } ])

let ruleAncestor =
    (Fact
        { name = "ancestor"
          body = [ varX; varY ] },
     [ Fact
           { name = "parent"
             body = [ varX; varY ] } ])

let ruleGrandparent =
    (Fact
        { name = "grandparent"
          body = [ varX; varY ] },
     [ Fact
           { name = "parent"
             body = [ varX; varZ ] }
       Fact
           { name = "parent"
             body = [ varZ; varY ] } ])

let someBase =
    [ factParent1
      factParent2
      factParent3
      factParent4
      ruleSibling
      ruleAncestor
      ruleGrandparent ]

let queryParent =
    Fact
        { name = "parent"
          body = [ varA; atomCharlie ] }

let queryAncestor =
    Fact
        { name = "ancestor"
          body = [ atomAlice; varB ] }

let queryGrandparent =
    Fact
        { name = "grandparent"
          body = [ varC; atomCharlie ] }

[<Fact>]
let ``Test Family`` () =
    let content = File.ReadAllText("../../../data/family.txt")
    let parsedPrologFile = getPrologFile content
    let result, queryVars = run parsedPrologFile 
    let evaluated = evaluate queryVars result
    Assert.Equal(
        "X = lisa;\nX = susan.",
        evaluated
        )


[<Fact>]
let ``Test Member`` () =
    let content = File.ReadAllText("../../../data/member.txt")
    let parsedPrologFile = getPrologFile content
    let result, queryVars = run parsedPrologFile 
    let evaluated = evaluate queryVars result
    Assert.Equal(
        "X = 0;\nX = A, A = X;\nX = 2;\nX = -1;\nX = 1.",
        evaluated
        )
    
    
[<Fact>]
let ``Test Member2`` () =
    
    let content = File.ReadAllText("../../../data/member2.txt")
    let parsedPrologFile = getPrologFile content
    let result, queryVars = run parsedPrologFile 
    let evaluated = evaluate queryVars result
    Assert.Equal(
        "False.",
        evaluated
        )

    
[<Fact>]
let ``Test append`` () =
    let content = File.ReadAllText("../../../data//append.txt")
    let parsedPrologFile = getPrologFile content
    let result, queryVars = run parsedPrologFile 
    let evaluated = evaluate queryVars result
    Assert.Equal(
        "R = cons(1,cons(2,cons(5,cons(6,nil)))).",
        evaluated
        )
    

[<Fact>]
let ``Test factorial`` () =
    let content = File.ReadAllText("../../../data/factorial.txt")
    let parsedPrologFile = getPrologFile content
    let result, queryVars = run parsedPrologFile 
    let evaluated = evaluate queryVars result
    Assert.Equal(
        "X = s(s(s(s(s(s(z)))))).",
        evaluated
        )
module PPA_semestral.tests.TestParser


open PPA_semestral.Tokenizer
open PPA_semestral.Parser
open PPA_semestral.Types
open PPA_semestral.Unification
open Xunit

let a = Seq.toList "member(X, [H|T]) :- member(X, T)."
let b = Seq.toList "f(g(X), g(X), f(X), h(f(g(C))))."
let c = Seq.toList "f(g(4, 4), g(X), f(X), h(f(g(C))))."
let d = Seq.toList "f(g(4, 4), g(X), f(X), h(f(g([7,8,9]))))."

let e =
    Seq.toList
        "parent(john, mary).
            parent(john, tom).
            parent(mary, -1).
            parent(tom, lisa).
            grandparent(X, Y) :- parent(X, Z), parent(Z, Y).
            ?-grandparent(john, marry)."

[<Fact>]
let ``Tokenize string`` () =
    Assert.StrictEqual(
        tokenize a,
        [ Identifier "member"
          LeftParen
          VarToken "X"
          Comma
          LeftBracket
          VarToken "H"
          Pipe
          VarToken "T"
          RightBracket
          RightParen
          Implication
          Identifier "member"
          LeftParen
          VarToken "X"
          Comma
          VarToken "T"
          RightParen
          Dot ]
    )

    Assert.StrictEqual(
        tokenize b,
        [ Identifier "f"
          LeftParen
          Identifier "g"
          LeftParen
          VarToken "X"
          RightParen
          Comma
          Identifier "g"
          LeftParen
          VarToken "X"
          RightParen
          Comma
          Identifier "f"
          LeftParen
          VarToken "X"
          RightParen
          Comma
          Identifier "h"
          LeftParen
          Identifier "f"
          LeftParen
          Identifier "g"
          LeftParen
          VarToken "C"
          RightParen
          RightParen
          RightParen
          RightParen
          Dot ]
    )

    Assert.StrictEqual(
        tokenize c,
        [ Identifier "f"
          LeftParen
          Identifier "g"
          LeftParen
          Number 4
          Comma
          Number 4
          RightParen
          Comma
          Identifier "g"
          LeftParen
          VarToken "X"
          RightParen
          Comma
          Identifier "f"
          LeftParen
          VarToken "X"
          RightParen
          Comma
          Identifier "h"
          LeftParen
          Identifier "f"
          LeftParen
          Identifier "g"
          LeftParen
          VarToken "C"
          RightParen
          RightParen
          RightParen
          RightParen
          Dot ]
    )

    Assert.StrictEqual(
        tokenize d,
        [ Identifier "f"
          LeftParen
          Identifier "g"
          LeftParen
          Number 4
          Comma
          Number 4
          RightParen
          Comma
          Identifier "g"
          LeftParen
          VarToken "X"
          RightParen
          Comma
          Identifier "f"
          LeftParen
          VarToken "X"
          RightParen
          Comma
          Identifier "h"
          LeftParen
          Identifier "f"
          LeftParen
          Identifier "g"
          LeftParen
          LeftBracket
          Number 7
          Comma
          Number 8
          Comma
          Number 9
          RightBracket
          RightParen
          RightParen
          RightParen
          RightParen
          Dot ]
    )


[<Fact>]
let `` Get prolog constructs`` () =
    Assert.StrictEqual(
        tokenize e |> dividePerConstruct,
        [ [ Identifier "parent"
            LeftParen
            Identifier "john"
            Comma
            Identifier "mary"
            RightParen ]
          [ Identifier "parent"
            LeftParen
            Identifier "john"
            Comma
            Identifier "tom"
            RightParen ]
          [ Identifier "parent"
            LeftParen
            Identifier "mary"
            Comma
            Number -1
            RightParen ]
          [ Identifier "parent"
            LeftParen
            Identifier "tom"
            Comma
            Identifier "lisa"
            RightParen ]
          [ Identifier "grandparent"
            LeftParen
            VarToken "X"
            Comma
            VarToken "Y"
            RightParen
            Implication
            Identifier "parent"
            LeftParen
            VarToken "X"
            Comma
            VarToken "Z"
            RightParen
            Comma
            Identifier "parent"
            LeftParen
            VarToken "Z"
            Comma
            VarToken "Y"
            RightParen ]
          [ QuestionMark
            Identifier "grandparent"
            LeftParen
            Identifier "john"
            Comma
            Identifier "marry"
            RightParen ] ]
    )


[<Fact>]

let ``Term Parser`` () =
    let a = Seq.toList "parent(marry, john)" |> tokenize
    let b = Seq.toList "parent(X, john)" |> tokenize
    let c = Seq.toList "parent([1, 2, 3])" |> tokenize
    let d = Seq.toList "parent(_, [john, john, john])" |> tokenize
    let e = Seq.toList "parent(child(hana), father(harry))" |> tokenize
    let f = Seq.toList "parent(_, [john|_])" |> tokenize
    let g = Seq.toList "parent([1,[2,3],[4]])" |> tokenize



    Assert.StrictEqual(
        Accept(
            Fact
                { name = "parent"
                  body = [ Atom(StrAtom "marry"); Atom(StrAtom "john") ] },
            []
        ),
        termParser a
    )

    Assert.StrictEqual(
        Accept(
            Fact
                { name = "parent"
                  body =
                    [ Fact
                          { name = "cons"
                            body =
                              [ Atom(NumAtom 1)
                                Fact
                                    { name = "cons"
                                      body =
                                        [ Atom(NumAtom 2)
                                          Fact
                                              { name = "cons"
                                                body = [ Atom(NumAtom 3); (Atom Nil) ] } ] } ] } ] },

            []
        ),
        termParser c
    )

    Assert.StrictEqual(
        Accept(
            Fact
                { name = "parent"
                  body =
                    [ Any
                      Fact
                          { name = "cons"
                            body =
                              [ Atom(StrAtom "john")
                                Fact
                                    { name = "cons"
                                      body =
                                        [ Atom(StrAtom "john")
                                          Fact
                                              { name = "cons"
                                                body = [ Atom(StrAtom "john"); (Atom Nil) ] } ] } ] } ] },
            []
        ),
        termParser d
    )

    Assert.StrictEqual(
        Accept(
            Fact
                { name = "parent"
                  body =
                    [ Fact
                          { name = "child"
                            body = [ Atom(StrAtom "hana") ] }
                      Fact
                          { name = "father"
                            body = [ Atom(StrAtom "harry") ] } ] },
            []
        ),
        termParser e
    )

    Assert.StrictEqual(
        Accept(
            Fact
                { name = "parent"
                  body =
                    [ Any
                      Fact
                          { name = "cons"
                            body = [ Atom(StrAtom "john"); Any ] } ] },
            []
        ),
        termParser f
    )

    Assert.StrictEqual(
        Accept(
            Fact
                { name = "parent"
                  body =
                    [ Fact
                          { name = "cons"
                            body =
                              [ Atom(NumAtom 1)
                                Fact
                                    { name = "cons"
                                      body =
                                        [ Fact
                                              { name = "cons"
                                                body =
                                                  [ Atom(NumAtom 2)
                                                    Fact
                                                        { name = "cons"
                                                          body = [ Atom(NumAtom 3); Atom Nil ] } ] }
                                          Fact
                                              { name = "cons"
                                                body =
                                                  [ Fact
                                                        { name = "cons"
                                                          body = [ Atom(NumAtom 4); Atom Nil ] }
                                                    Atom Nil ] } ] } ] } ] },
            []
        ),
        termParser g
    )

[<Fact>]
let ``Parse empty list`` () =
    let input = Seq.toList "empty([])." |> tokenize |> dividePerConstruct 
    Assert.StrictEqual(Accept(Fact { name = "empty"; body = [ Atom Nil ] }, []), termParser input.Head)

[<Fact>]
let ``Parse single-element list`` () =
    let input = Seq.toList "single([5])." |> tokenize |> dividePerConstruct 

    Assert.StrictEqual(
        Accept(
            Fact
                { name = "single"
                  body =
                    [ Fact
                          { name = "cons"
                            body = [ Atom(NumAtom 5); Atom Nil ] } ] },
            []
        ),
        termParser input.Head
    )


[<Fact>]
let ``Parse rule with multiple body terms`` () =
    let input = Seq.toList "rule(X) :- a(X), b(X), c(X)." |> tokenize |> dividePerConstruct

    Assert.StrictEqual(

        (Fact
            { name = "rule"
              body = [ Variable "X" ] },
         [ Fact { name = "a"; body = [ Variable "X" ] }
           Fact { name = "b"; body = [ Variable "X" ] }
           Fact { name = "c"; body = [ Variable "X" ] } ]),

        getRule input.Head
    )



[<Fact>]
let ``Parse complex nested structure`` () =
    let input = Seq.toList "f(a, [b, c|X], 5)." |> tokenize |> dividePerConstruct

    Assert.StrictEqual(
        Accept(
            Fact
                { name = "f"
                  body =
                    [ Atom (StrAtom "a")
                      Fact
                          { name = "cons"
                            body =
                              [ Atom(StrAtom "b")
                                Fact
                                    { name = "cons"
                                      body = [ Atom(StrAtom "c"); Variable "X" ] } ] }
                      Atom(NumAtom 5) ] },
            []
        ),
        termParser input.Head
    )

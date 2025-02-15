module PPA_semestral.tests.TestUnification

open Xunit
open PPA_semestral.Unification
open PPA_semestral.Unification

open PPA_semestral.Types
open PPA_semestral.Parser


//Data
let atomJohn = Atom(StrAtom "john")
let atomMary = Atom(StrAtom "mary")
let atom42 = Atom(NumAtom 42)
let atomNil = Atom Nil

let varX = Variable "X"
let varY = Variable "Y"
let varZ = Variable "Z"
let varW = Variable "W"

let varA = Variable "A"
let varB = Variable "B"
let varC = Variable "C"

let fact1 =
    Fact
        { name = "parent"
          body = [ atomJohn; atomMary ] }

let factTypeXYBar = { name = "bar"; body = [ varX; varY ] }
let factTypeAABar = { name = "bar"; body = [ varA; varA ] }

let factTypeBMAry =
    { name = "parent"
      body = [ varB; atomMary ] }

let factTypeXYXY =
    { name = "foo"
      body = [ varX; varY; varX; varY ] }

let factTypeBBAA =
    { name = "foo"
      body = [ varB; varB; varA; varA ] }

let factTypeFooBar1 =
    { name = "foo"
      body = [ Fact factTypeAABar; atomJohn; varA ] }

let factTypeFooBar2 =
    { name = "foo"
      body = [ Fact factTypeXYBar; varX; atomMary ] }

let factWMary =
    Fact
        { name = "parent"
          body = [ varW; atomMary ] }

let factTypeWMary =
    { name = "parent"
      body = [ varW; atomMary ] }

let factXY =
    Fact
        { name = "parent"
          body = [ varX; varY ] }

let factXX =
    Fact
        { name = "parent"
          body = [ varX; varX ] }

let fact3 =
    Fact
        { name = "sibling"
          body = [ varX; varY ] }

let fact4 =
    Fact
        { name = "ancestor"
          body = [ varX; atomJohn ] }


// (X, Y, parent(X,Y)) (john, Z, parent(W, mary))
let binding1: (Term * Term) list = [ (varX, atomJohn); (varY, varZ) ]

let bindingWithTransitiveConflict: (Term * Term) list =
    [ (varX, atomJohn); (varY, varZ); (atomMary, varZ); (varY, atom42) ]

let bindingWithTransitiveDefinition: (Term * Term) list =
    [ (varX, atomJohn); (varY, varZ); (atomMary, varZ) ]

let bindingAfterTransformation: (Term * Term) list =
    [ (varX, atomJohn); (varY, atomMary); (atomMary, varZ) ]

[<Fact>]
let ``Test Unification - Identical Facts`` () =
    Assert.StrictEqual(Success(Map.empty), unify factXY factXY Map.empty)

[<Fact>]
let ``Test Unification - Conflicting Atoms`` () =
    Assert.StrictEqual(Success(Map[("Y", varX)]), unify factXY factXX Map.empty)

[<Fact>]
let ``Test Unification - Different Arities`` () =
    let differentArityFact = { name = "parent"; body = [ varX ] }
    Assert.StrictEqual(Fail, unify factXY (Fact differentArityFact) Map.empty)

[<Fact>]
let ``Test Unification - Variable inside Structure`` () =
    let term1 = { name = "foo"; body = [ varX; varY ] }

    let term2 =
        { name = "foo"
          body =
            [ Fact
                  { name = "bar"
                    body = [ varX; atomJohn ] }
              atomJohn ] }

    Assert.StrictEqual(Fail, unify (Fact term1) (Fact term2) Map.empty)

[<Fact>]
let ``Test Unification - Variable and Structure`` () =
    let term1 = { name = "foo"; body = [ varX; varY ] }

    let term2 =
        { name = "foo"
          body = [ Fact { name = "bar"; body = [ varB; varA ] }; atomJohn ] }

    Assert.StrictEqual(
        Success(
            Map[("X", Fact { name = "bar"; body = [ varB; varA ] })
                ("Y", atomJohn)]
        ),
        unify (Fact term1) (Fact term2) Map.empty
    )

[<Fact>]
let ``Test Unification`` () =
    Assert.StrictEqual(
        Success(
            Map[("X", varA)
                ("Y", varA)
                ("B", varA)]
        ),
        unify (Fact factTypeXYXY) (Fact factTypeBBAA) Map.empty
    )

    Assert.StrictEqual(
        Success(
            Map[("X", varW)
                ("Y", atomMary)]
        ),
        unify factXY factWMary Map.empty
    )

    Assert.StrictEqual(
        Success(
            Map[("X", atomMary)
                ("W", atomMary)]
        ),
        unify factXX factWMary Map.empty
    )


[<Fact>]
let ``Test Substitution`` () =
    Assert.StrictEqual(
        { name = "foo"
          body = [ Variable "Ahoj"; Variable "Ahoj"; Atom(NumAtom 42) ] },
        (substituteFact
            { name = "foo"
              body = [ Variable "A"; Variable "A"; Variable "B" ] }
            Map[("A", Variable "Ahoj")
                ("B", Atom(NumAtom 42))])
    )

    Assert.StrictEqual(
        { name = "foo"
          body = [ Fact { name = "bar"; body = [ varA; varA ] }; atomJohn; varA ] },
        (substituteFact
            factTypeFooBar1
            Map[("X", atomJohn)
                ("Y", varB)])
    )

[<Fact>]
let ``Test Substitution - Variable Appearing Multiple Times`` () =
    let input =
        { name = "test"
          body = [ varX; varX; varY ] }

    let expected =
        { name = "test"
          body = [ atomJohn; atomJohn; atomMary ] }

    Assert.StrictEqual(
        expected,
        substituteFact
            input
            Map[("X", atomJohn)
                ("Y", atomMary)]
    )

[<Fact>]
let ``Test Substitution - Variable to Another Variable`` () =
    let input = { name = "test"; body = [ varA; varB ] }
    let expected = { name = "test"; body = [ varC; varB ] }
    Assert.StrictEqual(expected, substituteFact input Map[("A", varC)])

[<Fact>]
let ``Test Substitution - Structure Inside Structure`` () =
    let input =
        { name = "nested"
          body = [ Fact factTypeXYBar; varX ] }

    let expected =
        { name = "nested"
          body =
            [ Fact
                  { name = "bar"
                    body = [ atomJohn; varY ] }
              atomJohn ] }

    Assert.StrictEqual(expected, substituteFact input Map[("X", atomJohn)])

[<Fact>]
let ``Test Substitution - No Matching Variables`` () =
    let input =
        { name = "unaffected"
          body = [ varX; atom42 ] }

    Assert.StrictEqual(input, substituteFact input Map[("Y", atomJohn)]) // X is not in substitution



[<Fact>]
let ``Test Concatenating Bindings `` () =
    Assert.StrictEqual(
        Success(
            Map[("A", varX)
                ("B", atomJohn)
                ("Z", atomJohn)
                ("Y", varX)]
        ),
        concatBindings
            Map[("A", varY)
                ("B", varZ)]
            Map[("Z", atomJohn)
                ("Y", varX)]
    )

[<Fact>]
let ``Test Concatenating Bindings - Empty Bindings`` () =
    let bindings = Map[("A", varX)]
    Assert.StrictEqual(Success(bindings), concatBindings bindings Map.empty)

[<Fact>]
let ``Test Concatenating Bindings - Conflict`` () =
    let bindings1 = Map[("X", atomJohn)]
    let bindings2 = Map[("X", atomMary)]
    Assert.StrictEqual(Fail, concatBindings bindings1 bindings2) // X → john and X → mary conflict

[<Fact>]
let ``Test Concatenating Bindings - Nested Substitutions`` () =
    let bindings1 = Map[("A", varB)]
    let bindings2 = Map[("B", atomJohn)]

    Assert.StrictEqual(
        Success(
            Map[("A", atomJohn)
                ("B", atomJohn)]
        ),
        concatBindings bindings1 bindings2
    )

[<Fact>]
let ``Test List Unification`` () =
    let listTerm =
        Fact
            { name = "cons"
              body =
                [ Atom(NumAtom 1)
                  Fact
                      { name = "cons"
                        body =
                          [ Atom(NumAtom 3)
                            Fact
                                { name = "cons"
                                  body = [ Atom(NumAtom 0); Atom Nil ] } ] } ] }

    let t1 =
        Fact
            { name = "member"
              body = [ Variable "61340X"; listTerm ] }

    let t2 =
        Fact
            { name = "member"
              body =
                [ Variable "39910X"
                  Fact
                      { name = "cons"
                        body = [ Variable "39910X"; Any ] } ] }

    Assert.StrictEqual(
        Success(
            Map[("61340X", Atom(NumAtom 1))
                ("39910X", Atom(NumAtom 1))]
        ),
        unify t1 t2 Map.empty
    )

[<Fact>]
let ``Test List Unification - Cons Cells`` () =
    let t1 =
        Fact
            { name = "append"
              body =
                [ Fact
                      { name = "cons"
                        body =
                          [ Atom(NumAtom 1)
                            Fact
                                { name = "cons"
                                  body = [ Atom(NumAtom 2); Atom Nil ] } ] }
                  Fact
                      { name = "cons"
                        body =
                          [ Atom(NumAtom 5)
                            Fact
                                { name = "cons"
                                  body = [ Atom(NumAtom 6); Atom Nil ] } ] }
                  Variable "1-R" ] }

    let t2 =
        Fact
            { name = "append"
              body =
                [ Fact
                      { name = "cons"
                        body = [ Variable "2-A"; Variable "2-As" ] }
                  Variable "2-B"
                  Fact
                      { name = "cons"
                        body = [ Variable "2-A"; Variable "2-Cs" ] } ] }

    Assert.StrictEqual(
        Success(
            Map[("2-A", Atom (NumAtom 1))
                ("2-As", Fact{ name = "cons"; body = [ Atom(NumAtom 2); Atom Nil ] })
                ("2-B", Fact { name = "cons"; body = [ Atom(NumAtom 5); Fact{ name = "cons"; body = [ Atom(NumAtom 6); Atom Nil ] } ] })
                ("1-R", Fact { name = "cons"; body = [ Atom(NumAtom 1); Variable "2-Cs" ] })
                ]
        ),
        unify t1 t2 Map.empty
    )

# Tiny TinyProlog

This program is an attempt of implementing an naive Tiny prolog interpreter in **F#**. Its purpose was more as a learning tool, than something new, fast or innovative. Further implementation consists well known algorithm and procedures.

---
## User guide

This program requires **.NET SDK**!

To run a prolog file, one can execute the interpreter from the **project** root directory using:
```bash
dotnet run $"path/to/prolog_file.txt"
```

---
## Features

- **Terms**: Supports **Atoms** (strings or numbers) and **Lists** (written with square brackets, e.g., `[1,2,3]` or cons cells like `[H | T]`).
- **Database**: Stores **facts** and **rules**, each terminated by `.`. Rules are structured as `(head :- body)`, where:
  - The **head** (left-hand-side, or *lhs*) is a single term.
  - The **body** (right-hand-side, or *rhs*) is a conjunction (**AND**) of terms separated by `,`.
  - **OR** logic can be achieved by defining multiple rules with the same head.
- **Queries**: Executes queries starting with `?-`. Files must contain exactly one query to be valid.
- **Solutions**: Returns all possible solutions, with `true.` if variables are fully unified, or `false.` for inconsistencies.

---

## Limitations
- No arithmetic or comparison operators (e.g., `=:=`, `<`).
- Parentheses `()` in rule bodies are unsupported (allowed only in facts).
- No interactive mode; solutions are printed after execution.
- Interpreter cannot solve inputs with infinite different solutions, because it terminates after searching all possible solutions.

---

## Implementation

### Parsing
The program parses a file of raw text into a `PrologFile` structure, tokenizing and validating the input. The internal data types are:

```fsharp
type Term =
    | Atom of AtomType
    | Variable of string
    | Fact of FactType
    | Any

and FactType = { name: string; body: Term list }

and AtomType =
    | StrAtom of string 
    | NumAtom of int
    | Nil

type Rule = Term * Term list
type Query = Term
type PrologFile = { PrologRules: Rule list; PrologQuery: Query }
```
---
### Unification
Unification algorithm is implemented by using six simple rules presented in [article written by Martelli and Montanari](https://api.semanticscholar.org/CorpusID:10921306). Function `unify` takes two terms, binding and returns only variable bindings or failure:

```fsharp
type ReturnType =
    | Fail
    | Success of Map<string, Term>

type Bindings = Map<string, Term>
```
---
### Search Algorithm
Explores the state space using a `SearchState`:

```fsharp
type SearchState =
    { Goals: Term list
      Database: Rule list
      Bindings: Bindings
      Choices: (Term list * Bindings) list }
```

Each step is done by `step` function which takes `SearchState` and list of accumulated solutions.

Before first step, the state is initialized by pushing the query into `Goals`. 

One step: 
  - If `Goals` is non-empty:
    - Pop the first goal and find **all** rules whose lhs unifies with it. This leads to a list of right-hand-sides and their bindings. More than one element in this list represent something like choice point or crossroad in the search space. Hence, they are called `Choices`.
    - For each choice, prepend it to the remaining goals and add to `Choices`.
    - Reset `Goals` to empty list in run another step
  - If `Goals` is empty:
    - Pop a choice from `Choices`. 
      - If its goal list is empty, **solution is found**
        - record the solution to accumulator.
      - Turn the choice as `Goals` and run another step
    - if `Choices` are empty (`Goals` are empty as well)
      - return solutions and end searching
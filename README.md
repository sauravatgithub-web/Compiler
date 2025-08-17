# Compiler
This project attempts to build a compiler based on C++ programming language. There are several stages to this project. 
<ul>
    <li> The first stage involves describing regular expressions for the language. </li>
    <li> The second stage involves building state diagram for each of the regex patterns. We will try to build a DFA for each regex pattern to be used in further stages. </li>
    <li> The third stage will perform lexical analysis of the code. In this part, we will try to generate token sequuences to be used in further stages. </li>
</ul>

## Stage 1
We will define our regex in [regex.txt](regex.txt) file. 2<sup>nd</sup> stage will use this grammar patterns to build various state machines. It is important that the grammar is defined correctly using **concatenation** `.`, **or** `|`, **Kleen's closure** `*`, **optional** `?` and **Kleen's plus** operator `+`, all at correct places to ensure desired output. To include a wide range of inputs, operators are preceded by `\` as `\.`, `\|`, `\?` or `\*`. Even `(` as operator must be written as `\(` and `)` as `\)`. It will be better if spacing are appropriately added while defining the regular expressions, however, it is not necessary. The structure for grammar statement is : 
```
token_name : expression
Eg: digits : digit \. \( digit \) \*
Eg: digits : digit \+
```

## Stage 2
This stage involves [automata](automata) folder which reads from [regex.txt](regex.txt) file and for each line, we will build a state machine, particulary a Definite Finite Automata (DFA). 

The folder structure of this stage is shown below : 

| File                           | Description                                                 | 
|--------------------------------|-------------------------------------------------------------| 
| **automata.hpp**               | Defines Automata class                                      | 
| **dfa.cpp**                    | Implements DFA logic, including build and minimization      |
| **dfa.hpp**                    | Declares DFA class, and minimized_DFA class                 |
| **fsm_elements.hpp**           | Defines core FSM components like `State`, and `Transition`  |
| **fsm.hpp**                    | Entry point for Stage 2 — builds DFAs from grammar.         |
| **nfa.cpp**                    | Implements NFA logic, including ε-transitions               |
| **nfa.hpp**                    | Declases NFA class                                          |
| **regex_postfix.cpp**          | Converts regular expressions to postfix form                |
| **regex_postfix.hpp**          | Declares toPostfix function and other dependent functions   |
| **utility.cpp**                | Flows through the overall program across different files    |
| **utility.hpp**                | Declares create_machine function                            |

The state machines built in this part will be used in next phase to identify various tokens and lexemes. I have explained the working in my other repository [REGEX to Automata](https://github.com/sauravatgithub-web/REGEX-to-Automata). There are little differences but the overall method remains same. Some of them are as follows - 
* This version accepts `string` as input while the directed repo takes `char` as input.
* toPostfix function in regex_postfix.hpp has been changed accordingly fitting to grammar and input rules   described above.
* There is no **print** function in **Automata** class here (it is in the directed repo). Similarly, we are not producing dot files or diagrams in this one.
* For the class **minimized_dfa** in **dfa.hpp**, final_states are now a set for we need them for easy search in later stages.

The work flow diagram in this stage looks as below :- 
```mermaid
flowchart LR
    R[regex.txt] --> HPP[automata.hpp]
    UH[utility.hpp] --> HPP
    UC[utility.cpp] --> UH
    RH[regex_postfix.hpp] --> UH
    RC[regex_postfix.cpp] --> RH
    NH[nfa.hpp] --> UH
    NC[nfa.cpp] -->NH
    A[automata.hpp] --> NH
    DH[dfa.hpp] --> UH
    DC[dfa.cpp] --> DH
    A --> DH
```

## Stage 3
This stage involves [lexical_analysis](lexical_analysis) folder which reads from the [code.txt](code.txt) file and for each line, we will build token utilizing the state machines developed in previous stage. We will be  performing lexical analysis of the code and create tokens. The token sequences generated will further be used for syntax analysis in next stages.

The folder structure of this stage is shown below : 

| File                           | Description                                                            | 
|--------------------------------|------------------------------------------------------------------------| 
| **analyzer.hpp**               | Entry point for stage 3 - recieves FSMs from stage 2 and builds tokens |
| **literal_table.hpp**          | Defines Literal Table class to store all literals for next stages      |
| **machine.hpp**                | Defines State Machine class for FSMs from Stage 1                      |
| **symbol_table.hpp**           | Defines Symbol Table class to store all identifiers for next stages    |
| **token.cpp**                  | Defines token class, token types, keywords and related functions       |
| **token.hpp**                  | Defines token class, token types and declares token_creator function   |

The work in this stage is comparatively easy from the previous stage. To keep it simple, we are parsing through each line of code and simultaneously updating `current_state` of our machines. When we are reaching final state for a machine ensuring that we are the following **maximum substring matching** rule, we build token and store it. While generating the token, if it identifies as identifier, we will store it in symbol table. On the other hanc, if it's numerical, floating, string, character or boolean constant, we will store it in literal table. Now, all the tokens generated will be passed to next stage for syntax analysis.

The work flow diagram in this stage looks as below :- 
```mermaid
flowchart LR
    L[literal_table.hpp] --> TH[token.hpp]
    S[symbol_table.hpp] --> TH
    TC[token.cpp] --> TH
    TH --> A[analyzer.hpp]
    M[machine.hpp] --> A
    C[code.txt] --> A
```
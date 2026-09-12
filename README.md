# GrammarParser

A C++ library that reads EBNF grammars and turns them into a symbol tree.

The user supplies grammar rules as text. The library tokenizes them and builds
a tree of `Symbol` nodes. In a later stage, that tree will be used to parse
arbitrary input against the grammar.

## Syntax

Each rule is written as `name : definition ;`

| Notation  | Meaning                       |
|-----------|-------------------------------|
| `'abc'`   | literal text (terminal)       |
| `name`    | reference to another rule     |
| `a b`     | sequence: a then b            |
| `a \| b`  | choice: either a or b         |
| `{ a }`   | repetition: zero or more      |
| `[ a ]`   | option: zero or one           |
| `( a )`   | grouping                      |

Items in a sequence are written side by side; there is no separator.

Escaping inside a terminal uses a backslash: `'\''`, `'\\'`

Example:

    number : digit { digit } ;
    digit  : '0' | '1' | '2' ;

## Architecture

    grammar text
        ↓  Lexer
    tokens, grouped per rule (TokensInExpression)
        ↓  Parser
    Symbol tree

The parser has three layers, which establish operator precedence:

    choice   : sequence { '|' sequence }
    sequence : term { term }
    term     : TERMINAL | IDENTIFIER | '(' choice ')' | '[' choice ']' | '{' choice '}'

So `a b | c` groups as `(a b) | c`.

## Layout

    src/
      lexer/
        Token.hpp     TokenType, Token, TokensInExpression
        Lexer.h       class declaration
        Lexer.cpp     implementation
      Symbol.h        Symbol hierarchy + toString
      Parser.h        parseTerm / parseSequence / parseChoice

    tests/
      CMakeLists.txt
      run-tests.py
      src/lexer/
        lexer-1.cpp   lexer tests
      src/parser/
        parser-1.cpp  parser tests

    main.cpp          scratch demo

## Status

Done:
- Lexer — splits text into tokens, tracks line/column, reports errors
- Symbol hierarchy with `toString` for inspection
- `parseTerm`, `parseSequence`, `parseChoice` — builds the tree for one rule
- Tests for both lexer and parser layers

Not done:
- Rule table (`map<string, Symbol>`) to resolve `ReferenceSymbol`
- A driver that walks every rule instead of just one
- `parse()` on the tree — matching actual input against the grammar
- Left recursion detection (`a : a b ;` would loop forever)

## Building

Tests:

    cd tests
    python3 run-tests.py

Demo:

    g++ -std=c++20 main.cpp src/lexer/Lexer.cpp -o main
    ./main

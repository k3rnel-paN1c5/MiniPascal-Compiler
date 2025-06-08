# MiniPascal Language Specification

## 1. Introduction

MiniPascal is a statically-typed, procedural programming language designed as a simplified subset of Pascal. It supports basic data types, structured control flow, and both functions and procedures. Its syntax is designed to be clear and easy to parse, making it an excellent language for educational compiler projects.

This document specifies the lexical rules (tokens) and the syntax (grammar) recognized by the MiniPascal compiler, based on its implementation in `parser.y` and its corresponding AST structure.

---

## 2. Lexical Analysis

The MiniPascal source code is first broken down into a series of tokens by the lexical analyzer.

### 2.1. Comments

Two styles of comments are supported and are ignored by the compiler:

* **Single-line comments:** Start with `//` and continue to the end of the line.
* **Multi-line comments:** Enclosed in curly braces `{ ... }`. These can span multiple lines but cannot be nested.

### 2.2. Identifiers

Identifiers are used to name programs, variables, functions, and procedures.

* Must begin with an alphabetic character (`a-z`, `A-Z`) or an underscore (`_`).
* Can be followed by any sequence of letters, numbers (`0-9`), or underscores.
* Identifiers are **case-insensitive** (e.g., `myVar`, `myvar`, and `MYVAR` refer to the same identifier).

### 2.3. Keywords

The following are reserved keywords and cannot be used as identifiers:

|             |           |           |           |
| :---------- | :-------- | :-------- | :-------- |
| `AND`       | `ARRAY`   | `BEGIN`   | `BOOLEAN` |
| `DIV`       | `DO`      | `ELSE`    | `END`     |
| `FUNCTION`  | `IF`      | `INTEGER` | `NOT`     |
| `OF`        | `OR`      | `PROCEDURE`| `PROGRAM` |
| `REAL`      | `THEN`    | `VAR`     | `WHILE`   |

### 2.4. Literals

* **Integer**: A sequence of one or more digits (e.g., `123`, `0`, `42`).
* **Real**: A sequence of digits followed by a decimal point and another sequence of digits (e.g., `3.14`, `0.5`, `123.0`).
* **Boolean**: The keywords `TRUE` or `FALSE`.

### 2.5. Operators and Delimiters

The following single and multi-character sequences are recognized as operators and delimiters:

| Symbol(s) | Description              |
| :-------- | :----------------------- |
| `+` `-` `*` `/` | Arithmetic Operators     |
| `:=`      | Assignment               |
| `=` `<>` `<` `>` `<=` `>=` | Relational Operators     |
| `.` `,` `:` `;` | Punctuation              |
| `(` `)`   | Parentheses              |
| `[` `]`   | Array Brackets           |

---

## 3. Syntax Analysis (Grammar)

The following grammar, presented in an EBNF-like format, defines the syntactic structure of a MiniPascal program.

### 3.1. Program Structure

```ebnf
program ::= 'PROGRAM' IDENTIFIER ';' declarations subprogram_declarations compound_statement '.'

declarations ::= 'VAR' declaration_list | <empty>

declaration_list ::= declaration | declaration_list declaration

declaration ::= identifier_list ':' type ';'

type ::= standard_type | array_type

standard_type ::= 'INTEGER' | 'REAL' | 'BOOLEAN'

array_type ::= 'ARRAY' '[' INTEGER_LITERAL '..' INTEGER_LITERAL ']' 'OF' standard_type

identifier_list ::= IDENTIFIER | identifier_list ',' IDENTIFIER
```

### 3.2. Subprograms

```ebnf
subprogram_declarations ::= subprogram_declarations subprogram_declaration | <empty>

subprogram_declaration ::= subprogram_head ';' declarations compound_statement

subprogram_head ::= function_head | procedure_head

function_head ::= 'FUNCTION' IDENTIFIER arguments ':' standard_type

procedure_head ::= 'PROCEDURE' IDENTIFIER arguments

arguments ::= '(' parameter_list ')' | <empty>

parameter_list ::= declaration_list
```

### 3.3. Statements

```ebnf
compound_statement ::= 'BEGIN' optional_statements 'END'

optional_statements ::= statement_list | <empty>

statement_list ::= statement | statement_list ';' statement

statement ::=
      variable ':=' expression
    | procedure_statement
    | compound_statement
    | 'IF' expression 'THEN' statement
    | 'IF' expression 'THEN' statement 'ELSE' statement
    | 'WHILE' expression 'DO' statement

procedure_statement ::= IDENTIFIER | IDENTIFIER '(' expression_list ')'
```

### 3.4. Expressions and Variables

```ebnf
expression_list ::= expression | expression_list ',' expression

variable ::= IDENTIFIER | IDENTIFIER '[' expression ']'

expression ::=
      simple_expression
    | simple_expression relop simple_expression

simple_expression ::=
      term
    | simple_expression addop term
    | sign term

term ::=
      factor
    | term mulop factor

factor ::=
      variable
    | IDENTIFIER '(' expression_list ')'
    | INTEGER_LITERAL
    | REAL_LITERAL
    | 'TRUE' 
    | 'FALSE'
    | '(' expression ')'
    | 'NOT' factor

sign ::= '+' | '-'
relop ::= '=' | '<>' | '<' | '<=' | '>' | '>='
addop ::= '+' | '-' | 'OR'
mulop ::= '*' | '/' | 'DIV' | 'AND'
```

### 3.5. Operator Precedence and Associativity

The grammar implicitly defines operator precedence. The following list is in order from **lowest to highest precedence**. All binary operators are **left-associative**.

1.  `OR`
2.  `AND`
3.  `=` `<>` `<` `>` `<=` `>=` (Relational)
4.  `+` `-` (Additive)
5.  `*` `/` `DIV` (Multiplicative)
6.  `NOT`, `+`, `-` (Unary)

Parentheses `()` can be used to explicitly control the order of evaluation.

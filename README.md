# MiniPascal Compiler

[![Language](https://img.shields.io/badge/language-C%2B%2B-blue.svg)](https://isocpp.org/)
[![Build Tools](https://img.shields.io/badge/build-Flex%20%26%20Bison-orange.svg)](https://www.gnu.org/software/bison/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

An educational compiler for a simplified Pascal-like language, "MiniPascal". This project demonstrates the core principles of compiler construction, from lexical analysis and parsing to semantic analysis and final code generation for a custom stack-based virtual machine.

The compiler is built using C++, Flex for lexical analysis, and Bison for parsing. It features a modular design centered around the Visitor pattern, making the different compilation passes (like type-checking and code generation) clean and easy to understand.

### MiniPascal Language at a Glance

Here is a small example of what MiniPascal code looks like:

```pascal
program FactorialRecursive;
var result, input : Integer;

// A recursive function to calculate factorial
function factorial(n: integer) : integer;
begin
  if n <= 1 then
    factorial := 1
  else
    factorial := n * factorial(n - 1)
end;

begin
    input := 5;
    result := factorial(input);
    write(result)
end.
```

## Compiler Pipeline

The project implements a classic compiler pipeline. The source code is processed through several distinct stages, each transforming the code into a lower-level representation.

```
MiniPascal Source Code (*.txt)
           |
           v
+--------------------+
|   Lexical Analysis | (Flex)
|  (scanner.lex)      |
+--------------------+
           |
           v
      Token Stream
           |
           v
+--------------------+
|   Syntax Analysis  | (Bison)
|   (parser.y)     |
+--------------------+
           |
           v
 Abstract Syntax Tree (AST)
           |
           v
+--------------------+
|  Semantic Analysis | (TypeVisitor)
|  & Symbol Tables   |
+--------------------+
           |
           v
  Validated & Annotated AST
           |
           v
+--------------------+
|   Code Generation  | (CodeGenVisitor)
+--------------------+
           |
           v
  Virtual Machine Code (*.vm)
```

## How It Works

1.  **Lexical Analysis (`compiler.l`):** The Flex-based lexer scans the raw MiniPascal source code and converts it into a stream of tokens (e.g., `KEYWORD_PROGRAM`, `IDENTIFIER`, `INTEGER_LITERAL`).

2.  **Syntax Analysis (`compiler.y`):** The Bison-based parser consumes the token stream. It validates the code against the language's grammar and builds an **Abstract Syntax Tree (AST)**. The AST is a tree-like data structure that represents the syntactic structure of the program.

3.  **Semantic Analysis (`TypeVisitor.cpp`):** This is the "meaning-checking" phase. The `TypeVisitor` traverses the AST and uses a **Scoped Symbol Table** to:
    * Verify that all identifiers are declared before they are used.
    * Enforce type compatibility for assignments, expressions, and function parameters.
    * Handle function overloading and check for correct return types.
    * Issue warnings for safe implicit casts (e.g., `Integer` to `Real`).

4.  **Code Generation (`CodeGenVisitor.cpp`):** Once the AST is semantically validated, the `CodeGenVisitor` traverses it one final time. It translates each node into one or more assembly instructions for our target **stack-based Virtual Machine**, writing the final executable code to a `.vm` file.

## Language Specification (MiniPascal)

MiniPascal is a statically-typed, procedural language. The full grammar is specified in the [parser specification document](docs/MiniPascalLanguageSpecifications.md).

* **Data Types**: `Integer`, `Real`, `Boolean`, and one-dimensional `Array`s of these types.
* **Control Flow**: `if-then`, `if-then-else`, and `while-do` statements.
* **Operators**:
    * Arithmetic: `+`, `-`, `*`, `/`, `div`
    * Relational: `=`, `<>`, `>`, `<`, `>=`, `<=`
    * Logical: `and`, `or`, `not`
* **Subprograms**: Supports both `function`s (which return a value) and `procedure`s. Function overloading based on parameter types is also supported.
* **Comments**: Supports multi-line `{ ... }` comments and single-line `//` comments.

## Target Virtual Machine

The compiler generates code for a custom stack-based virtual machine (VM). The VM is a simple but capable target that supports:

* Integer, Real, and Address value types.
* Stack-based arithmetic and logic operations (e.g., `ADD`, `FADD`, `EQUAL`).
* Instructions for memory access (`PUSHG`, `STOREL`), control flow (`JUMP`, `JZ`), and function calls (`CALL`, `RETURN`).
* Built-in I/O operations like `WRITEI` and `WRITEF`.

The full specification can be found in the [VM specification document](docs/VirutalMachineSpecification.md).

## Project Structure

| Path         | Description                                                                    |
| :----------- | :----------------------------------------------------------------------------- |
| `build/`     | Output directory for the compiled executable and generated `.vm` files.        |
| `docs/`      | Contains language and virtual machine specification documents.                 |
| `include/`   | Header files (`.h`) defining the AST, Visitors, Symbol Table, etc.             |
| `src/`       | C++ source files (`.cpp`) implementing the compiler's logic.                   |
| `tests/`     | A collection of MiniPascal test files (`.txt`) demonstrating language features.|
| `compiler.l` | The Flex file defining lexical analysis rules.                                 |
| `compiler.y` | The Bison file defining the language grammar and parsing rules.                |
| `Makefile`   | The build script for compiling the project.                                    |

## Getting Started

### Prerequisites

* **C++ Compiler**: G++ (GNU C++ Compiler) is recommended.
* **Flex**: A tool for generating lexical analyzers.
* **Bison**: A parser generator.
* **Make**: The build automation tool.

### Building the Compiler

The provided `Makefile` automates the build process.

1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/k3rnel-paN1c5/MiniPascal-Compiler.git](https://github.com/k3rnel-paN1c5/MiniPascal-Compiler.git)
    cd MiniPascal-Compiler
    ```

2.  **Run `make` to build the compiler:**
    ```bash
    make
    ```
    This command compiles all sources and places the `compiler` executable in the `build/` directory.

3.  **Run `make clean` to remove build artifacts:**
    ```bash
    make clean
    ```

## How to Run

Execute the compiler from the project's root directory.

* **To compile a MiniPascal file:**
    ```bash
    ./build/compiler tests/test_comprehensive.txt
    ```

* **To specify an output file for the generated VM code:**
    ```bash
    ./build/compiler tests/test_comprehensive.txt -o my_program.vm
    ```

* **To compile from standard input (press Ctrl+D to end input):**
    ```bash
    ./build/compiler
    ```

## Contributing

Contributions are welcome! If you have suggestions for improving the code or documentation, please feel free to create an issue or submit a pull request.

1.  Fork the Project
2.  Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3.  Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4.  Push to the Branch (`git push origin feature/AmazingFeature`)
5.  Open a Pull Request

## License

This project is open source and available under the **MIT License**. See the `LICENSE` file for more information.

## Acknowledgments

* This project was developed as part of a university course on compiler construction.
* Inspiration and guidance from the classic "Dragon Book" (*Compilers: Principles, Techniques, and Tools*).
* The Flex & Bison documentation.


# MiniPascal-Compiler

Building a compiler for a simplified version of the Pascal programming language. This project aims to provide a clear and educational example of compiler construction, covering lexical analysis, parsing, abstract syntax tree (AST) generation, semantic analysis (type checking), and potentially code generation.

## Table of Contents

- [Features](#features)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Cloning the Repository](#cloning-the-repository)
  - [Building the Compiler](#building-the-compiler)
- [How to Run](#how-to-run)
- [Project Structure](#project-structure)
- [Language Specification (MiniPascal)](#language-specification-minipascal)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgments](#acknowledgments)

## Features

* **Lexical Analysis:** Tokenization of MiniPascal source code using Flex.
* **Parsing:** Syntax analysis using Bison to build the Abstract Syntax Tree (AST).
* **Abstract Syntax Tree (AST):** A robust AST representation for MiniPascal programs.
* **Semantic Analysis:**
    * **Symbol Table Management:** Implemented for scope handling and identifier lookup.
    * **Type Checking:** Ensures type compatibility and detects semantic errors in expressions and assignments.
* **Error Handling:** Comprehensive reporting of lexical, syntax, and semantic errors with line and column information.
* **Modular Design:** Utilizes a Visitor pattern for AST traversal, allowing easy extension for new compiler passes (e.g., code generation, optimization).

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

Before you begin, ensure you have the following tools installed on your system:

* **C++ Compiler:** G++ (GNU C++ Compiler) is recommended.
    * On Ubuntu/Debian: `sudo apt update && sudo apt install build-essential`
    * On macOS: Install Xcode Command Line Tools: `xcode-select --install`
    * On Windows: MinGW-w64 or WSL (Windows Subsystem for Linux) with a Linux distribution.
* **Flex (Lexical Analyzer Generator):**
    * On Ubuntu/Debian: `sudo apt install flex`
    * On macOS (via Homebrew): `brew install flex`
* **Bison (Parser Generator):**
    * On Ubuntu/Debian: `sudo apt install bison`
    * On macOS (via Homebrew): `brew install bison`
* **Make:** Usually comes with `build-essential` or Xcode Command Line Tools.

### Cloning the Repository

```bash
git clone [https://github.com/k3rnel-paN1c5/MiniPascal-Compiler.git](https://github.com/k3rnel-paN1c5/MiniPascal-Compiler.git)
cd MiniPascal-Compiler
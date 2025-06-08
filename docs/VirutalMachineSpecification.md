# Virtual Machine Specification

## 1. Description

### 1.1. Machine Organization

The machine is a **stack-based machine**, as opposed to a register-based one. It consists of:
* An instruction stack (for code).
* A call stack.
* An execution stack (for values).
* Structured memory blocks.
* Four main registers: `pc`, `sp`, `fp`, and `gp`.

The **execution stack** holds `Values`, which can be of type `integer`, `real`, or an `address`.

The **memory** is composed of two main segments:
1.  A segment for character strings.
2.  A segment for structured blocks of data.

Both segments are accessed via addresses stored on the execution stack. A structured block contains a sequence of values of the same type.

An `address` can point to one of four locations:
* The instruction stack (a `Code` pointer).
* The execution stack.
* A structured memory block.
* A character string.

#### Registers

The machine's state is managed by three primary registers that point to different parts of the execution stack:

* `sp` **(stack pointer)**: Points to the first available (empty) slot at the top of the stack.
* `fp` **(frame pointer)**: Points to the base of the current stack frame, used to access local variables.
* `gp` **(global pointer)**: Points to the base address for global variables.

The **call stack** is used to manage subprogram (function/procedure) calls. When a call is made, it saves the context by pushing a pair of pointers: the current `pc` (program counter) and `fp` (frame pointer).

---

## 2. Instructions

Instructions can take zero, one, or two operands. Operands can be:
* Integer literals (e.g., `10`, `-5`).
* Real literals (e.g., `3.14`, `-0.5`).
* String literals enclosed in double quotes (e.g., `"Hello"`), supporting standard C escape sequences (`\n`, `\t`, `\\`).
* Symbolic labels representing a code address.

### 2.1. Basic Stack Terminology

* **Pushing**: Storing a value at the location `P[sp]` and incrementing `sp`.
* **Popping**: Decrementing `sp` to remove values from the stack.
* **Top of the Stack**: Refers to the last value pushed, located at `P[sp-1]`.
* **Second on Stack**: Refers to the value below the top, at `P[sp-2]`.

### 2.2. Operations

Arithmetic and logical operations typically pop two values from the stack, perform the operation, and push the result back. Comparison operations push an integer `1` for `true` or `0` for `false`.

#### Integer Arithmetic

| Instruction | Description                                  |
| :---------- | :------------------------------------------- |
| `ADD`       | Pop `n` and `m`, push `m + n`.                   |
| `SUB`       | Pop `n` and `m`, push `m - n`.                   |
| `MUL`       | Pop `n` and `m`, push `m * n`.                   |
| `DIV`       | Pop `n` and `m`, push `m / n` (integer division). |
| `MOD`       | Pop `n` and `m`, push `m mod n`.                 |
| `NOT`       | Pop `n`, push `1` if `n` is `0`, else `0`.       |

#### Integer Comparison

| Instruction | Description                                  |
| :---------- | :------------------------------------------- |
| `INF`       | Pop `n` and `m`, push `1` if `m < n`, else `0`.    |
| `INFEQ`     | Pop `n` and `m`, push `1` if `m <= n`, else `0`.   |
| `SUP`       | Pop `n` and `m`, push `1` if `m > n`, else `0`.    |
| `SUPEQ`     | Pop `n` and `m`, push `1` if `m >= n`, else `0`.   |

#### Real Arithmetic & Comparison

| Instruction | Description                                  |
| :---------- | :------------------------------------------- |
| `FADD`      | Pop `n` and `m`, push `m + n`.                   |
| `FSUB`      | Pop `n` and `m`, push `m - n`.                   |
| `FMUL`      | Pop `n` and `m`, push `m * n`.                   |
| `FDIV`      | Pop `n` and `m`, push `m / n`.                   |
| `FINF`      | Pop `n` and `m`, push `1` if `m < n`, else `0`.    |
| `FINFEQ`    | Pop `n` and `m`, push `1` if `m <= n`, else `0`.   |
| `FSUP`      | Pop `n` and `m`, push `1` if `m > n`, else `0`.    |
| `FSUPEQ`    | Pop `n` and `m`, push `1` if `m >= n`, else `0`.   |

#### Equality

| Instruction | Description                                                               |
| :---------- | :------------------------------------------------------------------------ |
| `EQUAL`     | Pop `n` and `m`, push `1` if `n == m`, else `0`. (Values must be same type). |

#### String & Block Operations

| Instruction | Description                                                                |
| :---------- | :------------------------------------------------------------------------- |
| `CONCAT`    | Pop two string addresses, push address of new concatenated string.         |
| `ALLOC n`   | Allocate a block of size `n`, push its address.                            |
| `ALLOCN`    | Pop `n`, allocate a block of size `n`, push its address.                   |
| `FREE`      | Pop an address, free the corresponding memory block.                       |

#### Type Conversions

| Instruction | Description                                                                 |
| :---------- | :-------------------------------------------------------------------------- |
| `ITOF`      | Pop an integer, push its real (float) representation.                       |
| `FTOI`      | Pop a real, push its integer part (truncating).                             |
| `ATOI`      | Pop a string address, push its integer conversion. Fails if invalid.        |
| `ATOF`      | Pop a string address, push its real conversion. Fails if invalid.           |
| `STRI`      | Pop an integer, push the address of its new string representation.          |
| `STRF`      | Pop a real, push the address of its new string representation.              |

### 2.3. Data Movement

#### Pushing to the Stack

| Instruction | Description                                         |
| :---------- | :-------------------------------------------------- |
| `PUSHI n`   | Push integer literal `n`.                             |
| `PUSHN n`   | Push the integer `0` onto the stack `n` times.        |
| `PUSHF n`   | Push real literal `n`.                                |
| `PUSHG n`   | Push the global variable at `gp[n]`.                  |
| `PUSHL n`   | Push the local variable at `fp[n]`.                   |
| `PUSHSP`    | Push the value of the `sp` register.                  |
| `PUSHFP`    | Push the value of the `fp` register.                  |
| `PUSHGP`    | Push the value of the `gp` register.                  |
| `LOAD n`    | Pop address `a`, push value at `a[n]`.                |
| `LOADN`     | Pop `n`, pop address `a`, push value at `a[n]`.         |
| `DUP n`     | Duplicate the top `n` values on the stack.            |
| `DUPN`      | Pop `k`, duplicate the top `k` values.                |

#### Popping from the Stack

| Instruction | Description                           |
| :---------- | :------------------------------------ |
| `POP n`     | Pop `n` values from the stack.        |
| `POPN`      | Pop `k`, then pop `k` more values.    |

#### Storing Data

| Instruction | Description                                      |
| :---------- | :----------------------------------------------- |
| `STOREL n`  | Pop a value, store it in local variable `fp[n]`.   |
| `STOREG n`  | Pop a value, store it in global variable `gp[n]`.  |
| `STORE n`   | Pop value `v`, pop address `a`, store `v` at `a[n]`. |
| `STOREN`    | Pop `v`, `k`, and `a`, store `v` at `a[k]`.          |

### 2.4. Control Flow

| Instruction | Description                                                              |
| :---------- | :----------------------------------------------------------------------- |
| `JUMP L`    | Set `pc` to the address of label `L`.                                      |
| `JZ L`      | Pop a value. If it is `0`, jump to label `L`.                              |
| `PUSHA L`   | Push the code address of label `L` onto the stack.                         |
| `CALL`      | Pop address `a`, save `pc` and `fp` to call stack, set `fp=sp`, set `pc=a`. |
| `RETURN`    | Restore `fp` and `pc` from the call stack, effectively returning.          |

### 2.5. Program Lifecycle & I/O

| Instruction | Description                                                              |
| :---------- | :----------------------------------------------------------------------- |
| `START`     | Initializes the first stack frame: `fp = sp`. Used once.                 |
| `STOP`      | Halt program execution.                                                  |
| `NOP`       | No operation.                                                            |
| `ERR "msg"` | Halt execution and print an error message.                               |
| `READ`      | Read a line from stdin, store as a string, and push its address.         |
| `WRITEI`    | Pop and print an integer.                                                |
| `WRITEF`    | Pop and print a real.                                                    |
| `WRITES`    | Pop a string address and print the string.                               |

### 2.6. Miscellaneous

| Instruction    | Description                                                     |
| :------------- | :-------------------------------------------------------------- |
| `CHECK n p`    | Verify the integer on top of the stack `i` is in range `n <= i <= p`. |
| `SWAP`         | Swap the top two values on the stack.                           |

---

## 3. VM Language Specification

### 3.1. Lexical Rules

* **Whitespace**: Space, tab, and newline characters are ignored.
* **Comments**: Single-line comments start with `//`.
* **Identifiers**: `(<Alpha>|_) (<Alpha> | <Digit> | _ )*`
* **Integers**: `-? <Digit>+`
* **Reals**: `-? <Digit>+ (. <Digit>*)? ((e | E) (- | +)? <Digit>+)?`
* **Strings**: `"([^"]|\")*"`


### 3.2. Usage

The VM is executed from the command line:

On Windows
```bash
vm.exe [options] [file.vm]
```
On Linux/Macos (need wine to run exe files)
```bash
wine vm.exe [options] [file.vm]
```

Options:

| Option | Description                                                              |
| :---------- | :----------------------------------------------------------------------- |
| `dump`     | Display stack/memory info after execution.                 |
| `silent`      | Suppress all output except for write* instructions.                                                  |
| `count`       | Print the number of instructions executed.                                                            |
| `ssize` | Set the execution stack size (default: 1000).                               |
| `csize`      | Set the call stack size (default: 100).         |                        

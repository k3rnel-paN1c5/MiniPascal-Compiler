/**
 * @file ast.h
 * @brief Abstract Syntax Tree (AST) definition for a Pascal-like programming language
 *
 * This header defines the complete AST class hierarchy for parsing and representing
 * the structure of a Pascal-like programming language. It includes classes for 
 * different language constructs such as expressions, statements, declarations, 
 * and supports the Visitor design pattern for traversing the AST.
 *
 * Key components include:
 * - Node: Base class for all AST nodes
 * - Various expression and statement classes (Exp, Stmt, etc.)
 * - Type system representation (StdType, Array)
 * - Visitor interface for AST traversal
 * - Symbol table classes for semantic analysis
 */

#ifndef AST_H
#define AST_H

#include <vector>
#include "hash_fun.h"
#include "hash_table.h"
using namespace std;

// Forward declarations of all AST node 
class Node;
class Stmt;
class Prog;
class Ident;
class Decs;
class ParDec;
class IdentList;
class SubDecs;
class SubDec;
class SubHead;
class LocalDecs;
class LocalDec;
class Func;
class Args;
class ParList;
class Proc;
class CompStmt;
class OptionalStmts;
class StmtList;
class Var;
class Exp;
class Assign;
class ProcStmt;
class ExpList;
class IfThen;
class IfThenElse;
class While;
class Type;
class StdType;
class IdExp;
class Integer;
class Real;
class Bool;
class Array;
class BinOp;
class Add;
class Sub;
class Mult;
class Divide;
class Mod;
class GT;
class LT;
class GE;
class LE;
class ET;
class NE;
class And;
class Or;
class Not;
class Visitor;
class Symbol;

/**
 * @enum TypeEnum
 * @brief Represents the basic types supported in the language
 */
enum TypeEnum 
{
    INTTYPE, ///< Integer type
    REALTYPE, ///< Real (floating point) type
    BOOLTYPE,  ///< Boolean type
    INT_ARRAY,
    REAL_ARRAY,
    BOOL_ARRAY,
    VOID
};

/**
 * @enum OpType
 * @brief Represents all binary and comparison operators in the language
 */
enum OpType {
    OP_ADD, OP_SUB, OP_MULT, OP_DIV, OP_MOD, //< Arithmetic operators
    OP_GT, OP_GE, OP_LT, OP_LE, OP_ET, OP_NE, //< Comparison operators
    OP_AND, OP_OR ///< Logical operators
};

/**
 * @brief Converts a TypeEnum value to its string representation
 * @param type The TypeEnum to convert
 * @return String representation of the type
 */
string TypeEnumToString(TypeEnum );

/**
 * @class Node
 * @brief Base class for all AST nodes
 * 
 * Provides common attributes like line and column for error reporting,
 * and implements the Visitor pattern through the accept method.
 */
class Node 
{
public:
    int line; ///< Source line number for error 
    int column; ///< Source column number for error 
    Node *father; ///< Parent node in the AST
    /**
     * @brief Constructor for Node
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    Node(int lin, int col);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class Prog
 * @brief Represents the entire program
 * 
 * Root node of the AST that contains program name, declarations,
 * sub-declarations (functions/procedures), and the main compound statement.
 */
class Prog : public Node 
{
public: 
    Ident *name;                  ///< Program name
    Decs *declarations;           ///< Variable declarations
    SubDecs *subDeclarations;     ///< Function and procedure declarations
    CompStmt *compoundStatment;   ///< Main program body
    /**
     * @brief Constructor for Prog
     * @param n for Program name
     * @param decl variable declarations
     * @param subdecl function and procedure declarations
     * @param comst Main program body
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    Prog(Ident *, Decs *, SubDecs *, CompStmt *, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* );
 };

/**
 * @class Ident
 * @brief Represents an identifier (variable, function, procedure name)
 */
class Ident : public Node
{
public:
    string name; ///< The identifier name
    Symbol* symbol; ///< Corresponding symbol in symbol table
    Ident(string, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class Decs
 * @brief Represents a collection of variable declarations
 * 
 * Contains a vector of parameter declarations (ParDec) that define
 * variables in a program, function, or procedure.
 */
class Decs : public Node 
{
public:
    vector<ParDec *> * decs; ///< List of parameter declarations
    /**
     * @brief Constructor for Decs
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    Decs(int, int);
    /**
     * @brief Adds a parameter declaration to the list
     * @param parDec The parameter declaration to add
     */
    void AddDec(ParDec*); 
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class ParDec
 * @brief Represents a parameter declaration
 * 
 * Contains an identifier list and a type, defining one or more
 * variables of the same type in a declaration statement.
 */
class ParDec : public Node
{
public:
    IdentList* identList; ///< List of identifiers being declared
    Type* tp; ///< Type of the declared identifiers
    /**
     * @brief Constructor for ParDec
     * @param idlst List of identifiers being declared
     * @param typ Type of the declared identifiers
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    ParDec(IdentList*, Type *, int, int);
    
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class IdentList
 * @brief Represents a list of identifiers
 * 
 * Contains a vector of identifiers that are being declared
 * with the same type in a declaration statement.
 */
class IdentList : public Node
{
public:
    vector<Ident *> *identLst; ///< List of identifiers
    /**
     * @brief Constructor for IdentList
     * @param id Initial identifier in the list
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    IdentList(Ident*, int, int);
    /**
     * @brief Adds an identifier to the list
     * @param id The identifier to add
     */
    void AddIdent(Ident* );
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class Type
 * @brief Base class for type representations
 * 
 * Abstract base class for different types in the language.
 */
class Type : public Node
{
public:
    /**
     * @brief Constructor for Type
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    Type(int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class StdType
 * @brief Represents a standard type in the language
 * 
 * Contains a TypeEnum value that specifies which standard type
 * (integer, real, or boolean) this node represents.
 */
class StdType : public Type
{
public:
    TypeEnum type; ///< The standard type (INTTYPE, REALTYPE, or BOOLTYPE)
    /**
     * @brief Constructor for StdType
     * @param tp The TypeEnum value representing the standard type
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    StdType(TypeEnum, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class Array
 * @brief Represents an array type in the language
 * 
 * Contains the array bounds (begin and end indices) and the
 * standard type of the array elements.
 */
class Array : public Type
{
public:
    int beginIndex; ///< Starting index of the array
    int endIndex; ///< Ending index of the array
    StdType* stdType; ///< Type of the array elements
    /**
     * @brief Constructor for Array
     * @param beg Starting index of the array
     * @param end Ending index of the array
     * @param tp Type of the array elements
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    Array(int, int, StdType* , int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class SubDecs
 * @brief Represents a collection of subprogram declarations
 * 
 * Contains a vector of function and procedure declarations (SubDec)
 * that define the subprograms in a Pascal program.
 */
class SubDecs : public Node 
{
public:
    vector<SubDec*>* subdecs; ///< List of subprogram declarations
    /**
     * @brief Constructor for SubDecs
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    SubDecs(int, int);
    /**
     * @brief Adds a subprogram declaration to the list
     * @param sd The subprogram declaration to add
     */
    void AddDec(SubDec*);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class SubDec
 * @brief Represents a subprogram declaration
 * 
 * Contains a subprogram header and a compound statement that
 * define a function or procedure in a Pascal program.
 */
class SubDec : public Node 
{
public: 
    SubHead* subHead; ///< Subprogram header (function or procedure)
    LocalDecs* localDecs; ///< Local variables for function scope
    CompStmt* compStmt; ///< Compound statement forming the subprogram body
    /**
     * @brief Constructor for SubDec
     * @param head Subprogram header
     * @param locdecs Locael variable declaration
     * @param cmst Compound statement forming the subprogram body
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    SubDec(SubHead*, LocalDecs*, CompStmt*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class SubHead
 * @brief Base class for subprogram headers
 * 
 * Abstract base class for function and procedure headers.
 */
class SubHead : public Node
{
public:
    /**
     * @brief Constructor for SubHead
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    SubHead(int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class Func
 * @brief Represents a function declaration header
 * 
 * Contains the function name, arguments, and return type.
 */
class Func : public SubHead 
{
public:
    Ident* id; ///< Function name
    Args* args; ///< Function arguments
    StdType* typ; ///< Function return type
    /**
     * @brief Constructor for Func
     * @param ident Function name
     * @param ags Function arguments
     * @param tp Function return type
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    Func(Ident*, Args*, StdType*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};
/**
 * @class Proc
 * @brief Represents a procedure declaration header
 * 
 * Contains the procedure name and arguments.
 */
class Proc : public SubHead 
{
public:
    Ident* id;  ///< Procedure name
    Args* args; ///< Procedure arguments
    /**
     * @brief Constructor for Proc
     * @param ident Procedure name
     * @param ags Procedure arguments
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    Proc(Ident*, Args*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class Args
 * @brief Represents the arguments of a subprogram
 * 
 * Contains a parameter list that defines the formal parameters
 * of a function or procedure.
 */
class Args : public Node 
{
public:
    ParList* parList; ///< List of parameters
    /**
     * @brief Constructor for Args
     * @param parls List of parameters
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    Args(ParList*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class ParList
 * @brief Represents a list of parameters
 * 
 * Contains a vector of parameter declarations that define
 * the formal parameters of a function or procedure.
 */
class ParList : public Node
{
public:
    vector<ParDec*> * parList; ///< List of parameter declarations
    /**
     * @brief Constructor for ParList
     * @param prDec Initial parameter declaration
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    ParList(ParDec*, int, int);
    /**
     * @brief Adds a parameter declaration to the list
     * @param prDec The parameter declaration to add
     */
    void AddDec(ParDec*);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};
class LocalDecs: public Node
{
public: 
    vector<LocalDec*>* localDecs;
    LocalDecs(int, int);
    void AddDec(LocalDec*);

    virtual void accept(Visitor* ); 
};
class LocalDec: public Node
{
public:
    IdentList* identlist;
    Type* tp;
    LocalDec(IdentList*, Type*, int, int);

    virtual void accept(Visitor* ); 
};
/**
 * @class OptionalStmts
 * @brief Represents an optional list of statements
 * 
 * Contains a statement list that may be empty in certain
 * language constructs like compound statements.
 */
class OptionalStmts : public Node
{
    public:
    StmtList* stmtList; ///< List of statements (may be null)
    /**
     * @brief Constructor for OptionalStmts
     * @param stls List of statements (may be null)
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    OptionalStmts(StmtList*,int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class StmtList
 * @brief Represents a list of statements
 * 
 * Contains a vector of statements that form a sequence
 * of executable code in the program.
 */
class StmtList : public Node 
{
    public:
    vector<Stmt*> * stmts; ///< List of statements
    /**
     * @brief Constructor for StmtList
     * @param st Initial statement in the list
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    StmtList(Stmt*, int, int);
    /**
     * @brief Adds a statement to the list
     * @param st The statement to add
     */
    void AddStmt(Stmt*);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};
/**
 * @class Stmt
 * @brief Base class for statements
 * 
 * Abstract base class for different types of statements in the language.
 */
class Stmt : public Node 
{
    public:    
    /**
     * @brief Constructor for Stmt
     * @param lin Line number in source code
     * @param col Column number in source code
     */ 
    Stmt(int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class CompStmt
 * @brief Represents a compound statement
 * 
 * Contains a list of statements enclosed in begin-end blocks.
 */
class CompStmt : public Stmt 
{
public:
    OptionalStmts* optitonalStmts; ///< List of statements (may be empty)
    /**
     * @brief Constructor for CompStmt
     * @param opst List of statements (may be empty)
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    CompStmt(OptionalStmts*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class Assign
 * @brief Represents an assignment statement
 * 
 * Contains a variable and an expression to be assigned to it.
 */
class Assign : public Stmt
{
public:
    Var* var; ///< Target variable
    Exp* exp; ///< Expression to assign
    /**
     * @brief Constructor for Assign
     * @param vr Target variable
     * @param ex Expression to assign
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    Assign(Var*, Exp*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class ProcStmt
 * @brief Represents a procedure call statement
 * 
 * Contains a procedure name and a list of arguments.
 */
class ProcStmt : public Stmt
{
public:
    Ident* id; ///< Procedure name
    ExpList* expls; ///< List of argument expressions
    /**
     * @brief Constructor for ProcStmt
     * @param ident Procedure name
     * @param exls List of argument expressions
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    ProcStmt(Ident*, ExpList*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};
/**
 * @class ExpList
 * @brief Represents a list of expressions
 * 
 * Contains a vector of expressions used as arguments in
 * function or procedure calls.
 */
class ExpList : public Node
{
public:
    vector<Exp*> * expList; ///< List of expressions
    /**
     * @brief Constructor for ExpList
     * @param ex Initial expression in the list
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    ExpList(Exp*, int, int);
    /**
     * @brief Adds an expression to the list
     * @param ex The expression to add
     */
    void AddExp(Exp*);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};
/**
 * @class Exp
 * @brief Base class for expressions
 * 
 * Abstract base class for different types of expressions in the language.
 */
class Exp : public Node
{
public:
    TypeEnum type;
    /**
     * @brief Constructor for Exp
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    Exp(int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class IdExp
 * @brief Represents an identifier expression
 * 
 * Contains an identifier that refers to a variable in an expression.
 */
class IdExp : public Exp
{
public:
    Ident* id; ///< Variable identifier
    /**
     * @brief Constructor for IdExp
     * @param ident Variable identifier
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    IdExp(Ident*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class Integer
 * @brief Represents an integer literal expression
 * 
 * Contains an integer value.
 */
class Integer : public Exp
{
public:
    int val; ///< Integer value
    /**
     * @brief Constructor for Integer
     * @param value Integer value
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    Integer(int, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};
/**
 * @class Real
 * @brief Represents a real (floating point) literal expression
 * 
 * Contains a floating point value.
 */
class Real : public Exp
{
public:
    float val; ///< Floating point value
    /**
     * @brief Constructor for Real
     * @param value Floating point value
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    Real(float, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class Bool
 * @brief Represents a boolean literal expression
 * 
 * Contains a boolean value.
 */
class Bool : public Exp
{
public:
    bool val; ///< Boolean value
    /**
     * @brief Constructor for Bool
     * @param value Boolean value
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    Bool(bool, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class FuncCall
 * @brief Represents a function call expression
 * 
 * Contains an identifier and a list of expressions that
 * represent the arguments to a function call.
 */
class FuncCall : public Exp
{
public:
    Ident* id; ///< Function identifier
    ExpList* exps;  ///< List of argument expressions
    /**
     * @brief Constructor for FuncCall
     * @param ident Function identifier
     * @param exls List of argument expressions
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    FuncCall(Ident*, ExpList*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class Not
 * @brief Represents a logical NOT operation
 * 
 * Contains an expression to be negated.
 */
class Not: public Exp
{
public:
    Exp* exp; ///< Expression to negate
    /**
     * @brief Constructor for Not
     * @param e Expression to negate
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    Not(Exp* , int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class BinOp
 * @brief Base class for binary operations
 * 
 * Contains left and right expressions for a binary operation.
 * This is the parent class for all specific binary operations.
 */
class BinOp : public Exp 
{
public:
    Exp* leftExp; ///< Left operand expression
    Exp* rightExp; ///< Right operand expression
    /**
     * @brief Constructor for BinOp
     * @param lexp Left operand expression
     * @param rexp Right operand expression
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    BinOp(Exp*, Exp*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};
/**
 * @class Add
 * @brief Represents an addition operation
 * 
 * Binary operation for adding two expressions.
 */
class Add : public BinOp 
{
public:
    /**
     * @brief Constructor for Add
     * @param lexp Left operand expression
     * @param rexp Right operand expression
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    Add(Exp*, Exp*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class Sub
 * @brief Represents a subtraction operation
 * 
 * Binary operation for subtracting the right expression from the left.
 */
class Sub : public BinOp 
{
public:
    /**
     * @brief Constructor for Sub
     * @param lexp Left operand expression
     * @param rexp Right operand expression
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    Sub(Exp*, Exp*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};
/**
 * @class Mult
 * @brief Represents a multiplication operation
 * 
 * Binary operation for multiplying two expressions.
 */
class Mult : public BinOp 
{
public:
    /**
     * @brief Constructor for Mult
     * @param lexp Left operand expression
     * @param rexp Right operand expression
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    Mult(Exp*, Exp*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};
/**
 * @class Divide
 * @brief Represents a division operation
 * 
 * Binary operation for dividing the left expression by the right.
 */
class Divide : public BinOp 
{
public:
    /**
     * @brief Constructor for Divide
     * @param lexp Left operand expression
     * @param rexp Right operand expression
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    Divide(Exp*, Exp*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class Mod
 * @brief Represents a modulo operation
 * 
 * Binary operation for computing the remainder when dividing
 * the left expression by the right.
 */

class Mod : public BinOp 
{
public:
    /**
     * @brief Constructor for Mod
     * @param lexp Left operand expression
     * @param rexp Right operand expression
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    Mod(Exp*, Exp*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class GT
 * @brief Represents a greater than comparison
 * 
 * Binary operation for checking if the left expression is
 * greater than the right expression.
 */
class GT : public BinOp 
{
public:
    /**
     * @brief Constructor for GT
     * @param lexp Left operand expression
     * @param rexp Right operand expression
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    GT(Exp*, Exp*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class LT
 * @brief Represents a less than comparison
 * 
 * Binary operation for checking if the left expression is
 * less than the right expression.
 */
class LT : public BinOp 
{
public:
     /**
     * @brief Constructor for LT
     * @param lexp Left operand expression
     * @param rexp Right operand expression
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    LT(Exp*, Exp*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class GE
 * @brief Represents a greater than or equal comparison
 * 
 * Binary operation for checking if the left expression is
 * greater than or equal to the right expression.
 */
class GE : public BinOp 
{
public:
    /**
     * @brief Constructor for GE
     * @param lexp Left operand expression
     * @param rexp Right operand expression
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    GE(Exp*, Exp*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class LE
 * @brief Represents a less than or equal comparison
 * 
 * Binary operation for checking if the left expression is
 * less than or equal to the right expression.
 */
class LE : public BinOp 
{
public:
    /**
     * @brief Constructor for LE
     * @param lexp Left operand expression
     * @param rexp Right operand expression
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    LE(Exp*, Exp*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class ET
 * @brief Represents an equality comparison
 * 
 * Binary operation for checking if the left expression is
 * equal to the right expression.
 */
class ET : public BinOp 
{
public:
    /**
     * @brief Constructor for ET
     * @param lexp Left operand expression
     * @param rexp Right operand expression
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    ET(Exp*, Exp*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class NE
 * @brief Represents a not equal comparison
 * 
 * Binary operation for checking if the left expression is
 * not equal to the right expression.
 */
class NE : public BinOp 
{
public:
    /**
     * @brief Constructor for NE
     * @param lexp Left operand expression
     * @param rexp Right operand expression
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    NE(Exp*, Exp*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class And
 * @brief Represents a logical AND operation
 * 
 * Binary operation for the logical AND of two expressions.
 */
class And : public BinOp 
{
public:
    /**
     * @brief Constructor for And
     * @param lexp Left operand expression
     * @param rexp Right operand expression
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    And(Exp*, Exp*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class Or
 * @brief Represents a logical OR operation
 * 
 * Binary operation for the logical OR of two expressions.
 */
class Or : public BinOp 
{
public:
    /**
     * @brief Constructor for Or
     * @param lexp Left operand expression
     * @param rexp Right operand expression
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    Or(Exp*, Exp*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class IfThen
 * @brief Represents an if-then statement
 * 
 * Contains a condition expression and a statement to execute
 * if the condition is true.
 */
class IfThen : public Stmt
{
public: 
    Exp* expr; ///< Condition expression
    Stmt* stmt; ///< Statement to execute if condition is true
    /**
     * @brief Constructor for IfThen
     * @param exp Condition expression
     * @param st Statement to execute if condition is true
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    IfThen(Exp* , Stmt*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};
/**
 * @class IfThenElse
 * @brief Represents an if-then-else statement
 * 
 * Contains a condition expression, a statement to execute if the
 * condition is true, and a statement to execute if the condition is false.
 */
class IfThenElse : public Stmt
{
public: 
    Exp* expr; ///< Condition expression
    Stmt* trueStmt; ///< Statement to execute if condition is true
    Stmt* falseStmt; ///< Statement to execute if condition is false
    /**
     * @brief Constructor for IfThenElse
     * @param exp Condition expression
     * @param frstSt Statement to execute if condition is true
     * @param scndSt Statement to execute if condition is false
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    IfThenElse(Exp* , Stmt*, Stmt*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class While
 * @brief Represents a while loop statement
 * 
 * Contains a condition expression and a statement to execute
 * repeatedly as long as the condition is true.
 */
class While : public Stmt
{
public: 
    Exp* expr; ///< Loop condition expression
    Stmt* stmt; ///< Statement to execute while condition is true
    /**
     * @brief Constructor for While
     * @param exp Loop condition expression
     * @param st Statement to execute while condition is true
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    While(Exp* , Stmt*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class Var
 * @brief Represents a variable reference
 * 
 * Contains an identifier that refers to a variable in the program.
 * Base class for more complex variable references like array elements.
 */
class Var : public Node
{
public:
    Ident* id;
    TypeEnum type;
    Var(Ident*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

/**
 * @class ArrayElement
 * @brief Represents an array element access
 * 
 * Contains an identifier and an index expression that
 * refers to an element in an array.
 */
class ArrayElement : public Var
{
public:
    Exp* index;  ///< Index expression
    /**
     * @brief Constructor for ArrayElement
     * @param ident Array identifier
     * @param ind Index expression
     * @param lin Line number in source code
     * @param col Column number in source code
     */
    ArrayElement(Ident*, Exp*, int, int);
    /**
     * @brief Virtual accept method for the Visitor pattern
     * @param v The visitor object
     */
    virtual void accept(Visitor* ); 
};

//* Visitor Design Pattern

/**
 * @class Visitor
 * @brief Abstract visitor interface for the Visitor pattern
 * 
 * Defines visit methods for each node type in the AST.
 * Concrete visitors implement these methods to perform
 * operations on the AST (e.g., printing, type checking).
 */
class Visitor {
public:
    virtual void Visit(Node*) = 0;
    virtual void Visit(Stmt*) = 0;
    virtual void Visit(Prog*) = 0;
    virtual void Visit(Ident*) = 0;
    virtual void Visit(Decs*) = 0;
    virtual void Visit(ParDec*) = 0;
    virtual void Visit(IdentList*) = 0;
    virtual void Visit(SubDecs*) = 0;
    virtual void Visit(SubDec*) = 0;
    virtual void Visit(SubHead*) = 0;
    virtual void Visit(Func*) = 0;
    virtual void Visit(Args*) = 0;
    virtual void Visit(ParList*) = 0;
    virtual void Visit(Proc*) = 0;
    virtual void Visit(FuncCall*) = 0;
    virtual void Visit(CompStmt*) = 0;
    virtual void Visit(OptionalStmts*) = 0;
    virtual void Visit(StmtList*) = 0;
    virtual void Visit(Var*) = 0;
    virtual void Visit(Exp*) = 0;
    virtual void Visit(Assign*) = 0;
    virtual void Visit(ProcStmt*) = 0;
    virtual void Visit(ExpList*) = 0;
    virtual void Visit(IfThen*) = 0;
    virtual void Visit(IfThenElse*) = 0;
    virtual void Visit(While*) = 0;
    virtual void Visit(Type*) = 0;
    virtual void Visit(StdType*) = 0;
    virtual void Visit(IdExp*) = 0;
    virtual void Visit(Integer*) = 0;
    virtual void Visit(Real*) = 0;
    virtual void Visit(Bool*) = 0;
    virtual void Visit(Array*) = 0;
    virtual void Visit(ArrayElement*) = 0;
    virtual void Visit(BinOp*) = 0;
    virtual void Visit(Add*) = 0;
    virtual void Visit(Sub*) = 0;
    virtual void Visit(Mult*) = 0;
    virtual void Visit(Divide*) = 0;
    virtual void Visit(Mod*) = 0;
    virtual void Visit(GT*) = 0;
    virtual void Visit(LT*) = 0;
    virtual void Visit(GE*) = 0;
    virtual void Visit(LE*) = 0;
    virtual void Visit(ET*) = 0;
    virtual void Visit(NE*) = 0;
    virtual void Visit(And*) = 0;
    virtual void Visit(Or*) = 0;
    virtual void Visit(Not*) = 0;
};

/**
 * @class PrintVisitor
 * @brief Concrete visitor that prints the AST structure
 * 
 * Implements the Visitor interface to traverse the AST and
 * print each node, creating a textual representation of the tree.
 */
class PrintVisitor : public Visitor {
public:
    int level; ///< Current indentation level for pretty 
    PrintVisitor();
    virtual void Visit(Node*);
    virtual void Visit(Stmt*);
    virtual void Visit(Prog*);
    virtual void Visit(Ident*);
    virtual void Visit(Decs*);
    virtual void Visit(ParDec*);
    virtual void Visit(IdentList*);
    virtual void Visit(SubDecs*);
    virtual void Visit(SubDec*);
    virtual void Visit(SubHead*);
    virtual void Visit(Func*);
    virtual void Visit(Args*);
    virtual void Visit(ParList*);
    virtual void Visit(Proc*);
    virtual void Visit(FuncCall*);
    virtual void Visit(CompStmt*);
    virtual void Visit(OptionalStmts*);
    virtual void Visit(StmtList*);
    virtual void Visit(Var*);
    virtual void Visit(Exp*);
    virtual void Visit(Assign*);
    virtual void Visit(ProcStmt*);
    virtual void Visit(ExpList*);
    virtual void Visit(IfThen*);
    virtual void Visit(IfThenElse*);
    virtual void Visit(While*);
    virtual void Visit(Type*);
    virtual void Visit(StdType*);
    virtual void Visit(IdExp*);
    virtual void Visit(Integer*);
    virtual void Visit(Real*);
    virtual void Visit(Bool*);
    virtual void Visit(Array*);
    virtual void Visit(ArrayElement*);
    virtual void Visit(BinOp*);
    virtual void Visit(Add*);
    virtual void Visit(Sub*);
    virtual void Visit(Mult*);
    virtual void Visit(Divide*);
    virtual void Visit(Mod*);
    virtual void Visit(GT*);
    virtual void Visit(LT*);
    virtual void Visit(GE*);
    virtual void Visit(LE*);
    virtual void Visit(ET*);
    virtual void Visit(NE*);
    virtual void Visit(And*);
    virtual void Visit(Or*);
    virtual void Visit(Not*);
};
class TypeVisitor : public Visitor {
public:
    TypeVisitor();
    virtual void Visit(Node*);
    virtual void Visit(Stmt*);
    virtual void Visit(Prog*);
    virtual void Visit(Ident*);
    virtual void Visit(Decs*);
    virtual void Visit(ParDec*);
    virtual void Visit(IdentList*);
    virtual void Visit(SubDecs*);
    virtual void Visit(SubDec*);
    virtual void Visit(SubHead*);
    virtual void Visit(Func*);
    virtual void Visit(Args*);
    virtual void Visit(ParList*);
    virtual void Visit(Proc*);
    virtual void Visit(FuncCall*);
    virtual void Visit(CompStmt*);
    virtual void Visit(OptionalStmts*);
    virtual void Visit(StmtList*);
    virtual void Visit(Var*);
    virtual void Visit(Exp*);
    virtual void Visit(Assign*);
    virtual void Visit(ProcStmt*);
    virtual void Visit(ExpList*);
    virtual void Visit(IfThen*);
    virtual void Visit(IfThenElse*);
    virtual void Visit(While*);
    virtual void Visit(Type*);
    virtual void Visit(StdType*);
    virtual void Visit(IdExp*);
    virtual void Visit(Integer*);
    virtual void Visit(Real*);
    virtual void Visit(Bool*);
    virtual void Visit(Array*);
    virtual void Visit(ArrayElement*);
    virtual void Visit(BinOp*);
    virtual void Visit(Add*);
    virtual void Visit(Sub*);
    virtual void Visit(Mult*);
    virtual void Visit(Divide*);
    virtual void Visit(Mod*);
    virtual void Visit(GT*);
    virtual void Visit(LT*);
    virtual void Visit(GE*);
    virtual void Visit(LE*);
    virtual void Visit(ET*);
    virtual void Visit(NE*);
    virtual void Visit(And*);
    virtual void Visit(Or*);
    virtual void Visit(Not*);
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//* Symbol Table

/**
 * @enum SymbolKind
 * @brief Represents The kind of symbols (local variable, global variable, function, pocedure, parameter)
 */
enum SymbolKind {
    PARAM_VAR = 1, ///< parameter variable
    GLOBAL_VAR = 2, ///< global variable
    LOCAL_VAR = 3, ///< local variables for functions
    FUNC = 4, ///< function symbol
    PROC = 5 ///< procedure symbol
};

class FunctionSignature{
public:
    string name;
    vector<Type*>* paramTypes;
    TypeEnum returnType;

    FunctionSignature(string n, vector<Type*>* params, TypeEnum ret = VOID);
    string getSignatureString();
    bool matches(vector<Type*>* callParams);
};
/**
 * @class Symbol
 * @brief Represents an entry in the symbol table
 * 
 * Contains information about a declared identifier such as
 * its name, kind (variable, function, etc.), type, and memory location.
 * This class is used during semantic analysis and code generation phases
 * to track all declared identifiers and their properties.
 */
class Symbol
{
public:

    string Name; ///< Symbol name (identifier)
    SymbolKind Kind; ///< Symbol kind (variable, function, etc.)
    TypeEnum DataType;  ///< Data type (int, real, bool.)
    int Location;  ///< Memory location for code generation
    FunctionSignature* funcSig;
    Symbol(string name, SymbolKind kind, TypeEnum type);
    Symbol(string name, SymbolKind kind, FunctionSignature* sig);
};

/**
 * @typedef HashTable
 * @brief Type alias for the hash table used in each scope
 * 
 * Uses the CHashTable template class to store Symbol objects,
 * providing efficient symbol lookup within a scope.
 */
typedef CHashTable<Symbol> HashTable;

/**
 * @class Scope
 * @brief Represents a single scope in the program
 * 
 * Contains a hash table of symbols defined in this scope.
 */
class Scope
{
public:
    Scope* Parent;
    HashTable *hashTab; ///< Hash table for this scope
    vector<Scope*> *Children;
    Scope();
    void AddChildScope(Scope*  s);
};

/**
 * @class SymbolTable
 * @brief Manages the symbol tables for different scopes
 * 
 * The SymbolTable class is responsible for managing all scopes in the program,
 * including the global scope and nested scopes. It provides functionality for:
 * - Creating new scopes
 * - Adding symbols to scopes
 * - Looking up symbols across scope boundaries
 * - Managing scope hierarchy
 * 
 * This class is crucial for semantic analysis and ensuring proper
 * variable scoping rules are followed.
 */
class SymbolTable{
public:
    Scope* rootScope; ///< root program scope
    Scope* currentScope; ///< current scope 
    vector<Scope* > *Scopes;  ///< List of inner scopes
    SymbolTable();
    bool AddSymbol(Ident* ident, SymbolKind kind, Type* type);
    bool AddSymbol(Ident* ident, SymbolKind kind, FunctionSignature* sig);
    Symbol* LookUpSymbol(Ident* ident);
    Symbol* LookUpSymbol(Ident* ident, SymbolKind kind, vector<TypeEnum>* paramTypes);
    void NewScope();
    void CloseScope();
};

class Error : public Node
{
public:
	string Message;
	Error(string message, int lin, int col);
};

class Errors
{
public:
	vector<Error *> *errorStack;
	Errors();
	void AddError(string message, int lin, int col);
	void Print();
};

#endif
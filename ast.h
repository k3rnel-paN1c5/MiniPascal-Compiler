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

/**
 * @enum TypeEnum
 * @brief Represents the basic types supported in the language
 */
enum TypeEnum 
{
    INTTYPE, ///< Integer type
    REALTYPE, ///< Real (floating point) type
    BOOLTYPE  ///< Boolean type
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
    virtual void  accept(Visitor* v); 
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
    virtual void  accept(Visitor* );
 };

/**
 * @class Ident
 * @brief Represents an identifier (variable, function, procedure name)
 */
class Ident : public Node
{
public:
    string name; ///< The identifier name
    Ident(string, int, int);
    virtual void  accept(Visitor* ); 
};

class Decs : public Node 
{
public:
    vector<ParDec *> * decs;
    Decs(int, int);
    void AddDec(ParDec*); 
    virtual void  accept(Visitor* ); 
};
class ParDec : public Node
{
public:
    IdentList* identList;
    Type* tp;
    ParDec(IdentList*, Type *, int, int);
    
    virtual void  accept(Visitor* ); 
};
class IdentList : public Node
{
public:
    vector<Ident *> *identLst;
    IdentList(Ident*, int, int);
    void AddIdent(Ident* );
    virtual void  accept(Visitor* ); 
};
class Type : public Node
{
public:
    Type(int, int);
    virtual void  accept(Visitor* ); 
};
class StdType : public Type
{
public:
    TypeEnum type;
    StdType(TypeEnum, int, int);
    virtual void  accept(Visitor* ); 
};
class Array : public Type
{
public:
    int beginIndex;
    int endIndex;
    StdType* stdType;
    Array(int, int, StdType* , int, int);
    virtual void  accept(Visitor* ); 
};

class SubDecs : public Node 
{
public:
    vector<SubDec*>* subdecs;
    SubDecs(int, int);
    void AddDec(SubDec*);
    virtual void  accept(Visitor* ); 
};
class SubDec : public Node 
{
public: 
    SubHead* subHead;
    CompStmt* compStmt;
    SubDec(SubHead*, CompStmt*, int, int);
    virtual void  accept(Visitor* ); 
};
class SubHead : public Node
{
public:
    SubHead(int, int);
    virtual void  accept(Visitor* ); 
};
class Func : public SubHead 
{
public:
    Ident* id;
    Args* args;
    StdType* typ;
    Func(Ident*, Args*, StdType*, int, int);
    virtual void  accept(Visitor* ); 
};
class Proc : public SubHead 
{
public:
    Ident* id;
    Args* args;
    Proc(Ident*, Args*, int, int);
    virtual void  accept(Visitor* ); 
};
class Args : public Node 
{
public:
    ParList* parList;
    Args(ParList*, int, int);
    virtual void  accept(Visitor* ); 
};
class ParList : public Node
{
public:
    vector<ParDec*> * parList;
    ParList(ParDec*, int, int);
    void AddDec(ParDec*);
    virtual void  accept(Visitor* ); 
};
class OptionalStmts : public Node
{
    public:
    StmtList* stmtList;
    OptionalStmts(StmtList*,int, int);
    virtual void  accept(Visitor* ); 
};
class StmtList : public Node 
{
    public:
    vector<Stmt*> * stmts;
    StmtList(Stmt*, int, int);
    void AddStmt(Stmt*);
    virtual void  accept(Visitor* ); 
};
class Stmt : public Node 
{
    public:     
    Stmt(int, int);
    virtual void  accept(Visitor* ); 
};
class CompStmt : public Stmt 
{
public:
    OptionalStmts* optitonalStmts;
    CompStmt(OptionalStmts*, int, int);
    virtual void  accept(Visitor* ); 
};
class Assign : public Stmt
{
public:
    Var* var;
    Exp* exp;
    Assign(Var*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class ProcStmt : public Stmt
{
public:
    Ident* id;
    ExpList* expls;
    ProcStmt(Ident*, ExpList*, int, int);
    virtual void  accept(Visitor* ); 
};
class ExpList : public Node
{
public:
    vector<Exp*> * expList;
    ExpList(Exp*, int, int);
    void AddExp(Exp*);
    virtual void  accept(Visitor* ); 
};
class Exp : public Node
{
public:
    Exp(int, int);
    virtual void  accept(Visitor* ); 
};
class IdExp : public Exp
{
public:
    Ident* id;
    IdExp(Ident*, int, int);
    virtual void accept(Visitor* ); 
};
class Integer : public Exp
{
public:
    int val;
    Integer(int, int, int);
    virtual void  accept(Visitor* ); 
};
class Real : public Exp
{
public:
    float val;
    Real(float, int, int);
    virtual void  accept(Visitor* ); 
};
class Bool : public Exp
{
public:
    bool val;
    Bool(bool, int, int);
    virtual void  accept(Visitor* ); 
};
class FuncCall : public Exp
{
public:
    Ident* id;
    ExpList* exps;
    FuncCall(Ident*, ExpList*, int, int);
    virtual void  accept(Visitor* ); 
};
class Not: public Exp
{
public:
    Exp* exp;
    Not(Exp* , int, int);
    virtual void  accept(Visitor* ); 
};
class BinOp : public Exp 
{
public:
    Exp* leftExp;
    Exp* rightExp;
    BinOp(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class Add : public BinOp 
{
public:
    Add(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class Sub : public BinOp 
{
public:
    Sub(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class Mult : public BinOp 
{
public:
    Mult(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class Divide : public BinOp 
{
public:
    Divide(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class Mod : public BinOp 
{
public:
    Mod(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class GT : public BinOp 
{
public:
    GT(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class LT : public BinOp 
{
public:
    LT(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class GE : public BinOp 
{
public:
    GE(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class LE : public BinOp 
{
public:
    LE(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class ET : public BinOp 
{
public:
    ET(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class NE : public BinOp 
{
public:
    NE(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class And : public BinOp 
{
public:
    And(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class Or : public BinOp 
{
public:
    Or(Exp*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
};
class IfThen : public Stmt
{
public: 
    Exp* expr;
    Stmt* stmt;
    IfThen(Exp* , Stmt*, int, int);
    virtual void  accept(Visitor* ); 
};
class IfThenElse : public Stmt
{
public: 
    Exp* expr;
    Stmt* trueStmt;
    Stmt* falseStmt;
    IfThenElse(Exp* , Stmt*, Stmt*, int, int);
    virtual void  accept(Visitor* ); 
};
class While : public Stmt
{
public: 
    Exp* expr;
    Stmt* stmt;
    While(Exp* , Stmt*, int, int);
    virtual void  accept(Visitor* ); 
};

class Var : public Node
{
public:
    Ident* id;
    Var(Ident*, int, int);
    virtual void  accept(Visitor* ); 
};
class ArrayElement : public Var
{
public:
    Exp* index;
    ArrayElement(Ident*, Exp*, int, int);
    virtual void  accept(Visitor* ); 
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

//* Symbol Table

/**
 * @class Symbol
 * @brief Represents an entry in the symbol table
 * 
 * Contains information about a declared identifier such as
 * its name, kind (variable, function, etc.), type, and memory location.
 */
class Symbol
{
public:

    string Name; ///< Symbol name (identifier)
    int Kind; ///< Symbol kind (variable, function, class, etc.)
    int Type;  ///< Data type (int, float, etc.)
    int Location;  ///< Memory location for code generation
    Symbol(string name, int kind, int type, int location);
};

/**
 * @typedef HashTable
 * @brief Type alias for the hash table used in each scope
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
    HashTable *hashTab = new HashTable(); ///< Hash table for this scope
};

/**
 * @class SymbolTable
 * @brief Manages the symbol tables for different scopes
 * 
 * Maintains the global (root) scope and a list of inner scopes
 * for handling nested blocks and function/procedure scopes.
 */
class SymbolTable{
    Scope* rootScope; ///< Global scope of the program
    vector<Scope* > *Scopes;  ///< List of inner scopes
    /**
     * @brief Adds a symbol to the appropriate scope
     * @return The added symbol
     */
    Symbol AddSymbole();
};

#endif
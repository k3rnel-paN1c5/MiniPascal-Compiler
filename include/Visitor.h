/**
 * @file Visitor.h
 * @brief Visitor pattern implementation for AST traversal and operations
 *
 * This header defines the Visitor pattern interface and concrete implementations
 * for traversing and operating on the Abstract Syntax Tree. It provides a clean
 * separation between the AST structure and the operations performed on it,
 * allowing for extensible functionality without modifying the AST classes.
 *
 * Key components include:
 * - Visitor: Abstract base class defining the visitor interface
 * - PrintVisitor: Concrete visitor for printing AST structure
 * - TypeVisitor: Concrete visitor for type checking and analysis
 * - CodeGenVisitor: Concrete visitor for generating VM assembly code
 */
#ifndef VISITOR_H
#define VISITOR_H

#include <fstream>
using namespace std;

// Forward declarations for all AST nodes
// These must come BEFORE the Visitor class definition
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
class FuncCall;
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
class ArrayElement;
class BinOp;
class Add;
class Sub;
class Mult;
class Divide;
class IntDiv;
class GT;
class LT;
class GE;
class LE;
class ET;
class NE;
class And;
class Or;
class Not;
class UnaryMinus;

/**
 * @class Visitor
 * @brief Abstract visitor interface for the Visitor pattern
 *
 * Defines visit methods for each node type in the AST.
 * Concrete visitors implement these methods to perform
 * operations on the AST (e.g., printing, type checking).
 */
class Visitor
{
public:
    virtual void Visit(Node *) = 0;
    virtual void Visit(Stmt *) = 0;
    virtual void Visit(Prog *) = 0;
    virtual void Visit(Ident *) = 0;
    virtual void Visit(Decs *) = 0;
    virtual void Visit(ParDec *) = 0;
    virtual void Visit(IdentList *) = 0;
    virtual void Visit(SubDecs *) = 0;
    virtual void Visit(SubDec *) = 0;
    virtual void Visit(SubHead *) = 0;
    virtual void Visit(LocalDecs *) = 0;
    virtual void Visit(LocalDec *) = 0;
    virtual void Visit(Func *) = 0;
    virtual void Visit(Args *) = 0;
    virtual void Visit(ParList *) = 0;
    virtual void Visit(Proc *) = 0;
    virtual void Visit(FuncCall *) = 0;
    virtual void Visit(CompStmt *) = 0;
    virtual void Visit(OptionalStmts *) = 0;
    virtual void Visit(StmtList *) = 0;
    virtual void Visit(Var *) = 0;
    virtual void Visit(Exp *) = 0;
    virtual void Visit(Assign *) = 0;
    virtual void Visit(ProcStmt *) = 0;
    virtual void Visit(ExpList *) = 0;
    virtual void Visit(IfThen *) = 0;
    virtual void Visit(IfThenElse *) = 0;
    virtual void Visit(While *) = 0;
    virtual void Visit(Type *) = 0;
    virtual void Visit(StdType *) = 0;
    virtual void Visit(IdExp *) = 0;
    virtual void Visit(Integer *) = 0;
    virtual void Visit(Real *) = 0;
    virtual void Visit(Bool *) = 0;
    virtual void Visit(Array *) = 0;
    virtual void Visit(ArrayElement *) = 0;
    virtual void Visit(UnaryMinus *) = 0;
    virtual void Visit(BinOp *) = 0;
    virtual void Visit(Add *) = 0;
    virtual void Visit(Sub *) = 0;
    virtual void Visit(Mult *) = 0;
    virtual void Visit(Divide *) = 0;
    virtual void Visit(IntDiv *) = 0;
    virtual void Visit(GT *) = 0;
    virtual void Visit(LT *) = 0;
    virtual void Visit(GE *) = 0;
    virtual void Visit(LE *) = 0;
    virtual void Visit(ET *) = 0;
    virtual void Visit(NE *) = 0;
    virtual void Visit(And *) = 0;
    virtual void Visit(Or *) = 0;
    virtual void Visit(Not *) = 0;
};

/**
 * @class PrintVisitor
 * @brief Concrete visitor that prints the AST structure
 *
 * Implements the Visitor interface to traverse the AST and
 * print each node, creating a textual representation of the tree.
 */
class PrintVisitor : public Visitor
{
public:
    int level; ///< Current indentation level for pretty
    /**
     * @brief Constructor for PrintVisitor
     *
     * Initializes the print visitor for pretty printing the ast.
     */
    PrintVisitor();
    /**
     * @brief Visit method for base Node objects
     * @param node The Node to visit
     */
    virtual void Visit(Node *);
    /**
     * @brief Visit method for Statement nodes
     * @param stmt The Statement to visit
     */
    virtual void Visit(Stmt *);
    /**
     * @brief Visit method for Program nodes
     * @param prog The Program to visit
     */
    virtual void Visit(Prog *);
    /**
     * @brief Visit method for Identifier nodes
     * @param ident The Identifier to visit
     */
    virtual void Visit(Ident *);
    /**
     * @brief Visit method for Declarations nodes
     * @param decs The Declarations to visit
     */
    virtual void Visit(Decs *);
    /**
     * @brief Visit method for Parameter Declaration nodes
     * @param pardec The Parameter Declaration to visit
     */
    virtual void Visit(ParDec *);
    /**
     * @brief Visit method for Identifier List nodes
     * @param identlist The Identifier List to visit
     */
    virtual void Visit(IdentList *);
    /**
     * @brief Visit method for Subprogram Declarations nodes
     * @param subdecs The Subprogram Declarations to visit
     */
    virtual void Visit(SubDecs *);
    /**
     * @brief Visit method for Subprogram Declaration nodes
     * @param subdec The Subprogram Declaration to visit
     */
    virtual void Visit(SubDec *);
    /**
     * @brief Visit method for Subprogram Header nodes
     * @param subhead The Subprogram Header to visit
     */
    virtual void Visit(SubHead *);
    /**
     * @brief Visit method for Decalrations of  local variables
     * @param localdecs The local varaible declaration to visit
     */
    virtual void Visit(LocalDec *);
    /**
     * @brief Visit method for declaration of local variable of the same type
     * @param localdec The local declaration element to visit
     */
    virtual void Visit(LocalDecs *);
    /**
     * @brief Visit method for Function nodes
     * @param func The Function to visit
     */
    virtual void Visit(Func *);
    /**
     * @brief Visit method for Arguments nodes
     * @param args The Arguments to visit
     */
    virtual void Visit(Args *);
    /**
     * @brief Visit method for Parameter List nodes
     * @param parlist The Parameter List to visit
     */
    virtual void Visit(ParList *);
    /**
     * @brief Visit method for Procedure nodes
     * @param proc The Procedure to visit
     */
    virtual void Visit(Proc *);
    /**
     * @brief Visit method for Function Call nodes
     * @param funccall The Function Call to visit
     */
    virtual void Visit(FuncCall *);
    /**
     * @brief Visit method for Compound Statement nodes
     * @param compstmt The Compound Statement to visit
     */
    virtual void Visit(CompStmt *);
    /**
     * @brief Visit method for Optional Statements nodes
     * @param optstmts The Optional Statements to visit
     */
    virtual void Visit(OptionalStmts *);
    /**
     * @brief Visit method for Statement List nodes
     * @param stmtlist The Statement List to visit
     */
    virtual void Visit(StmtList *);
    /**
     * @brief Visit method for Variable nodes
     * @param var The Variable to visit
     */
    virtual void Visit(Var *);
    /**
     * @brief Visit method for Expression nodes
     * @param exp The Expression to visit
     */
    virtual void Visit(Exp *);
    /**
     * @brief Visit method for Assignment nodes
     * @param assign The Assignment to visit
     */
    virtual void Visit(Assign *);
    /**
     * @brief Visit method for Procedure Statement nodes
     * @param procstmt The Procedure Statement to visit
     */
    virtual void Visit(ProcStmt *);
    /**
     * @brief Visit method for Expression List nodes
     * @param explist The Expression List to visit
     */
    virtual void Visit(ExpList *);
    /**
     * @brief Visit method for If-Then nodes
     * @param ifthen The If-Then to visit
     */
    virtual void Visit(IfThen *);
    /**
     * @brief Visit method for If-Then-Else nodes
     * @param ifthenelse The If-Then-Else to visit
     */
    virtual void Visit(IfThenElse *);
    /**
     * @brief Visit method for While nodes
     * @param while_node The While to visit
     */
    virtual void Visit(While *);
    /**
     * @brief Visit method for Type nodes
     * @param type The Type to visit
     */
    virtual void Visit(Type *);
    /**
     * @brief Visit method for Standard Type nodes
     * @param stdtype The Standard Type to visit
     */
    virtual void Visit(StdType *);
    /**
     * @brief Visit method for Identifier Expression nodes
     * @param idexp The Identifier Expression to visit
     */
    virtual void Visit(IdExp *);
    /**
     * @brief Visit method for Integer nodes
     * @param integer The Integer to visit
     */
    virtual void Visit(Integer *);
    /**
     * @brief Visit method for Real nodes
     * @param real The Real to visit
     */
    virtual void Visit(Real *);
    /**
     * @brief Visit method for Boolean nodes
     * @param bool_node The Boolean to visit
     */
    virtual void Visit(Bool *);
    /**
     * @brief Visit method for Array nodes
     * @param array The Array to visit
     */
    virtual void Visit(Array *);
    /**
     * @brief Visit method for Array Element nodes
     * @param arrayelem The Array Element to visit
     */
    virtual void Visit(ArrayElement *);
    /**
     * @brief Visit method for negating Operation nodes
     * @param e The negating Operation to visit
     */
    virtual void Visit(UnaryMinus *);
    /**
     * @brief Visit method for Binary Operation nodes
     * @param binop The Binary Operation to visit
     */
    virtual void Visit(BinOp *);
    /**
     * @brief Visit method for Addition nodes
     * @param add The Addition to visit
     */
    virtual void Visit(Add *);
    /**
     * @brief Visit method for Subtraction nodes
     * @param sub The Subtraction to visit
     */
    virtual void Visit(Sub *);
    /**
     * @brief Visit method for Multiplication nodes
     * @param mult The Multiplication to visit
     */
    virtual void Visit(Mult *);
    /**
     * @brief Visit method for Division nodes
     * @param divide The Division to visit
     */
    virtual void Visit(Divide *);
    /**
     * @brief Visit method for Integer Divison nodes
     * @param intDiv The integer divison to visit
     */
    virtual void Visit(IntDiv *);
    /**
     * @brief Visit method for Greater Than nodes
     * @param gt The Greater Than to visit
     */
    virtual void Visit(GT *);
    /**
     * @brief Visit method for Less Than nodes
     * @param lt The Less Than to visit
     */
    virtual void Visit(LT *);
    /**
     * @brief Visit method for Greater Equal nodes
     * @param ge The Greater Equal to visit
     */
    virtual void Visit(GE *);
    /**
     * @brief Visit method for Less Equal nodes
     * @param le The Less Equal to visit
     */
    virtual void Visit(LE *);
    /**
     * @brief Visit method for Equal nodes
     * @param et The Equal to visit
     */
    virtual void Visit(ET *);
    /**
     * @brief Visit method for Not Equal nodes
     * @param ne The Not Equal to visit
     */
    virtual void Visit(NE *);
    /**
     * @brief Visit method for Logical And nodes
     * @param and_node The Logical And to visit
     */
    virtual void Visit(And *);
    /**
     * @brief Visit method for Logical Or nodes
     * @param or_node The Logical Or to visit
     */
    virtual void Visit(Or *);
    /**
     * @brief Visit method for Logical Not nodes
     * @param not_node The Logical Not to visit
     */
    virtual void Visit(Not *);
};

/**
 * @class TypeVisitor
 * @brief Concrete visitor that performs type checking and analysis
 *
 * Implements the Visitor interface to traverse the AST and perform
 * semantic analysis, including type checking, type inference, and
 * validation of type compatibility in expressions and statements.
 */
class TypeVisitor : public Visitor
{
public:
    Func *currentFunction;         ///< To keep track of the current function context for return type checking
    bool currentFunctionHasReturn; ///< Flag to check if current function has a return statement
    /**
     * @brief Constructor for TypeVisitor
     *
     * Initializes the type visitor for semantic analysis.
     */
    TypeVisitor();
    /**
     * @brief Visit method for base Node objects
     * @param node The Node to visit
     */
    virtual void Visit(Node *);
    /**
     * @brief Visit method for Statement nodes
     * @param stmt The Statement to visit
     */
    virtual void Visit(Stmt *);
    /**
     * @brief Visit method for Program nodes
     * @param prog The Program to visit
     */
    virtual void Visit(Prog *);
    /**
     * @brief Visit method for Identifier nodes
     * @param ident The Identifier to visit
     */
    virtual void Visit(Ident *);
    /**
     * @brief Visit method for Declarations nodes
     * @param decs The Declarations to visit
     */
    virtual void Visit(Decs *);
    /**
     * @brief Visit method for Parameter Declaration nodes
     * @param pardec The Parameter Declaration to visit
     */
    virtual void Visit(ParDec *);
    /**
     * @brief Visit method for Identifier List nodes
     * @param identlist The Identifier List to visit
     */
    virtual void Visit(IdentList *);
    /**
     * @brief Visit method for Subprogram Declarations nodes
     * @param subdecs The Subprogram Declarations to visit
     */
    virtual void Visit(SubDecs *);
    /**
     * @brief Visit method for Subprogram Declaration nodes
     * @param subdec The Subprogram Declaration to visit
     */
    virtual void Visit(SubDec *);
    /**
     * @brief Visit method for Subprogram Header nodes
     * @param subhead The Subprogram Header to visit
     */
    virtual void Visit(SubHead *);
    /**
     * @brief Visit method for Decalrations of  local variables
     * @param localdecs The local varaible declaration to visit
     */
    virtual void Visit(LocalDec *);
    /**
     * @brief Visit method for declaration of local variable of the same type
     * @param localdec The local declaration element to visit
     */
    virtual void Visit(LocalDecs *);
    /**
     * @brief Visit method for Function nodes
     * @param func The Function to visit
     */
    virtual void Visit(Func *);
    /**
     * @brief Visit method for Arguments nodes
     * @param args The Arguments to visit
     */
    virtual void Visit(Args *);
    /**
     * @brief Visit method for Parameter List nodes
     * @param parlist The Parameter List to visit
     */
    virtual void Visit(ParList *);
    /**
     * @brief Visit method for Procedure nodes
     * @param proc The Procedure to visit
     */
    virtual void Visit(Proc *);
    /**
     * @brief Visit method for Function Call nodes
     * @param funccall The Function Call to visit
     */
    virtual void Visit(FuncCall *);
    /**
     * @brief Visit method for Compound Statement nodes
     * @param compstmt The Compound Statement to visit
     */
    virtual void Visit(CompStmt *);
    /**
     * @brief Visit method for Optional Statements nodes
     * @param optstmts The Optional Statements to visit
     */
    virtual void Visit(OptionalStmts *);
    /**
     * @brief Visit method for Statement List nodes
     * @param stmtlist The Statement List to visit
     */
    virtual void Visit(StmtList *);
    /**
     * @brief Visit method for Variable nodes
     * @param var The Variable to visit
     */
    virtual void Visit(Var *);
    /**
     * @brief Visit method for Expression nodes
     * @param exp The Expression to visit
     */
    virtual void Visit(Exp *);
    /**
     * @brief Visit method for Assignment nodes
     * @param assign The Assignment to visit
     */
    virtual void Visit(Assign *);
    /**
     * @brief Visit method for Procedure Statement nodes
     * @param procstmt The Procedure Statement to visit
     */
    virtual void Visit(ProcStmt *);
    /**
     * @brief Visit method for Expression List nodes
     * @param explist The Expression List to visit
     */
    virtual void Visit(ExpList *);
    /**
     * @brief Visit method for If-Then nodes
     * @param ifthen The If-Then to visit
     */
    virtual void Visit(IfThen *);
    /**
     * @brief Visit method for If-Then-Else nodes
     * @param ifthenelse The If-Then-Else to visit
     */
    virtual void Visit(IfThenElse *);
    /**
     * @brief Visit method for While nodes
     * @param while_node The While to visit
     */
    virtual void Visit(While *);
    /**
     * @brief Visit method for Type nodes
     * @param type The Type to visit
     */
    virtual void Visit(Type *);
    /**
     * @brief Visit method for Standard Type nodes
     * @param stdtype The Standard Type to visit
     */
    virtual void Visit(StdType *);
    /**
     * @brief Visit method for Identifier Expression nodes
     * @param idexp The Identifier Expression to visit
     */
    virtual void Visit(IdExp *);
    /**
     * @brief Visit method for Integer nodes
     * @param integer The Integer to visit
     */
    virtual void Visit(Integer *);
    /**
     * @brief Visit method for Real nodes
     * @param real The Real to visit
     */
    virtual void Visit(Real *);
    /**
     * @brief Visit method for Boolean nodes
     * @param bool_node The Boolean to visit
     */
    virtual void Visit(Bool *);
    /**
     * @brief Visit method for Array nodes
     * @param array The Array to visit
     */
    virtual void Visit(Array *);
    /**
     * @brief Visit method for Array Element nodes
     * @param arrayelem The Array Element to visit
     */
    virtual void Visit(ArrayElement *);
    /**
     * @brief Visit method for negating Operation nodes
     * @param e The negating Operation to visit
     */
    virtual void Visit(UnaryMinus *);
    /**
     * @brief Visit method for Binary Operation nodes
     * @param binop The Binary Operation to visit
     */
    virtual void Visit(BinOp *);
    /**
     * @brief Visit method for Addition nodes
     * @param add The Addition to visit
     */
    virtual void Visit(Add *);
    /**
     * @brief Visit method for Subtraction nodes
     * @param sub The Subtraction to visit
     */
    virtual void Visit(Sub *);
    /**
     * @brief Visit method for Multiplication nodes
     * @param mult The Multiplication to visit
     */
    virtual void Visit(Mult *);
    /**
     * @brief Visit method for Division nodes
     * @param divide The Division to visit
     */
    virtual void Visit(Divide *);
    /**
     * @brief Visit method for Integer Disvion nodes
     * @param intDiv The integerr divion to visit
     */
    virtual void Visit(IntDiv *);
    /**
     * @brief Visit method for Greater Than nodes
     * @param gt The Greater Than to visit
     */
    virtual void Visit(GT *);
    /**
     * @brief Visit method for Less Than nodes
     * @param lt The Less Than to visit
     */
    virtual void Visit(LT *);
    /**
     * @brief Visit method for Greater Equal nodes
     * @param ge The Greater Equal to visit
     */
    virtual void Visit(GE *);
    /**
     * @brief Visit method for Less Equal nodes
     * @param le The Less Equal to visit
     */
    virtual void Visit(LE *);
    /**
     * @brief Visit method for Equal nodes
     * @param et The Equal to visit
     */
    virtual void Visit(ET *);
    /**
     * @brief Visit method for Not Equal nodes
     * @param ne The Not Equal to visit
     */
    virtual void Visit(NE *);
    /**
     * @brief Visit method for Logical And nodes
     * @param and_node The Logical And to visit
     */
    virtual void Visit(And *);
    /**
     * @brief Visit method for Logical Or nodes
     * @param or_node The Logical Or to visit
     */
    virtual void Visit(Or *);
    /**
     * @brief Visit method for Logical Not nodes
     * @param not_node The Logical Not to visit
     */
    virtual void Visit(Not *);
};

/**
 * @class CodeGenVisitor
 * @brief Code generation visitor for converting AST to VM assembly.
 *
 * This visitor traverses the AST after semantic analysis and generates
 * assembly code for the target virtual machine.
 */
class CodeGenVisitor : public Visitor
{
private:
    ofstream outFile; ///< the file to write the code tp
    int labelCount; ///<  count of labels to faciltate naming the labels
    Func *currentFunctionContext; ///< To access function properties

    // Helper to generate unique labels for jumps
    string newLabel();

    // Helper to emit an instruction to the output file
    void emit(const string &instruction);

    // Helper to emit a label
    void emitLabel(const string &label);

public:
    CodeGenVisitor(const string &filename);
    ~CodeGenVisitor();

    /**
     * @brief Visit method for base Node objects
     * @param node The Node to visit
     */
    virtual void Visit(Node *);
    /**
     * @brief Visit method for Statement nodes
     * @param stmt The Statement to visit
     */
    virtual void Visit(Stmt *);
    /**
     * @brief Visit method for Program nodes
     * @param prog The Program to visit
     */
    virtual void Visit(Prog *);
    /**
     * @brief Visit method for Identifier nodes
     * @param ident The Identifier to visit
     */
    virtual void Visit(Ident *);
    /**
     * @brief Visit method for Declarations nodes
     * @param decs The Declarations to visit
     */
    virtual void Visit(Decs *);
    /**
     * @brief Visit method for Parameter Declaration nodes
     * @param pardec The Parameter Declaration to visit
     */
    virtual void Visit(ParDec *);
    /**
     * @brief Visit method for Identifier List nodes
     * @param identlist The Identifier List to visit
     */
    virtual void Visit(IdentList *);
    /**
     * @brief Visit method for Subprogram Declarations nodes
     * @param subdecs The Subprogram Declarations to visit
     */
    virtual void Visit(SubDecs *);
    /**
     * @brief Visit method for Subprogram Declaration nodes
     * @param subdec The Subprogram Declaration to visit
     */
    virtual void Visit(SubDec *);
    /**
     * @brief Visit method for Subprogram Header nodes
     * @param subhead The Subprogram Header to visit
     */
    virtual void Visit(SubHead *);
    /**
     * @brief Visit method for Decalrations of  local variables
     * @param localdecs The local varaible declaration to visit
     */
    virtual void Visit(LocalDec *);
    /**
     * @brief Visit method for declaration of local variable of the same type
     * @param localdec The local declaration element to visit
     */
    virtual void Visit(LocalDecs *);
    /**
     * @brief Visit method for Function nodes
     * @param func The Function to visit
     */
    virtual void Visit(Func *);
    /**
     * @brief Visit method for Arguments nodes
     * @param args The Arguments to visit
     */
    virtual void Visit(Args *);
    /**
     * @brief Visit method for Parameter List nodes
     * @param parlist The Parameter List to visit
     */
    virtual void Visit(ParList *);
    /**
     * @brief Visit method for Procedure nodes
     * @param proc The Procedure to visit
     */
    virtual void Visit(Proc *);
    /**
     * @brief Visit method for Function Call nodes
     * @param funccall The Function Call to visit
     */
    virtual void Visit(FuncCall *);
    /**
     * @brief Visit method for Compound Statement nodes
     * @param compstmt The Compound Statement to visit
     */
    virtual void Visit(CompStmt *);
    /**
     * @brief Visit method for Optional Statements nodes
     * @param optstmts The Optional Statements to visit
     */
    virtual void Visit(OptionalStmts *);
    /**
     * @brief Visit method for Statement List nodes
     * @param stmtlist The Statement List to visit
     */
    virtual void Visit(StmtList *);
    /**
     * @brief Visit method for Variable nodes
     * @param var The Variable to visit
     */
    virtual void Visit(Var *);
    /**
     * @brief Visit method for Expression nodes
     * @param exp The Expression to visit
     */
    virtual void Visit(Exp *);
    /**
     * @brief Visit method for Assignment nodes
     * @param assign The Assignment to visit
     */
    virtual void Visit(Assign *);
    /**
     * @brief Visit method for Procedure Statement nodes
     * @param procstmt The Procedure Statement to visit
     */
    virtual void Visit(ProcStmt *);
    /**
     * @brief Visit method for Expression List nodes
     * @param explist The Expression List to visit
     */
    virtual void Visit(ExpList *);
    /**
     * @brief Visit method for If-Then nodes
     * @param ifthen The If-Then to visit
     */
    virtual void Visit(IfThen *);
    /**
     * @brief Visit method for If-Then-Else nodes
     * @param ifthenelse The If-Then-Else to visit
     */
    virtual void Visit(IfThenElse *);
    /**
     * @brief Visit method for While nodes
     * @param while_node The While to visit
     */
    virtual void Visit(While *);
    /**
     * @brief Visit method for Type nodes
     * @param type The Type to visit
     */
    virtual void Visit(Type *);
    /**
     * @brief Visit method for Standard Type nodes
     * @param stdtype The Standard Type to visit
     */
    virtual void Visit(StdType *);
    /**
     * @brief Visit method for Identifier Expression nodes
     * @param idexp The Identifier Expression to visit
     */
    virtual void Visit(IdExp *);
    /**
     * @brief Visit method for Integer nodes
     * @param integer The Integer to visit
     */
    virtual void Visit(Integer *);
    /**
     * @brief Visit method for Real nodes
     * @param real The Real to visit
     */
    virtual void Visit(Real *);
    /**
     * @brief Visit method for Boolean nodes
     * @param bool_node The Boolean to visit
     */
    virtual void Visit(Bool *);
    /**
     * @brief Visit method for Array nodes
     * @param array The Array to visit
     */
    virtual void Visit(Array *);
    /**
     * @brief Visit method for Array Element nodes
     * @param arrayelem The Array Element to visit
     */
    virtual void Visit(ArrayElement *);
    /**
     * @brief Visit method for negating Operation nodes
     * @param e The negating Operation to visit
     */
    virtual void Visit(UnaryMinus *);
    /**
     * @brief Visit method for Binary Operation nodes
     * @param binop The Binary Operation to visit
     */
    virtual void Visit(BinOp *);
    /**
     * @brief Visit method for Addition nodes
     * @param add The Addition to visit
     */
    virtual void Visit(Add *);
    /**
     * @brief Visit method for Subtraction nodes
     * @param sub The Subtraction to visit
     */
    virtual void Visit(Sub *);
    /**
     * @brief Visit method for Multiplication nodes
     * @param mult The Multiplication to visit
     */
    virtual void Visit(Mult *);
    /**
     * @brief Visit method for Division nodes
     * @param divide The Division to visit
     */
    virtual void Visit(Divide *);
    /**
     * @brief Visit method for Integer Divison nodes
     * @param intDiv The integer divison to visit
     */
    virtual void Visit(IntDiv *);
    /**
     * @brief Visit method for Greater Than nodes
     * @param gt The Greater Than to visit
     */
    virtual void Visit(GT *);
    /**
     * @brief Visit method for Less Than nodes
     * @param lt The Less Than to visit
     */
    virtual void Visit(LT *);
    /**
     * @brief Visit method for Greater Equal nodes
     * @param ge The Greater Equal to visit
     */
    virtual void Visit(GE *);
    /**
     * @brief Visit method for Less Equal nodes
     * @param le The Less Equal to visit
     */
    virtual void Visit(LE *);
    /**
     * @brief Visit method for Equal nodes
     * @param et The Equal to visit
     */
    virtual void Visit(ET *);
    /**
     * @brief Visit method for Not Equal nodes
     * @param ne The Not Equal to visit
     */
    virtual void Visit(NE *);
    /**
     * @brief Visit method for Logical And nodes
     * @param and_node The Logical And to visit
     */
    virtual void Visit(And *);
    /**
     * @brief Visit method for Logical Or nodes
     * @param or_node The Logical Or to visit
     */
    virtual void Visit(Or *);
    /**
     * @brief Visit method for Logical Not nodes
     * @param not_node The Logical Not to visit
     */
    virtual void Visit(Not *);
};

#endif
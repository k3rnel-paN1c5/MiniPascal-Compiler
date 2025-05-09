#ifndef AST_H
#define AST_H

#include <vector>
using namespace std;

class Node;
class Prog;
class Ident;
class Decs;
class ParDec;
class IdentList;
class SubDecs;
class SubDec;
class Func;
class Args;
class ParList;
class Proc;
class CompStmt;
class OptionalStmts;
class StmtList;
class Stmt;
class Var;
class Exp;
class Assigne;
class ProcStmt;
class IfThen;
class IfThenElse;
class While;
class Type;
class StdType;
class Integer;
class Real;
class Boolean;
class Array;
class IntNum;
class RealNum;
class BoolVal;
class BinOp;
class Add;
class Minus;
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


class Node 
{
public:
    int line;
    int column;
    Node *father;
    Node(int, int);
};
class Prog : Node 
{
    public: 
    public:
    Ident *name;
    Decs *declarations;
    SubDecs *subDeclarations;
    CompStmt *compoundStatment;
    Prog(Ident *, Decs *, SubDecs *, CompStmt *, int, int);
};


#endif
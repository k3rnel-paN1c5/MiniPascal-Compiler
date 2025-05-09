#include "ast.h"
#include <iostream>

Node::Node(int lin, int col)
{
    this->line = lin;
    this->column = col;
    this->father = NULL;
}

Prog::Prog(Ident * n, Decs * decl, SubDecs * subdecl, CompStmt * comst, int lin, int col) : Node(lin, col)
{
    this->name = n;
    this->declarations = decl;
    this->subDeclarations = subdecl;
    this->compoundStatment = comst;
}

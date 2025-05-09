#include "ast.h"
#include <iostream>

Node::Node(int lin, int col)
{
    this->line = lin;
    this->column = col;
    this->father = NULL;
}

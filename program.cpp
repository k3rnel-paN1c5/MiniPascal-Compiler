#include "ast.h"
#include "parser.h"
extern int yydebug;
extern int yyparse();
extern Func* root;


int main() {
    yydebug = 1;  // Enable debug if needed
    yyparse(); 
    return 0;
}
extern int yydebug;
extern int yyparse();


int main() {
    yydebug = 0;  // Enable debug if needed
    yyparse(); 
    return 0;
}
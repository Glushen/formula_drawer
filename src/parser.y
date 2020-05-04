%code requires {
#include <string>
#include <expression.h>
#include <parser_helper.h>

extern int yylex(void);
extern void yyerror(const char *s, ...);
extern void yy_accept_ast(fd::exp::Expression expression);
}

%union {
    fd::exp::Expression* expression;
}

%token END_OF_FILE
%token <expression> PRIMITIVE

%type <expression> expression

%%

input:
    expression END_OF_FILE  { yy_accept_ast(ph::unwrap($1)); YYACCEPT; }

expression:
    PRIMITIVE

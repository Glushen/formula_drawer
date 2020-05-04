%expect 0
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
    std::vector<fd::exp::Case>* cases;
    std::vector<std::vector<std::unique_ptr<fd::exp::Expression>>>* matrix;
    std::vector<std::unique_ptr<fd::exp::Expression>>* matrixRow;
}

%token END_OF_FILE
%token <expression> PRIMITIVE

%token EQUAL_OPERATOR UNEQUAL_OPERATOR LESS_OPERATOR GREATER_OPERATOR LESS_EQUAL_OPERATOR GREATER_EQUAL_OPERATOR
%token SUM PRODUCT INTEGRAL CASES MATRIX

%type <expression> exp
%type <cases> cases
%type <matrix> matrix
%type <matrixRow> matrix-row

%left EQUAL_OPERATOR UNEQUAL_OPERATOR LESS_OPERATOR GREATER_OPERATOR LESS_EQUAL_OPERATOR GREATER_EQUAL_OPERATOR
%left '+' '-'
%left '*' '/'
%nonassoc UNARY_PLUS UNARY_MINUS
%right '^'
%nonassoc '['

%%

input:
    exp END_OF_FILE  { yy_accept_ast(ph::unwrap($1)); YYACCEPT; }

exp:
    PRIMITIVE                       { $$ = $1; }
|   '(' exp ')'                     { $$ = new fd::exp::Bracketed(ph::uniquePtr($2)); }
|   exp '[' exp ']'                 { $$ = new fd::exp::Index(ph::uniquePtr($1), ph::uniquePtr($3)); }
|   exp '^' exp                     { $$ = new fd::exp::Power(ph::uniquePtr($1), ph::uniquePtr($3)); }
|   '+' exp %prec UNARY_PLUS        { $$ = new fd::exp::Unary(u8"+", ph::uniquePtr($2)); }
|   '-' exp %prec UNARY_MINUS       { $$ = new fd::exp::Unary(u8"−", ph::uniquePtr($2)); }
|   exp '*' exp                     { $$ = new fd::exp::Binary(u8"⋅", ph::uniquePtr($1), ph::uniquePtr($3)); }
|   exp '/' exp                     { $$ = new fd::exp::Division(ph::uniquePtr($1), ph::uniquePtr($3)); }
|   exp '+' exp                     { $$ = new fd::exp::Binary(u8"+", ph::uniquePtr($1), ph::uniquePtr($3)); }
|   exp '-' exp                     { $$ = new fd::exp::Binary(u8"−", ph::uniquePtr($1), ph::uniquePtr($3)); }
|   exp EQUAL_OPERATOR exp          { $$ = new fd::exp::Binary(u8"=", ph::uniquePtr($1), ph::uniquePtr($3)); }
|   exp UNEQUAL_OPERATOR exp        { $$ = new fd::exp::Binary(u8"≠", ph::uniquePtr($1), ph::uniquePtr($3)); }
|   exp LESS_OPERATOR exp           { $$ = new fd::exp::Binary(u8"<", ph::uniquePtr($1), ph::uniquePtr($3)); }
|   exp GREATER_OPERATOR exp        { $$ = new fd::exp::Binary(u8">", ph::uniquePtr($1), ph::uniquePtr($3)); }
|   exp LESS_EQUAL_OPERATOR exp     { $$ = new fd::exp::Binary(u8"≤", ph::uniquePtr($1), ph::uniquePtr($3)); }
|   exp GREATER_EQUAL_OPERATOR exp  { $$ = new fd::exp::Binary(u8"≥", ph::uniquePtr($1), ph::uniquePtr($3)); }
|   SUM      '(' exp ',' exp ',' exp ')'  { $$ = new fd::exp::Variadic(u8"∑", ph::uniquePtr($3), ph::uniquePtr($5), ph::uniquePtr($7)); }
|   PRODUCT  '(' exp ',' exp ',' exp ')'  { $$ = new fd::exp::Variadic(u8"∏", ph::uniquePtr($3), ph::uniquePtr($5), ph::uniquePtr($7)); }
|   INTEGRAL '(' exp ',' exp ',' exp ')'  { $$ = new fd::exp::Variadic(u8"∫", ph::uniquePtr($3), ph::uniquePtr($5), ph::uniquePtr($7)); }
|   CASES '(' cases ')'                   { $$ = new fd::exp::Cases(ph::unwrap($3)); }
|   MATRIX '(' matrix ')'                 { $$ = new fd::exp::Matrix(ph::unwrap($3)); }

cases:
    exp ',' exp            { $$ = new std::vector<fd::exp::Case>(); $$->emplace_back(ph::uniquePtr($1), ph::uniquePtr($3)); }
|   cases ',' exp ',' exp  { $$ = $1; $$->emplace_back(ph::uniquePtr($3), ph::uniquePtr($5)); }

matrix:
    '(' matrix-row ')'             { $$ = new std::vector<std::vector<std::unique_ptr<fd::exp::Expression>>>(); $$->push_back(ph::unwrap($2)); }
|   matrix ',' '(' matrix-row ')'  { $$ = $1; $$->push_back(ph::unwrap($4)); }

matrix-row:
    exp                 { $$ = new std::vector<std::unique_ptr<fd::exp::Expression>>(); $$->emplace_back($1); }
|   matrix-row ',' exp  { $$ = $1; $$->emplace_back($3); }

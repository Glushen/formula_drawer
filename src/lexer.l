%option noyywrap nodefault case-insensitive

%{
#include <parser.h>
%}

%%

sum          { return SUM; }
product      { return PRODUCT; }
int(egral)?  { return INTEGRAL; }
cases        { return CASES; }
matrix       { return MATRIX; }
inf(inity)?  { yylval.expression = new fd::exp::Primitive(u8"∞"); return PRIMITIVE; }
[a-z_][a-z_0-9]*|([0-9]+\.?[0-9]*|\.[0-9]+)(e(\+|\-)?[0-9]+)?  { yylval.expression = new fd::exp::Primitive(yytext); return PRIMITIVE; }

\=\=?     { return EQUAL_OPERATOR; }
\!\=|\<\> { return UNEQUAL_OPERATOR; }
\<        { return LESS_OPERATOR; }
\>        { return GREATER_OPERATOR; }
\<\=      { return LESS_EQUAL_OPERATOR; }
\>\=      { return GREATER_EQUAL_OPERATOR; }

\^|\*|\/|\+|\-|\(|\)|\[|\]|\,  { return yytext[0]; }

[ \t\r\n]       { /* ignore */ }
<<EOF>>         { return END_OF_FILE; }
.               { yyerror("Mystery character %c", yytext[0]); }

%%

void yy_set_input_string(const char* in) {
    yy_scan_string(in);
}

void yy_clear_buffer(void) {
    yy_delete_buffer(YY_CURRENT_BUFFER);
}

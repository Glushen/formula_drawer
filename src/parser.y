%code requires {
extern int yylex(void);
extern void yyerror(const char *s, ...);
}

%union {
    int temp;
}

%token TEMP

%%

STATEMENT:
    TEMP

%option noyywrap nodefault

%{
#include <parser.h>
%}

%%

.        { return TEMP; }
\n       { return TEMP; }
<<EOF>>  { return TEMP; }

%%

%{
    #include <string.h>
    #include "sintatico.tab.h"
%}

%option noyywrap

PRINT "PRINT"
STDIN "STDIN"
IF "IF"
ELSE "ELSE"
WHILE "WHILE"
FOR "FOR"

TIPO "NUM"|"STR"
VAR [A-Za-z_][A-Za-z0-9_]*
NUMERO [0-9]+(\.[0-9]+)?

STRING \"(\\.|[^"\\])*\"
MASCARA '%[is]'

%%

{PRINT}      { return PRINT; }
{STDIN}      { return STDIN; }
{IF}         { return IF; }
{ELSE}       { return ELSE; }
{WHILE}      { return WHILE; }
{FOR}        { return FOR; }

"<="         { return ME; }
">="         { return MA; }
"=="         { return IG; }
"!="         { return DI; }

"<"          { return '<'; }
">"          { return '>'; }
"+"          { return '+'; }
"-"          { return '-'; }
"*"          { return '*'; }
"/"          { return '/'; }
"="          { return '='; }
"("          { return '('; }
")"          { return ')'; }
"{"          { return '{'; }
"}"          { return '}'; }
","          { return ','; }
";"          { return ';'; }
"["          { return '['; }
"]"          { return ']'; } 

\n           { yylineno++; }
[ \t]+       {  }


{TIPO} {
    yylval.valor_texto = strdup(yytext);
    return TIPO;
}

{VAR} {
    yylval.valor_texto = strdup(yytext); 
    return VAR;
}

{NUMERO} {
    yylval.valor_numerico = atof(yytext);
    return NUMERO;
}

{STRING} {
    yylval.valor_texto = strdup(yytext + 1);
    yylval.valor_texto[yyleng - 2] = '\0';
    return STRING;
}

{MASCARA} {
    yylval.valor_texto = strdup(yytext + 1);
    yylval.valor_texto[yyleng - 2] = '\0';
    return MASCARA;
}

"\\\\".*\n   { yylineno++; }
"\\\\".*     {  }

. { fprintf(stderr, "Caractere inválido na linha %d: %s\n", yylineno, yytext); }

%%

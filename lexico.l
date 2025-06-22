%{
    #include <string.h>
    #include "sintatico.tab.h"
    // yylineno é uma variável global do Flex que rastreia o número da linha atual
%}

%option noyywrap

FIM "FIM"
INICIO "INICIO"
PRINT "PRINT"
STDIN "STDIN"
IF "IF"
ELSE "ELSE"
WHILE "WHILE"

TIPO "NUM"|"STR"
VAR [A-Za-z_][A-Za-z0-9_]*
NUMERO [0-9]+(\.[0-9]+)?

STRING \"(\\.|[^"\\])*\"
MASCARA '%[is]'


%%

{FIM}      { return FIM; }
{INICIO}   { return INICIO; }
{PRINT}    { return PRINT; }
{STDIN}    { return STDIN; }
{IF}       { return IF; }
{ELSE}     { return ELSE; }
{WHILE}    { return WHILE; }

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

\n         { yylineno++; }
[ \t]+     {  }


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

. { fprintf(stderr, "Caractere inválido na linha %d: %s\n", yylineno, yytext); }

%%

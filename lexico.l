%{
	#include <string.h>
%}

FIM "FIM"
INICIO "INICIO"
PRINT "PRINT"
STDIN "STDIN"
TIPO "INT"|"STRING"

ID [A-Za-z]+
NUMERO [0-9]+

STRING \"(\\.|[^"\\])*\"

MASCARA %(i|s)

%%

{FIM} { return FIM; }
{INICIO} { return INICIO; }
{PRINT} { return PRINT; }
{STDIN} { return STDIN; }

{TIPO} {
    yylval.sval = strdup(yytext);
    return TIPO;
}

{ID} {
    yylval.sval = strdup(yytext);
    return ID;
}

{NUMERO} {
    yylval.ival = atoi(yytext);
    return NUMERO;
}

{STRING} {
    yylval.sval = strdup(yytext+1);
    yylval.sval[yyleng-2] = '\0';
    return STRING;
}

{MASCARA} {
    yylval.sval = strdup(yytext);
    return MASCARA;
}

"+" { return '+'; }
"-" { return '-'; }
"*" { return '*'; }
"/" { return '/'; }
"(" { return '('; }
")" { return ')'; }
"=" { return '='; }
"," { return ','; }

[\ \n\t]* { ; } 

%%
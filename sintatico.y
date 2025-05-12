%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uthash.h"

int yylex();
void yyerror (char *s) {
    printf("%s\n", s);
}

typedef struct {
	char *nome;
	char *tipo;
	union {
		int ival;
		char *sval;
	} valor;
	UT_hash_handle hh;
} Variavel;

Variavel *tabela_variaveis = NULL;

void inserir_variavel(char *tipo, char *nome, int ival, char *sval) {
	Variavel *v = malloc(sizeof(Variavel));
	v->nome = strdup(nome);
	v->tipo = strdup(tipo);
	if (strcmp(tipo, "int") == 0){
		v->valor.ival = ival;
	}else{
		v->valor.sval = strdup(sval);
	}
	HASH_ADD_KEYPTR(hh, tabela_variaveis, v->nome, strlen(v->nome), v);
}

Variavel *buscar_variavel(char *nome) {
	Variavel *v = NULL;
	HASH_FIND_STR(tabela_variaveis, nome, v);
	return v;
}

%}

%union {
    int ival;
    char *sval;
}

%token FIM
%token INICIO
%token PRINT
%token STDIN
%token <sval> TIPO
%token <ival> NUMERO
%token <sval> ID
%token <sval> STRING
%token <sval> MASCARA
%left '+' '-'
%left '*' '/'

%type <ival> valor
%type <ival> calcula

%%

val: INICIO cod FIM
    ;

cod: instrucao cod
    | instrucao
    ;

instrucao: calcula
         | escreve
		 | declara
         | atribui
         | entrada
         ;

calcula: valor '+' calcula { $$ = $1 + $3; }
       | valor '-' calcula { $$ = $1 - $3; }
       | valor '*' calcula { $$ = $1 * $3; }
       | valor '/' calcula { $$ = $1 / $3; }
       | '(' calcula ')' { $$ = $2; }
       | valor { $$ = $1; }
       ;

valor:    NUMERO { $$ = $1; }
        | ID     {
            Variavel *v = buscar_variavel($1);
            if (!v) {
                yyerror("Variável não declarada");
                YYABORT;
            }
            if (strcmp(v->tipo, "int") == 0) {
                $$ = v->valor.ival;
            } else {
                yyerror("Operação matemática inválida.");
                YYABORT;
            }
        }

escreve: PRINT '('saidas')';

saidas:  tipos_saida','saidas
        |tipos_saida { printf("\n"); }
        ;

tipos_saida:  MASCARA','calcula { 
                if(strcmp($1, "'%i'") == 0){
                    printf("%d", $3);
                }
            }
            | ID      { 
                Variavel *v = buscar_variavel($1);
                if (!v) {
                    yyerror("Variável não declarada");
                    YYABORT;
                }
                if (strcmp(v->tipo, "int") == 0) {
                    printf("%d", v->valor.ival);
                } else {
                    printf("%s", v->valor.sval);
                }
            }
            | STRING { printf("%s", $1); }
            ;
 
declara: TIPO ID '=' calcula {
            inserir_variavel($1, $2, $4, NULL);
        }
      	|TIPO ID '=' STRING {
            inserir_variavel($1, $2, 0, $4);
        }
        |TIPO ID {
            inserir_variavel($1, $2, 0, NULL);
        }
      	;

atribui: ID '=' calcula {
            Variavel *v = buscar_variavel($1);
            inserir_variavel(v->tipo, v->nome, $3, NULL);
        }
        |ID '=' STRING {
            Variavel *v = buscar_variavel($1);
            inserir_variavel(v->tipo, v->nome, 0, $3);
        }

entrada: STDIN '('MASCARA','ID')' {
            Variavel *v = buscar_variavel($5);
            if (!v) {
                yyerror("Variável não declarada");
                YYABORT;
            }

            if(strcmp($3, "'%i'") == 0){
                int temp;
                scanf("%d", &temp);
                inserir_variavel(v->tipo, v->nome, temp, NULL);
            }
        }
%%

#include "lex.yy.c"

int main() {
    FILE *codigo = fopen("entrada.rag", "r");
    if (!codigo) {
        perror("entrada.txt");
        return 1;
    }
    yyin = codigo;
    yyparse();
    fclose(codigo);
    return 0;
}

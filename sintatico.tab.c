/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "sintatico.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "uthash.h"

int yylex(void);
void yyerror(const char *s);
extern int yylineno;
extern FILE *yyin;

typedef enum {
    AST_OP_BINARIA, AST_NUMERO, AST_TEXTO, AST_VARIAVEL,
    AST_DECLARACAO, AST_ATRIBUICAO, AST_IMPRESSAO, AST_ENTRADA,
    AST_IF, AST_WHILE, AST_DECLARACAO_ARRAY, AST_ACESSO_ARRAY, AST_FOR
} TipoNo;

typedef struct No {
    TipoNo tipo;
    union {
        struct { char operador; struct No *esquerda, *direita; } op_binaria;
        struct { double valor; } numero;
        struct { char *valor; } texto;
        struct { char *nome; } variavel;
        struct { char *tipo, *nome; struct No *inicializacao; } declaracao;
        struct { struct No *target; struct No *valor; } atribuicao;
        struct { struct ListaNos *argumentos; } impressao;
        struct { char *mascara; struct No *destino; } entrada;
        struct { struct No *condicao; struct No *bloco_then; struct No *bloco_else; } se;
        struct { struct No *condicao; struct No *bloco_fazer; } enquanto;
        struct { struct No *inicializacao; struct No *condicao; struct No *incremento; struct No *bloco_fazer; } loop_for;
        struct { char *tipo_base; char *nome; struct No *tamanho; } declaracao_array;
        struct { char *nome_array; struct No *indice; } acesso_array;
    };
} No;

typedef struct ListaNos {
    No *no;
    struct ListaNos *proximo;
} ListaNos;

typedef struct {
    char *nome;
    char *tipo;
    union {
        double valor_numerico;
        char *valor_texto;
        struct {
            char *tipo_base;
            int tamanho;
            void *elementos;
        } array_info;
    } valor;
    UT_hash_handle hh;
} Variavel;

Variavel *tabela_simbolos = NULL;

void executar(No *no);
Variavel *buscar_variavel(const char *nome);
No *novo_op_binaria(char operador, No *esquerda, No *direita);
No *novo_numero(double valor);
No *novo_texto(char *texto);
No *novo_variavel(char *nome);
No *novo_declaracao(char *tipo, char *nome, No *inicializacao);
No *novo_atribuicao(No *target, No *valor);
No *novo_impressao(ListaNos *argumentos);
No *novo_entrada(char *mascara, No *destino);
No *novo_se(No *condicao, No *bloco_then, No *bloco_else);
No *novo_enquanto(No *condicao, No *bloco_fazer);
No *novo_for(No *inicializacao, No *condicao, No *incremento, No *bloco_fazer);
No *novo_declaracao_array(char *tipo_base, char *nome, No *tamanho);
No *novo_acesso_array(char *nome_array, No *indice);
ListaNos *nova_lista_nos(No *no, ListaNos *proximo);
void inserir_variavel(char *tipo_str, char *nome, No *inicializacao);
void inserir_array(char *tipo_base_str, char *nome, No *tamanho_no);
double avaliar_expressao(No *no);
void liberar_tabela_simbolos();
void liberar_ast(No *no);

Variavel *buscar_variavel(const char *nome) {
    Variavel *v;
    HASH_FIND_STR(tabela_simbolos, nome, v);
    return v;
}

No *novo_op_binaria(char operador, No *esquerda, No *direita) {
    No *n = malloc(sizeof(No));
    if (!n) { perror("Erro ao alocar novo_op_binaria"); exit(1); }
    n->tipo = AST_OP_BINARIA;
    n->op_binaria.operador = operador;
    n->op_binaria.esquerda = esquerda;
    n->op_binaria.direita = direita;
    return n;
}

No *novo_numero(double valor) {
    No *n = malloc(sizeof(No));
    if (!n) { perror("Erro ao alocar novo_numero"); exit(1); }
    n->tipo = AST_NUMERO;
    n->numero.valor = valor;
    return n;
}

No *novo_variavel(char *nome) {
    No *n = malloc(sizeof(No));
    if (!n) { perror("Erro ao alocar novo_variavel"); exit(1); }
    n->tipo = AST_VARIAVEL;
    n->variavel.nome = strdup(nome);
    return n;
}

No *novo_texto(char *texto) {
    No *n = malloc(sizeof(No));
    if (!n) { perror("Erro ao alocar novo_texto"); exit(1); }
    n->tipo = AST_TEXTO;
    n->texto.valor = strdup(texto);
    return n;
}

ListaNos *nova_lista_nos(No *no, ListaNos *proximo) {
    ListaNos *l = malloc(sizeof(ListaNos));
    if (!l) { perror("Erro ao alocar nova_lista_nos"); exit(1); }
    l->no = no;
    l->proximo = proximo;
    return l;
}

No *novo_impressao(ListaNos *argumentos) {
    No *n = malloc(sizeof(No));
    if (!n) { perror("Erro ao alocar novo_impressao"); exit(1); }
    n->tipo = AST_IMPRESSAO;
    n->impressao.argumentos = argumentos;
    return n;
}

No *novo_declaracao(char *tipo, char *nome, No *inicializacao) {
    No *n = malloc(sizeof(No));
    if (!n) { perror("Erro ao alocar novo_declaracao"); exit(1); }
    n->tipo = AST_DECLARACAO;
    char *tipo_mod = strdup(tipo);
    for (char *p = tipo_mod; *p; ++p){
        *p = toupper(*p);
    }
    n->declaracao.tipo = strdup(tipo_mod);
    n->declaracao.nome = strdup(nome);
    n->declaracao.inicializacao = inicializacao;
    free(tipo_mod);
    return n;
}

No *novo_atribuicao(No *target, No *valor) {
    No *n = malloc(sizeof(No));
    if (!n) { perror("Erro ao alocar novo_atribuicao"); exit(1); }
    n->tipo = AST_ATRIBUICAO;
    n->atribuicao.target = target;
    n->atribuicao.valor = valor;
    return n;
}

No *novo_entrada(char *mascara, No *destino) {
    No *n = malloc(sizeof(No));
    if (!n) { perror("Erro ao alocar novo_entrada"); exit(1); }
    n->tipo = AST_ENTRADA;
    n->entrada.mascara = strdup(mascara);
    n->entrada.destino = destino;
    return n;
}

No *novo_se(No *condicao, No *bloco_then, No *bloco_else) {
    No *n = malloc(sizeof(No));
    if (!n) { perror("Erro ao alocar novo_se"); exit(1); }
    n->tipo = AST_IF;
    n->se.condicao = condicao;
    n->se.bloco_then = bloco_then;
    n->se.bloco_else = bloco_else;
    return n;
}

No *novo_enquanto(No *condicao, No *bloco_fazer) {
    No *n = malloc(sizeof(No));
    if (!n) { perror("Erro ao alocar novo_enquanto"); exit(1); }
    n->tipo = AST_WHILE;
    n->enquanto.condicao = condicao;
    n->enquanto.bloco_fazer = bloco_fazer;
    return n;
}

No *novo_for(No *inicializacao, No *condicao, No *incremento, No *bloco_fazer) {
    No *n = malloc(sizeof(No));
    if (!n) { perror("Erro ao alocar novo_for"); exit(1); }
    n->tipo = AST_FOR;
    n->loop_for.inicializacao = inicializacao;
    n->loop_for.condicao = condicao;
    n->loop_for.incremento = incremento;
    n->loop_for.bloco_fazer = bloco_fazer;
    return n;
}

No *novo_declaracao_array(char *tipo_base, char *nome, No *tamanho) {
    No *n = malloc(sizeof(No));
    if (!n) { perror("Erro ao alocar novo_declaracao_array"); exit(1); }
    n->tipo = AST_DECLARACAO_ARRAY;
    char *tipo_mod = strdup(tipo_base);
    for (char *p = tipo_mod; *p; ++p){
        *p = toupper(*p);
    }
    n->declaracao_array.tipo_base = strdup(tipo_mod);
    n->declaracao_array.nome = strdup(nome);
    n->declaracao_array.tamanho = tamanho;
    free(tipo_mod);
    return n;
}

No *novo_acesso_array(char *nome_array, No *indice) {
    No *n = malloc(sizeof(No));
    if (!n) { perror("Erro ao alocar novo_acesso_array"); exit(1); }
    n->tipo = AST_ACESSO_ARRAY;
    n->acesso_array.nome_array = strdup(nome_array);
    n->acesso_array.indice = indice;
    return n;
}

void inserir_variavel(char *tipo_str, char *nome, No *inicializacao) {
    if (buscar_variavel(nome)) {
        fprintf(stderr, "Erro na linha %d: Variável '%s' já declarada.\n", yylineno, nome);
        return;
    }

    Variavel *v = malloc(sizeof(Variavel));
    if (!v) { perror("Erro ao alocar Variavel"); exit(1); }
    v->nome = strdup(nome);
    v->tipo = strdup(tipo_str);

    if (strcmp(tipo_str, "NUM") == 0) {
        v->valor.valor_numerico = 0;
        if (inicializacao) {
            double val = avaliar_expressao(inicializacao);
            v->valor.valor_numerico = val;
        }
    } else if (strcmp(tipo_str, "STR") == 0) {
        v->valor.valor_texto = strdup("");
        if (inicializacao) {
            if (inicializacao->tipo == AST_TEXTO) {
                free(v->valor.valor_texto);
                v->valor.valor_texto = strdup(inicializacao->texto.valor);
            } else if (inicializacao->tipo == AST_VARIAVEL) {
                Variavel *orig_var = buscar_variavel(inicializacao->variavel.nome);
                if (orig_var && strcmp(orig_var->tipo, "STR") == 0) {
                    free(v->valor.valor_texto);
                    v->valor.valor_texto = strdup(orig_var->valor.valor_texto);
                } else {
                    fprintf(stderr, "Erro na linha %d: Inicialização de STR com tipo incompatível.\n", yylineno);
                }
            } else {
                fprintf(stderr, "Erro na linha %d: Inicialização incompatível para o tipo STR.\n", yylineno);
            }
        }
    }
    HASH_ADD_KEYPTR(hh, tabela_simbolos, v->nome, strlen(v->nome), v);
}

void inserir_array(char *tipo_base_str, char *nome, No *tamanho_no) {
    if (buscar_variavel(nome)) {
        fprintf(stderr, "Erro na linha %d: Array '%s' já declarado.\n", yylineno, nome);
        return;
    }

    double tamanho_val = avaliar_expressao(tamanho_no);
    if (tamanho_val <= 0 || fmod(tamanho_val, 1.0) != 0) {
        fprintf(stderr, "Erro na linha %d: Tamanho de array inválido para '%s'. Deve ser um inteiro positivo.\n", yylineno, nome);
        return;
    }
    int tamanho = (int)tamanho_val;

    Variavel *v = malloc(sizeof(Variavel));
    if (!v) { perror("Erro ao alocar Variavel para array"); exit(1); }
    v->nome = strdup(nome);
    v->tipo = (strcmp(tipo_base_str, "NUM") == 0) ? strdup("NUM_ARRAY") : strdup("STR_ARRAY");
    v->valor.array_info.tipo_base = strdup(tipo_base_str);
    v->valor.array_info.tamanho = tamanho;

    if (strcmp(tipo_base_str, "NUM") == 0) {
        v->valor.array_info.elementos = calloc(tamanho, sizeof(double));
        if (!v->valor.array_info.elementos) { perror("Erro ao alocar elementos NUM_ARRAY"); exit(1); }
    } else if (strcmp(tipo_base_str, "STR") == 0) {
        v->valor.array_info.elementos = calloc(tamanho, sizeof(char *));
        if (!v->valor.array_info.elementos) { perror("Erro ao alocar elementos STR_ARRAY"); exit(1); }
        for (int i = 0; i < tamanho; i++) {
            ((char **)v->valor.array_info.elementos)[i] = strdup("");
            if (!((char **)v->valor.array_info.elementos)[i]) { perror("Erro ao alocar string vazia no array"); exit(1); }
        }
    } else {
        fprintf(stderr, "Erro na linha %d: Tipo base de array '%s' não suportado. Use 'NUM' ou 'STR'.\n", yylineno, tipo_base_str);
        free(v->nome); free(v->tipo); free(v->valor.array_info.tipo_base); free(v);
        return;
    }

    HASH_ADD_KEYPTR(hh, tabela_simbolos, v->nome, strlen(v->nome), v);
}

double avaliar_expressao(No *no) {
    if (!no) return 0;

    switch (no->tipo) {
        case AST_NUMERO: {
            return no->numero.valor;
        }
        case AST_VARIAVEL: {
            Variavel *v = buscar_variavel(no->variavel.nome);
            if (!v) {
                fprintf(stderr, "Erro na linha %d: Variável '%s' não declarada.\n", yylineno, no->variavel.nome);
                return 0;
            }
            if (strcmp(v->tipo, "NUM") == 0) {
                return v->valor.valor_numerico;
            } else {
                fprintf(stderr, "Erro na linha %d: Tentativa de usar variável de tipo '%s' em expressão numérica.\n", yylineno, v->tipo);
                return 0;
            }
        }
        case AST_OP_BINARIA: {
            double esq = avaliar_expressao(no->op_binaria.esquerda);
            double dir = avaliar_expressao(no->op_binaria.direita);

            switch (no->op_binaria.operador) {
                case '+': return esq + dir;
                case '-': return esq - dir;
                case '*': return esq * dir;
                case '/': return (dir != 0) ? esq / dir : (fprintf(stderr, "Erro na linha %d: Divisão por zero.\n", yylineno), 0);
                case '<': return (esq < dir) ? 1 : 0;
                case '>': return (esq > dir) ? 1 : 0;
                case 'E': return (esq <= dir) ? 1 : 0; // <=
                case 'A': return (esq >= dir) ? 1 : 0; // >=
                case 'I': return (esq == dir) ? 1 : 0; // ==
                case 'D': return (esq != dir) ? 1 : 0; // !=
                default:
                    fprintf(stderr, "Erro na linha %d: Operador binário desconhecido '%c'.\n", yylineno, no->op_binaria.operador);
                    return 0;
            }
        }
        case AST_ACESSO_ARRAY: {
            Variavel *array_var = buscar_variavel(no->acesso_array.nome_array);
            if (!array_var) {
                fprintf(stderr, "Erro na linha %d: Array '%s' não declarado.\n", yylineno, no->acesso_array.nome_array);
                return 0;
            }
            if (strcmp(array_var->tipo, "NUM_ARRAY") != 0 && strcmp(array_var->tipo, "STR_ARRAY") != 0) {
                     fprintf(stderr, "Erro na linha %d: Variável '%s' não é um array.\n", yylineno, no->acesso_array.nome_array);
                     return 0;
            }

            double indice_val = avaliar_expressao(no->acesso_array.indice);
            if (indice_val < 0 || fmod(indice_val, 1.0) != 0 || indice_val >= array_var->valor.array_info.tamanho) {
                fprintf(stderr, "Erro na linha %d: Índice de array fora dos limites para '%s'. Índice: %.1f, Tamanho: %d.\n", yylineno, array_var->nome, indice_val, array_var->valor.array_info.tamanho);
                return 0;
            }
            int indice = (int)indice_val;

            if (strcmp(array_var->valor.array_info.tipo_base, "NUM") == 0) {
                return ((double *)array_var->valor.array_info.elementos)[indice];
            } else if (strcmp(array_var->valor.array_info.tipo_base, "STR") == 0) {
                fprintf(stderr, "Erro na linha %d: Tentativa de usar elemento de array de strings em expressão numérica.\n", yylineno);
                return 0;
            }
            break;
        }

        default:
            fprintf(stderr, "Erro na linha %d: Tipo de nó inválido em expressão: %d.\n", yylineno, no->tipo);
            return 0;
    }
    return 0;
}

void executar(No *no) {
    if (!no) return;

    if (no->tipo == AST_OP_BINARIA && no->op_binaria.operador == ';') {
        executar(no->op_binaria.esquerda);
        executar(no->op_binaria.direita);
        return;
    }

    switch (no->tipo) {
        case AST_DECLARACAO: {
            inserir_variavel(no->declaracao.tipo, no->declaracao.nome, no->declaracao.inicializacao);
            break;
        }
        case AST_DECLARACAO_ARRAY: {
            inserir_array(no->declaracao_array.tipo_base, no->declaracao_array.nome, no->declaracao_array.tamanho);
            break;
        }
        case AST_ATRIBUICAO: {
            if (no->atribuicao.target->tipo == AST_VARIAVEL) {
                Variavel *v = buscar_variavel(no->atribuicao.target->variavel.nome);
                if (!v) {
                    fprintf(stderr, "Erro na linha %d: Variável '%s' não declarada.\n", yylineno, no->atribuicao.target->variavel.nome);
                    return;
                }

                if (strcmp(v->tipo, "NUM") == 0) {
                    v->valor.valor_numerico = avaliar_expressao(no->atribuicao.valor);
                } else if (strcmp(v->tipo, "STR") == 0) {
                    if (no->atribuicao.valor->tipo == AST_TEXTO) {
                        free(v->valor.valor_texto);
                        v->valor.valor_texto = strdup(no->atribuicao.valor->texto.valor);
                    } else if (no->atribuicao.valor->tipo == AST_VARIAVEL) {
                        Variavel *orig_var = buscar_variavel(no->atribuicao.valor->variavel.nome);
                        if (orig_var && strcmp(orig_var->tipo, "STR") == 0) {
                            free(v->valor.valor_texto);
                            v->valor.valor_texto = strdup(orig_var->valor.valor_texto);
                        } else {
                            fprintf(stderr, "Erro na linha %d: Atribuição de tipo incompatível para STR.\n", yylineno);
                        }
                    } else {
                        fprintf(stderr, "Erro na linha %d: Atribuição de tipo incompatível para STR.\n", yylineno);
                    }
                } else {
                    fprintf(stderr, "Erro na linha %d: Variável '%s' é um array, use acesso com índice.\n", yylineno, v->nome);
                }
            } else if (no->atribuicao.target->tipo == AST_ACESSO_ARRAY) {
                char *array_name = no->atribuicao.target->acesso_array.nome_array;
                No *indice_node = no->atribuicao.target->acesso_array.indice;

                Variavel *array_var = buscar_variavel(array_name);
                if (!array_var) {
                    fprintf(stderr, "Erro na linha %d: Array '%s' não declarado.\n", yylineno, array_name);
                    return;
                }
                if (strcmp(array_var->tipo, "NUM_ARRAY") != 0 && strcmp(array_var->tipo, "STR_ARRAY") != 0) {
                    fprintf(stderr, "Erro na linha %d: '%s' não é um array.\n", yylineno, array_name);
                    return;
                }

                double indice_val = avaliar_expressao(indice_node);
                if (indice_val < 0 || fmod(indice_val, 1.0) != 0 || indice_val >= array_var->valor.array_info.tamanho) {
                    fprintf(stderr, "Erro na linha %d: Índice de array fora dos limites para '%s'. Índice: %.1f, Tamanho: %d.\n", yylineno, array_var->nome, indice_val, array_var->valor.array_info.tamanho);
                    return;
                }
                int indice = (int)indice_val;

                if (strcmp(array_var->valor.array_info.tipo_base, "NUM") == 0) {
                    double valor_atribuicao = avaliar_expressao(no->atribuicao.valor);
                    ((double *)array_var->valor.array_info.elementos)[indice] = valor_atribuicao;
                } else if (strcmp(array_var->valor.array_info.tipo_base, "STR") == 0) {
                    if (no->atribuicao.valor->tipo == AST_TEXTO) {
                        free(((char **)array_var->valor.array_info.elementos)[indice]);
                        ((char **)array_var->valor.array_info.elementos)[indice] = strdup(no->atribuicao.valor->texto.valor);
                    } else if (no->atribuicao.valor->tipo == AST_VARIAVEL) {
                             Variavel *orig_var = buscar_variavel(no->atribuicao.valor->variavel.nome);
                             if (orig_var && strcmp(orig_var->tipo, "STR") == 0) {
                                 free(((char **)array_var->valor.array_info.elementos)[indice]);
                                 ((char **)array_var->valor.array_info.elementos)[indice] = strdup(orig_var->valor.valor_texto);
                             } else {
                                 fprintf(stderr, "Erro na linha %d: Atribuição de tipo incompatível para array de strings.\n", yylineno);
                             }
                    } else {
                        fprintf(stderr, "Erro na linha %d: Atribuição de tipo incompatível para array de strings.\n", yylineno);
                    }
                }
            } else {
                fprintf(stderr, "Erro interno em tempo de execução na linha %d: Tipo de target de atribuição desconhecido: %d.\n", yylineno, no->atribuicao.target->tipo);
            }
            break;
        }
        case AST_IMPRESSAO: {
            ListaNos *arg = no->impressao.argumentos;
            while (arg) {
                if (arg->no->tipo == AST_VARIAVEL) {
                    Variavel *v = buscar_variavel(arg->no->variavel.nome);
                    if (v) {
                        if (strcmp(v->tipo, "NUM") == 0) {
                            printf("%.1f", v->valor.valor_numerico);
                        } else if (strcmp(v->tipo, "STR") == 0) {
                            printf("%s", v->valor.valor_texto);
                        } else {
                            fprintf(stderr, "Erro na linha %d: Tentativa de imprimir array sem índice: '%s'.\n", yylineno, v->nome);
                        }
                    } else {
                            fprintf(stderr, "Erro na linha %d: Variável '%s' não declarada para impressão.\n", yylineno, arg->no->variavel.nome);
                    }
                } else if (arg->no->tipo == AST_NUMERO) {
                    printf("%.1f", arg->no->numero.valor);
                } else if (arg->no->tipo == AST_TEXTO) {
                    printf("%s", arg->no->texto.valor);
                } else if (arg->no->tipo == AST_OP_BINARIA) {
                    printf("%.1f", avaliar_expressao(arg->no));
                } else if (arg->no->tipo == AST_ACESSO_ARRAY) {
                    Variavel *array_var = buscar_variavel(arg->no->acesso_array.nome_array);
                    if (!array_var) {
                        fprintf(stderr, "Erro na linha %d: Array '%s' não declarado para impressão.\n", yylineno, arg->no->acesso_array.nome_array);
                        arg = arg->proximo;
                        continue;
                    }
                    double indice_val = avaliar_expressao(arg->no->acesso_array.indice);
                    if (indice_val < 0 || fmod(indice_val, 1.0) != 0 || indice_val >= array_var->valor.array_info.tamanho) {
                        fprintf(stderr, "Erro na linha %d: Índice de array fora dos limites para impressão de '%s'.\n", yylineno, array_var->nome);
                        arg = arg->proximo;
                        continue;
                    }
                    int indice = (int)indice_val;

                    if (strcmp(array_var->valor.array_info.tipo_base, "NUM") == 0) {
                        printf("%.1f", ((double *)array_var->valor.array_info.elementos)[indice]);
                    } else if (strcmp(array_var->valor.array_info.tipo_base, "STR") == 0) {
                        printf("%s", ((char **)array_var->valor.array_info.elementos)[indice]);
                    }
                }
                arg = arg->proximo;
                if (arg) printf(" ");
            }
            printf("\n");
            break;
        }
        case AST_ENTRADA: {
            No *dest = no->entrada.destino;
            if (dest->tipo == AST_VARIAVEL) {
                Variavel *v = buscar_variavel(dest->variavel.nome);
                if (!v) {
                    fprintf(stderr, "Erro na linha %d: Variável '%s' não declarada.\n", yylineno, dest->variavel.nome);
                    return;
                }
                if (strcmp(v->tipo, "NUM") == 0) {
                    if (strcmp(no->entrada.mascara, "%i") == 0 || strcmp(no->entrada.mascara, "%d") == 0) {
                        int valor;
                        if (scanf("%d", &valor) == 1) {
                            v->valor.valor_numerico = (double)valor;
                        } else {
                            fprintf(stderr, "Erro na linha %d: Falha na leitura de inteiro.\n", yylineno);
                        }
                    } else if (strcmp(no->entrada.mascara, "%f") == 0) {
                        double valor;
                        if (scanf("%lf", &valor) == 1) {
                            v->valor.valor_numerico = valor;
                        } else {
                            fprintf(stderr, "Erro na linha %d: Falha na leitura de float.\n", yylineno);
                        }
                    } else {
                        fprintf(stderr, "Erro na linha %d: Máscara de formato '%s' incompatível com tipo NUM para entrada.\n", yylineno, no->entrada.mascara);
                    }
                } else if (strcmp(v->tipo, "STR") == 0) {
                    if (strcmp(no->entrada.mascara, "%s") == 0) {
                        char buffer[256];
                        if (scanf("%255s", buffer) == 1) {
                            free(v->valor.valor_texto);
                            v->valor.valor_texto = strdup(buffer);
                        } else {
                            fprintf(stderr, "Erro na linha %d: Falha na leitura de string.\n", yylineno);
                        }
                    } else {
                        fprintf(stderr, "Erro na linha %d: Máscara de formato '%s' incompatível com tipo STR para entrada.\n", yylineno, no->entrada.mascara);
                    }
                } else {
                    fprintf(stderr, "Erro na linha %d: STDIN não suporta variáveis do tipo '%s'.\n", yylineno, v->tipo);
                }
            } else if (dest->tipo == AST_ACESSO_ARRAY) {
                char *array_name = dest->acesso_array.nome_array;
                No *indice_node = dest->acesso_array.indice;
        
                Variavel *array_var = buscar_variavel(array_name);
                if (!array_var) {
                    fprintf(stderr, "Erro na linha %d: Array '%s' não declarado.\n", yylineno, array_name);
                    return;
                }
                if (strcmp(array_var->tipo, "NUM_ARRAY") != 0 && strcmp(array_var->tipo, "STR_ARRAY") != 0) {
                    fprintf(stderr, "Erro na linha %d: '%s' não é um array.\n", yylineno, array_name);
                    return;
                }
        
                double indice_val = avaliar_expressao(indice_node);
                if (indice_val < 0 || fmod(indice_val, 1.0) != 0 || indice_val >= array_var->valor.array_info.tamanho) {
                    fprintf(stderr, "Erro na linha %d: Índice de array fora dos limites para '%s'. Índice: %.1f, Tamanho: %d.\n", yylineno, array_var->nome, indice_val, array_var->valor.array_info.tamanho);
                    return;
                }
                int indice = (int)indice_val;
        
                if (strcmp(array_var->valor.array_info.tipo_base, "NUM") == 0) {
                    if (strcmp(no->entrada.mascara, "%i") == 0 || strcmp(no->entrada.mascara, "%d") == 0) {
                        int valor;
                        if (scanf("%d", &valor) == 1) {
                            ((double *)array_var->valor.array_info.elementos)[indice] = (double)valor;
                        } else {
                            fprintf(stderr, "Erro na linha %d: Falha na leitura de inteiro para array.\n", yylineno);
                        }
                    } else if (strcmp(no->entrada.mascara, "%f") == 0) {
                        double valor;
                        if (scanf("%lf", &valor) == 1) {
                            ((double *)array_var->valor.array_info.elementos)[indice] = valor;
                        } else {
                            fprintf(stderr, "Erro na linha %d: Falha na leitura de float para array.\n", yylineno);
                        }
                    } else {
                        fprintf(stderr, "Erro na linha %d: Máscara de formato '%s' incompatível com array NUM.\n", yylineno, no->entrada.mascara);
                    }
                } else if (strcmp(array_var->valor.array_info.tipo_base, "STR") == 0) {
                    if (strcmp(no->entrada.mascara, "%s") == 0) {
                        char buffer[256];
                        if (scanf("%255s", buffer) == 1) {
                            free(((char **)array_var->valor.array_info.elementos)[indice]);
                            ((char **)array_var->valor.array_info.elementos)[indice] = strdup(buffer);
                        } else {
                            fprintf(stderr, "Erro na linha %d: Falha na leitura de string para array.\n", yylineno);
                        }
                    } else {
                        fprintf(stderr, "Erro na linha %d: Máscara de formato '%s' incompatível com array STR.\n", yylineno, no->entrada.mascara);
                    }
                }
            }
            int c; while ((c = getchar()) != '\n' && c != EOF);
            break;
        }
        case AST_IF: {
            double condicao_resultado = avaliar_expressao(no->se.condicao);
            if (condicao_resultado != 0) {
                executar(no->se.bloco_then);
            } else if (no->se.bloco_else) {
                executar(no->se.bloco_else);
            }
            break;
        }
        case AST_WHILE: {
            while (avaliar_expressao(no->enquanto.condicao) != 0) {
                executar(no->enquanto.bloco_fazer);
            }
            break;
        }
        case AST_FOR: {
            executar(no->loop_for.inicializacao);
            while (avaliar_expressao(no->loop_for.condicao) != 0) {
                executar(no->loop_for.bloco_fazer);
                executar(no->loop_for.incremento);
            }
            break;
        }
        case AST_NUMERO:
        case AST_TEXTO:
        case AST_VARIAVEL:
        case AST_OP_BINARIA:
        case AST_ACESSO_ARRAY:
            break;

        default:
            fprintf(stderr, "Erro na linha %d: Tipo de nó não implementado para execução direta: %d.\n", yylineno, no->tipo);
            break;
    }
}

void liberar_tabela_simbolos() {
    Variavel *current_var, *tmp;
    HASH_ITER(hh, tabela_simbolos, current_var, tmp) {
        HASH_DEL(tabela_simbolos, current_var);
        free(current_var->nome);
        free(current_var->tipo);
        if (strcmp(current_var->tipo, "STR") == 0) {
            free(current_var->valor.valor_texto);
        } else if (strcmp(current_var->tipo, "NUM_ARRAY") == 0 || strcmp(current_var->tipo, "STR_ARRAY") == 0) {
            if (strcmp(current_var->valor.array_info.tipo_base, "STR") == 0) {
                char **str_array = (char **)current_var->valor.array_info.elementos;
                for (int i = 0; i < current_var->valor.array_info.tamanho; i++) {
                    free(str_array[i]);
                }
            }
            free(current_var->valor.array_info.tipo_base);
            free(current_var->valor.array_info.elementos);
        }
        free(current_var);
    }
}

void liberar_ast(No *no) {
    if (!no) return;
    switch (no->tipo) {
        case AST_OP_BINARIA:
            liberar_ast(no->op_binaria.esquerda);
            liberar_ast(no->op_binaria.direita);
            break;
        case AST_DECLARACAO:
            free(no->declaracao.tipo);
            free(no->declaracao.nome);
            liberar_ast(no->declaracao.inicializacao);
            break;
        case AST_ATRIBUICAO:
            liberar_ast(no->atribuicao.target);
            liberar_ast(no->atribuicao.valor);
            break;
        case AST_IMPRESSAO: {
            ListaNos *arg = no->impressao.argumentos;
            while (arg) {
                ListaNos *temp = arg;
                liberar_ast(arg->no);
                arg = arg->proximo;
                free(temp);
            }
            break;
        }
        case AST_ENTRADA:
            free(no->entrada.mascara);
            liberar_ast(no->entrada.destino);
            break;
        case AST_IF:
            liberar_ast(no->se.condicao);
            liberar_ast(no->se.bloco_then);
            liberar_ast(no->se.bloco_else);
            break;
        case AST_WHILE:
            liberar_ast(no->enquanto.condicao);
            liberar_ast(no->enquanto.bloco_fazer);
            break;
        case AST_FOR:
            liberar_ast(no->loop_for.inicializacao);
            liberar_ast(no->loop_for.condicao);
            liberar_ast(no->loop_for.incremento);
            liberar_ast(no->loop_for.bloco_fazer);
            break;
        case AST_DECLARACAO_ARRAY:
            free(no->declaracao_array.tipo_base);
            free(no->declaracao_array.nome);
            liberar_ast(no->declaracao_array.tamanho);
            break;
        case AST_ACESSO_ARRAY:
            free(no->acesso_array.nome_array);
            liberar_ast(no->acesso_array.indice);
            break;
        case AST_TEXTO:
            free(no->texto.valor);
            break;
        case AST_VARIAVEL:
            free(no->variavel.nome);
            break;
        case AST_NUMERO:
            break;
    }
    free(no);
}


#line 811 "sintatico.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "sintatico.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_PRINT = 3,                      /* PRINT  */
  YYSYMBOL_STDIN = 4,                      /* STDIN  */
  YYSYMBOL_IF = 5,                         /* IF  */
  YYSYMBOL_ELSE = 6,                       /* ELSE  */
  YYSYMBOL_WHILE = 7,                      /* WHILE  */
  YYSYMBOL_ME = 8,                         /* ME  */
  YYSYMBOL_MA = 9,                         /* MA  */
  YYSYMBOL_IG = 10,                        /* IG  */
  YYSYMBOL_DI = 11,                        /* DI  */
  YYSYMBOL_FOR = 12,                       /* FOR  */
  YYSYMBOL_TIPO = 13,                      /* TIPO  */
  YYSYMBOL_VAR = 14,                       /* VAR  */
  YYSYMBOL_STRING = 15,                    /* STRING  */
  YYSYMBOL_MASCARA = 16,                   /* MASCARA  */
  YYSYMBOL_NUMERO = 17,                    /* NUMERO  */
  YYSYMBOL_18_ = 18,                       /* '['  */
  YYSYMBOL_19_ = 19,                       /* ']'  */
  YYSYMBOL_20_ = 20,                       /* '<'  */
  YYSYMBOL_21_ = 21,                       /* '>'  */
  YYSYMBOL_22_ = 22,                       /* '+'  */
  YYSYMBOL_23_ = 23,                       /* '-'  */
  YYSYMBOL_24_ = 24,                       /* '*'  */
  YYSYMBOL_25_ = 25,                       /* '/'  */
  YYSYMBOL_26_ = 26,                       /* '('  */
  YYSYMBOL_27_ = 27,                       /* ')'  */
  YYSYMBOL_28_ = 28,                       /* '{'  */
  YYSYMBOL_29_ = 29,                       /* '}'  */
  YYSYMBOL_30_ = 30,                       /* ';'  */
  YYSYMBOL_31_ = 31,                       /* ','  */
  YYSYMBOL_32_ = 32,                       /* '='  */
  YYSYMBOL_YYACCEPT = 33,                  /* $accept  */
  YYSYMBOL_programa = 34,                  /* programa  */
  YYSYMBOL_bloco = 35,                     /* bloco  */
  YYSYMBOL_instrucao = 36,                 /* instrucao  */
  YYSYMBOL_if_sem_else = 37,               /* if_sem_else  */
  YYSYMBOL_if_com_else = 38,               /* if_com_else  */
  YYSYMBOL_if_instrucao = 39,              /* if_instrucao  */
  YYSYMBOL_while_instrucao = 40,           /* while_instrucao  */
  YYSYMBOL_for_instrucao = 41,             /* for_instrucao  */
  YYSYMBOL_condicao = 42,                  /* condicao  */
  YYSYMBOL_calcula = 43,                   /* calcula  */
  YYSYMBOL_valor = 44,                     /* valor  */
  YYSYMBOL_escreve = 45,                   /* escreve  */
  YYSYMBOL_argumentos = 46,                /* argumentos  */
  YYSYMBOL_declaracao = 47,                /* declaracao  */
  YYSYMBOL_mult_tipo = 48,                 /* mult_tipo  */
  YYSYMBOL_atribuicao = 49,                /* atribuicao  */
  YYSYMBOL_entrada = 50,                   /* entrada  */
  YYSYMBOL_acesso_array = 51               /* acesso_array  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   190

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  33
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  19
/* YYNRULES -- Number of rules.  */
#define YYNRULES  46
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  104

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   272


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      26,    27,    24,    22,    31,    23,     2,    25,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    30,
      20,    32,    21,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    18,     2,    19,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    28,     2,    29,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   764,   764,   767,   768,   771,   772,   773,   774,   775,
     776,   777,   778,   781,   784,   785,   788,   789,   792,   795,
     798,   799,   800,   801,   802,   803,   806,   807,   808,   809,
     810,   811,   814,   815,   816,   817,   820,   823,   824,   827,
     828,   829,   832,   833,   836,   839,   842
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "PRINT", "STDIN", "IF",
  "ELSE", "WHILE", "ME", "MA", "IG", "DI", "FOR", "TIPO", "VAR", "STRING",
  "MASCARA", "NUMERO", "'['", "']'", "'<'", "'>'", "'+'", "'-'", "'*'",
  "'/'", "'('", "')'", "'{'", "'}'", "';'", "','", "'='", "$accept",
  "programa", "bloco", "instrucao", "if_sem_else", "if_com_else",
  "if_instrucao", "while_instrucao", "for_instrucao", "condicao",
  "calcula", "valor", "escreve", "argumentos", "declaracao", "mult_tipo",
  "atribuicao", "entrada", "acesso_array", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-75)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-44)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -75,    10,   142,   -75,    -6,     8,    11,    14,    16,    22,
     -15,   -75,   -75,    18,   -75,   -75,   -75,   -75,   -75,   -75,
      43,   -75,   -75,   -75,    -8,   -75,   -75,    13,    18,    27,
      18,    18,    33,   -11,    18,    30,   163,   -75,    18,    18,
      18,    18,    18,    53,    23,    26,    36,   152,    46,    19,
      18,    18,   -10,   -75,   -19,   -19,   -75,   -75,    43,    18,
     -75,    65,    52,    18,    18,    18,    18,    18,    18,    54,
      18,   159,    43,   -75,   -75,    30,    58,   -75,   -75,    43,
      43,    43,    43,    43,    43,   -75,    51,   -75,   -75,    57,
      88,    65,    84,   -75,    67,     3,    68,   -75,   -75,   -75,
     106,   124,   -75,   -75
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     2,     1,     0,     0,     0,     0,     0,     0,
      33,    34,    32,     0,     4,    16,    17,    10,    11,    12,
       5,    31,     6,     7,     0,     8,     9,    35,     0,     0,
       0,     0,     0,    40,     0,    33,     0,    35,     0,     0,
       0,     0,     0,    38,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    30,    26,    27,    28,    29,    44,     0,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    39,    46,    37,    42,     0,    43,     3,    22,
      23,    24,    25,    20,    21,     3,     0,    41,    45,     0,
       0,     0,    13,    18,     0,     0,     0,     3,    14,     3,
       0,     0,    15,    19
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -75,   -75,   -74,   -75,   -75,   -75,     2,   -75,   -75,   -29,
     -12,   -75,   -75,    45,    74,    47,    21,   -75,    -2
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,    14,    15,    16,    17,    18,    19,    46,
      20,    21,    22,    44,    23,    24,    25,    26,    37
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      27,    36,    48,    34,    89,    40,    41,    50,     6,    73,
       3,    90,    38,    39,    40,    41,    43,   -42,    47,    47,
      28,    51,    52,   100,    42,   101,    54,    55,    56,    57,
      58,    97,    35,    11,    29,    12,    33,    30,    71,    72,
      31,    86,    32,    45,    13,   -43,     9,    43,    34,    70,
      60,    79,    80,    81,    82,    83,    84,    61,    47,    77,
       4,     5,     6,    62,     7,    38,    39,    40,    41,     8,
       9,    10,    11,    69,    12,    38,    39,    40,    41,    75,
      78,    91,    85,    13,    59,    88,    92,    27,    27,    77,
      95,     4,     5,     6,    96,     7,    99,    98,    27,    27,
       8,     9,    10,    11,    74,    12,    49,     0,    76,     4,
       5,     6,    94,     7,    13,     0,     0,    93,     8,     9,
      10,    11,     0,    12,     0,     0,     0,     4,     5,     6,
       0,     7,    13,     0,     0,   102,     8,     9,    10,    11,
       0,    12,     0,     0,     0,     4,     5,     6,     0,     7,
      13,     0,     0,   103,     8,     9,    10,    11,     0,    12,
      63,    64,    65,    66,     0,     0,     0,     0,    13,     0,
       0,     0,    67,    68,    38,    39,    40,    41,    87,     0,
       0,    38,    39,    40,    41,    38,    39,    40,    41,     0,
      53
};

static const yytype_int8 yycheck[] =
{
       2,    13,    31,    18,    78,    24,    25,    18,     5,    19,
       0,    85,    22,    23,    24,    25,    28,    32,    30,    31,
      26,    32,    34,    97,    32,    99,    38,    39,    40,    41,
      42,    28,    14,    15,    26,    17,    14,    26,    50,    51,
      26,    70,    26,    16,    26,    32,    13,    59,    18,    30,
      27,    63,    64,    65,    66,    67,    68,    31,    70,    61,
       3,     4,     5,    27,     7,    22,    23,    24,    25,    12,
      13,    14,    15,    27,    17,    22,    23,    24,    25,    14,
      28,    30,    28,    26,    31,    27,    29,    89,    90,    91,
       6,     3,     4,     5,    27,     7,    28,    95,   100,   101,
      12,    13,    14,    15,    59,    17,    32,    -1,    61,     3,
       4,     5,    91,     7,    26,    -1,    -1,    29,    12,    13,
      14,    15,    -1,    17,    -1,    -1,    -1,     3,     4,     5,
      -1,     7,    26,    -1,    -1,    29,    12,    13,    14,    15,
      -1,    17,    -1,    -1,    -1,     3,     4,     5,    -1,     7,
      26,    -1,    -1,    29,    12,    13,    14,    15,    -1,    17,
       8,     9,    10,    11,    -1,    -1,    -1,    -1,    26,    -1,
      -1,    -1,    20,    21,    22,    23,    24,    25,    19,    -1,
      -1,    22,    23,    24,    25,    22,    23,    24,    25,    -1,
      27
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    34,    35,     0,     3,     4,     5,     7,    12,    13,
      14,    15,    17,    26,    36,    37,    38,    39,    40,    41,
      43,    44,    45,    47,    48,    49,    50,    51,    26,    26,
      26,    26,    26,    14,    18,    14,    43,    51,    22,    23,
      24,    25,    32,    43,    46,    16,    42,    43,    42,    47,
      18,    32,    43,    27,    43,    43,    43,    43,    43,    31,
      27,    31,    27,     8,     9,    10,    11,    20,    21,    27,
      30,    43,    43,    19,    46,    14,    48,    51,    28,    43,
      43,    43,    43,    43,    43,    28,    42,    19,    27,    35,
      35,    30,    29,    29,    49,     6,    27,    28,    39,    28,
      35,    35,    29,    29
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    33,    34,    35,    35,    36,    36,    36,    36,    36,
      36,    36,    36,    37,    38,    38,    39,    39,    40,    41,
      42,    42,    42,    42,    42,    42,    43,    43,    43,    43,
      43,    43,    44,    44,    44,    44,    45,    46,    46,    47,
      47,    47,    48,    48,    49,    50,    51
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     7,     9,    11,     1,     1,     7,    11,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     1,     4,     3,     1,     4,
       2,     5,     1,     1,     3,     6,     4
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* programa: bloco  */
#line 764 "sintatico.y"
                { executar((yyvsp[0].no)); liberar_ast((yyvsp[0].no)); liberar_tabela_simbolos(); }
#line 1925 "sintatico.tab.c"
    break;

  case 3: /* bloco: %empty  */
#line 767 "sintatico.y"
                        { (yyval.no) = NULL; }
#line 1931 "sintatico.tab.c"
    break;

  case 4: /* bloco: bloco instrucao  */
#line 768 "sintatico.y"
                        { (yyval.no) = ((yyvsp[-1].no) == NULL) ? (yyvsp[0].no) : novo_op_binaria(';', (yyvsp[-1].no), (yyvsp[0].no)); }
#line 1937 "sintatico.tab.c"
    break;

  case 5: /* instrucao: calcula  */
#line 771 "sintatico.y"
                             { (yyval.no) = (yyvsp[0].no); }
#line 1943 "sintatico.tab.c"
    break;

  case 6: /* instrucao: escreve  */
#line 772 "sintatico.y"
                             { (yyval.no) = (yyvsp[0].no); }
#line 1949 "sintatico.tab.c"
    break;

  case 7: /* instrucao: declaracao  */
#line 773 "sintatico.y"
                             { (yyval.no) = (yyvsp[0].no); }
#line 1955 "sintatico.tab.c"
    break;

  case 8: /* instrucao: atribuicao  */
#line 774 "sintatico.y"
                             { (yyval.no) = (yyvsp[0].no); }
#line 1961 "sintatico.tab.c"
    break;

  case 9: /* instrucao: entrada  */
#line 775 "sintatico.y"
                             { (yyval.no) = (yyvsp[0].no); }
#line 1967 "sintatico.tab.c"
    break;

  case 10: /* instrucao: if_instrucao  */
#line 776 "sintatico.y"
                             { (yyval.no) = (yyvsp[0].no); }
#line 1973 "sintatico.tab.c"
    break;

  case 11: /* instrucao: while_instrucao  */
#line 777 "sintatico.y"
                             { (yyval.no) = (yyvsp[0].no); }
#line 1979 "sintatico.tab.c"
    break;

  case 12: /* instrucao: for_instrucao  */
#line 778 "sintatico.y"
                             { (yyval.no) = (yyvsp[0].no); }
#line 1985 "sintatico.tab.c"
    break;

  case 13: /* if_sem_else: IF '(' condicao ')' '{' bloco '}'  */
#line 781 "sintatico.y"
                                                { (yyval.no) = novo_se((yyvsp[-4].no), (yyvsp[-1].no), NULL); }
#line 1991 "sintatico.tab.c"
    break;

  case 14: /* if_com_else: IF '(' condicao ')' '{' bloco '}' ELSE if_instrucao  */
#line 784 "sintatico.y"
                                                                     { (yyval.no) = novo_se((yyvsp[-6].no), (yyvsp[-3].no), (yyvsp[0].no)); }
#line 1997 "sintatico.tab.c"
    break;

  case 15: /* if_com_else: IF '(' condicao ')' '{' bloco '}' ELSE '{' bloco '}'  */
#line 785 "sintatico.y"
                                                                      { (yyval.no) = novo_se((yyvsp[-8].no), (yyvsp[-5].no), (yyvsp[-1].no)); }
#line 2003 "sintatico.tab.c"
    break;

  case 18: /* while_instrucao: WHILE '(' condicao ')' '{' bloco '}'  */
#line 792 "sintatico.y"
                                                        { (yyval.no) = novo_enquanto((yyvsp[-4].no), (yyvsp[-1].no)); }
#line 2009 "sintatico.tab.c"
    break;

  case 19: /* for_instrucao: FOR '(' declaracao ';' condicao ';' atribuicao ')' '{' bloco '}'  */
#line 795 "sintatico.y"
                                                                                { (yyval.no) = novo_for((yyvsp[-8].no), (yyvsp[-6].no), (yyvsp[-4].no), (yyvsp[-1].no)); }
#line 2015 "sintatico.tab.c"
    break;

  case 20: /* condicao: calcula '<' calcula  */
#line 798 "sintatico.y"
                                   { (yyval.no) = novo_op_binaria('<', (yyvsp[-2].no), (yyvsp[0].no)); }
#line 2021 "sintatico.tab.c"
    break;

  case 21: /* condicao: calcula '>' calcula  */
#line 799 "sintatico.y"
                                   { (yyval.no) = novo_op_binaria('>', (yyvsp[-2].no), (yyvsp[0].no)); }
#line 2027 "sintatico.tab.c"
    break;

  case 22: /* condicao: calcula ME calcula  */
#line 800 "sintatico.y"
                                   { (yyval.no) = novo_op_binaria('E', (yyvsp[-2].no), (yyvsp[0].no)); }
#line 2033 "sintatico.tab.c"
    break;

  case 23: /* condicao: calcula MA calcula  */
#line 801 "sintatico.y"
                                   { (yyval.no) = novo_op_binaria('A', (yyvsp[-2].no), (yyvsp[0].no)); }
#line 2039 "sintatico.tab.c"
    break;

  case 24: /* condicao: calcula IG calcula  */
#line 802 "sintatico.y"
                                   { (yyval.no) = novo_op_binaria('I', (yyvsp[-2].no), (yyvsp[0].no)); }
#line 2045 "sintatico.tab.c"
    break;

  case 25: /* condicao: calcula DI calcula  */
#line 803 "sintatico.y"
                                   { (yyval.no) = novo_op_binaria('D', (yyvsp[-2].no), (yyvsp[0].no)); }
#line 2051 "sintatico.tab.c"
    break;

  case 26: /* calcula: calcula '+' calcula  */
#line 806 "sintatico.y"
                                   { (yyval.no) = novo_op_binaria('+', (yyvsp[-2].no), (yyvsp[0].no)); }
#line 2057 "sintatico.tab.c"
    break;

  case 27: /* calcula: calcula '-' calcula  */
#line 807 "sintatico.y"
                                    { (yyval.no) = novo_op_binaria('-', (yyvsp[-2].no), (yyvsp[0].no)); }
#line 2063 "sintatico.tab.c"
    break;

  case 28: /* calcula: calcula '*' calcula  */
#line 808 "sintatico.y"
                                    { (yyval.no) = novo_op_binaria('*', (yyvsp[-2].no), (yyvsp[0].no)); }
#line 2069 "sintatico.tab.c"
    break;

  case 29: /* calcula: calcula '/' calcula  */
#line 809 "sintatico.y"
                                    { (yyval.no) = novo_op_binaria('/', (yyvsp[-2].no), (yyvsp[0].no)); }
#line 2075 "sintatico.tab.c"
    break;

  case 30: /* calcula: '(' calcula ')'  */
#line 810 "sintatico.y"
                                    { (yyval.no) = (yyvsp[-1].no); }
#line 2081 "sintatico.tab.c"
    break;

  case 31: /* calcula: valor  */
#line 811 "sintatico.y"
                                    { (yyval.no) = (yyvsp[0].no); }
#line 2087 "sintatico.tab.c"
    break;

  case 32: /* valor: NUMERO  */
#line 814 "sintatico.y"
                                    { (yyval.no) = novo_numero((yyvsp[0].valor_numerico)); }
#line 2093 "sintatico.tab.c"
    break;

  case 33: /* valor: VAR  */
#line 815 "sintatico.y"
                                    { (yyval.no) = novo_variavel((yyvsp[0].valor_texto)); }
#line 2099 "sintatico.tab.c"
    break;

  case 34: /* valor: STRING  */
#line 816 "sintatico.y"
                                    { (yyval.no) = novo_texto((yyvsp[0].valor_texto)); }
#line 2105 "sintatico.tab.c"
    break;

  case 35: /* valor: acesso_array  */
#line 817 "sintatico.y"
                                    { (yyval.no) = (yyvsp[0].no); }
#line 2111 "sintatico.tab.c"
    break;

  case 36: /* escreve: PRINT '(' argumentos ')'  */
#line 820 "sintatico.y"
                                    { (yyval.no) = novo_impressao((yyvsp[-1].lista_nos)); }
#line 2117 "sintatico.tab.c"
    break;

  case 37: /* argumentos: calcula ',' argumentos  */
#line 823 "sintatico.y"
                                     { (yyval.lista_nos) = nova_lista_nos((yyvsp[-2].no), (yyvsp[0].lista_nos)); }
#line 2123 "sintatico.tab.c"
    break;

  case 38: /* argumentos: calcula  */
#line 824 "sintatico.y"
                                     { (yyval.lista_nos) = nova_lista_nos((yyvsp[0].no), NULL); }
#line 2129 "sintatico.tab.c"
    break;

  case 39: /* declaracao: TIPO VAR '=' calcula  */
#line 827 "sintatico.y"
                                    { (yyval.no) = novo_declaracao((yyvsp[-3].valor_texto), (yyvsp[-2].valor_texto), (yyvsp[0].no)); }
#line 2135 "sintatico.tab.c"
    break;

  case 40: /* declaracao: TIPO VAR  */
#line 828 "sintatico.y"
                                    { (yyval.no) = novo_declaracao((yyvsp[-1].valor_texto), (yyvsp[0].valor_texto), NULL); }
#line 2141 "sintatico.tab.c"
    break;

  case 41: /* declaracao: TIPO VAR '[' calcula ']'  */
#line 829 "sintatico.y"
                                     { (yyval.no) = novo_declaracao_array((yyvsp[-4].valor_texto), (yyvsp[-3].valor_texto), (yyvsp[-1].no)); }
#line 2147 "sintatico.tab.c"
    break;

  case 42: /* mult_tipo: VAR  */
#line 832 "sintatico.y"
                                    { (yyval.no) = novo_variavel((yyvsp[0].valor_texto)); }
#line 2153 "sintatico.tab.c"
    break;

  case 43: /* mult_tipo: acesso_array  */
#line 833 "sintatico.y"
                                    { (yyval.no) = (yyvsp[0].no); }
#line 2159 "sintatico.tab.c"
    break;

  case 44: /* atribuicao: mult_tipo '=' calcula  */
#line 836 "sintatico.y"
                                     { (yyval.no) = novo_atribuicao((yyvsp[-2].no), (yyvsp[0].no)); }
#line 2165 "sintatico.tab.c"
    break;

  case 45: /* entrada: STDIN '(' MASCARA ',' mult_tipo ')'  */
#line 839 "sintatico.y"
                                             { (yyval.no) = novo_entrada((yyvsp[-3].valor_texto), (yyvsp[-1].no)); }
#line 2171 "sintatico.tab.c"
    break;

  case 46: /* acesso_array: VAR '[' calcula ']'  */
#line 842 "sintatico.y"
                                    { (yyval.no) = novo_acesso_array((yyvsp[-3].valor_texto), (yyvsp[-1].no)); }
#line 2177 "sintatico.tab.c"
    break;


#line 2181 "sintatico.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 845 "sintatico.y"


#include "lex.yy.c"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <nome_arquivo.rag>\n", argv[0]);
        return 1;
    }

    char *arquivo = argv[1];
    char *extensao = strrchr(arquivo, '.'); 

    if (!extensao || strcmp(extensao, ".rag") != 0) {
        fprintf(stderr, "Erro: O arquivo de entrada deve ter a extensão '.rag'.\n");
        return 1;
    }

    FILE *codigo = fopen(arquivo, "r");
    if (!codigo) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }
    yyin = codigo;
    yyparse();
    fclose(codigo);
    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "Erro de sintaxe na linha %d: %s\n", yylineno, s);
}

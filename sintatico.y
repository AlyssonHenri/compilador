%{
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
    AST_IF, AST_WHILE, AST_DECLARACAO_ARRAY, AST_ACESSO_ARRAY
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
        struct { char *mascara; char *variavel; } entrada;
        struct { struct No *condicao; struct No *bloco_then; struct No *bloco_else; } se;
        struct { struct No *condicao; struct No *bloco_fazer; } enquanto;
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
No *novo_entrada(char *mascara, char *variavel);
No *novo_se(No *condicao, No *bloco_then, No *bloco_else);
No *novo_enquanto(No *condicao, No *bloco_fazer);
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

No *novo_entrada(char *mascara, char *variavel) {
    No *n = malloc(sizeof(No));
    if (!n) { perror("Erro ao alocar novo_entrada"); exit(1); }
    n->tipo = AST_ENTRADA;
    n->entrada.mascara = strdup(mascara);
    n->entrada.variavel = strdup(variavel);
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
        fprintf(stderr, "Erro semântico na linha %d: Variável '%s' já declarada.\n", yylineno, nome);
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
                    fprintf(stderr, "Erro semântico na linha %d: Inicialização de STR com tipo incompatível.\n", yylineno);
                }
            } else {
                fprintf(stderr, "Erro semântico na linha %d: Inicialização incompatível para o tipo STR.\n", yylineno);
            }
        }
    }
    HASH_ADD_KEYPTR(hh, tabela_simbolos, v->nome, strlen(v->nome), v);
}

void inserir_array(char *tipo_base_str, char *nome, No *tamanho_no) {
    if (buscar_variavel(nome)) {
        fprintf(stderr, "Erro semântico na linha %d: Array '%s' já declarado.\n", yylineno, nome);
        return;
    }

    double tamanho_val = avaliar_expressao(tamanho_no);
    if (tamanho_val <= 0 || fmod(tamanho_val, 1.0) != 0) {
        fprintf(stderr, "Erro semântico na linha %d: Tamanho de array inválido para '%s'. Deve ser um inteiro positivo.\n", yylineno, nome);
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
        fprintf(stderr, "Erro semântico na linha %d: Tipo base de array '%s' não suportado. Use 'NUM' ou 'STR'.\n", yylineno, tipo_base_str);
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
                fprintf(stderr, "Erro na linha %d: Índice de array fora dos limites para '%s'. Índice: %.0f, Tamanho: %d.\n", yylineno, array_var->nome, indice_val, array_var->valor.array_info.tamanho);
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
                    fprintf(stderr, "Erro na linha %d: Índice de array fora dos limites para '%s'. Índice: %.0f, Tamanho: %d.\n", yylineno, array_var->nome, indice_val, array_var->valor.array_info.tamanho);
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
                            printf("%.0f", v->valor.valor_numerico);
                        } else if (strcmp(v->tipo, "STR") == 0) {
                            printf("%s", v->valor.valor_texto);
                        } else {
                             fprintf(stderr, "Erro na linha %d: Tentativa de imprimir array sem índice: '%s'.\n", yylineno, v->nome);
                        }
                    } else {
                         fprintf(stderr, "Erro na linha %d: Variável '%s' não declarada para impressão.\n", yylineno, arg->no->variavel.nome);
                    }
                } else if (arg->no->tipo == AST_NUMERO) {
                    printf("%.0f", arg->no->numero.valor);
                } else if (arg->no->tipo == AST_TEXTO) {
                    printf("%s", arg->no->texto.valor);
                } else if (arg->no->tipo == AST_OP_BINARIA) {
                    printf("%.0f", avaliar_expressao(arg->no));
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
                        printf("%.0f", ((double *)array_var->valor.array_info.elementos)[indice]);
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
            Variavel *v = buscar_variavel(no->entrada.variavel);
            if (!v) {
                fprintf(stderr, "Erro na linha %d: Variável '%s' não declarada.\n", yylineno, no->entrada.variavel);
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
            free(no->entrada.variavel);
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

%}

%union {
    double valor_numerico;
    char *valor_texto;
    No *no;
    ListaNos *lista_nos;
}

%token PRINT STDIN IF ELSE WHILE ME MA IG DI
%token <valor_texto> TIPO VAR STRING MASCARA
%token <valor_numerico> NUMERO

%token '[' ']'

%nonassoc '<' '>' ME MA IG DI
%left '+' '-'
%left '*' '/'

%type <no> valor calcula instrucao declaracao atribuicao entrada escreve programa bloco
%type <no> condicao if_instrucao while_instrucao acesso_array
%type <lista_nos> argumentos
%type <no> lvalue

%%

programa: bloco { executar($1); liberar_ast($1); liberar_tabela_simbolos(); }
    ;

bloco:                 { $$ = NULL; }
     | bloco instrucao { $$ = ($1 == NULL) ? $2 : novo_op_binaria(';', $1, $2); }
     ;

instrucao: calcula                 { $$ = $1; }
         | escreve                 { $$ = $1; }
         | declaracao              { $$ = $1; }
         | atribuicao              { $$ = $1; }
         | entrada                 { $$ = $1; }
         | if_instrucao            { $$ = $1; }
         | while_instrucao         { $$ = $1; }
         ;

if_instrucao: IF '(' condicao ')' '{' bloco '}'                 { $$ = novo_se($3, $6, NULL); }
            | IF '(' condicao ')' '{' bloco '}' ELSE '{' bloco '}' { $$ = novo_se($3, $6, $10); }
            ;

while_instrucao: WHILE '(' condicao ')' '{' bloco '}'          { $$ = novo_enquanto($3, $6); }
               ;

condicao: calcula '<' calcula      { $$ = novo_op_binaria('<', $1, $3); }
        | calcula '>' calcula      { $$ = novo_op_binaria('>', $1, $3); }
        | calcula ME calcula       { $$ = novo_op_binaria('E', $1, $3); } // <=
        | calcula MA calcula       { $$ = novo_op_binaria('A', $1, $3); } // >=
        | calcula IG calcula       { $$ = novo_op_binaria('I', $1, $3); } // ==
        | calcula DI calcula       { $$ = novo_op_binaria('D', $1, $3); } // !=
        ;

calcula: valor
       | calcula '+' valor         { $$ = novo_op_binaria('+', $1, $3); }
       | calcula '-' valor         { $$ = novo_op_binaria('-', $1, $3); }
       | calcula '*' valor         { $$ = novo_op_binaria('*', $1, $3); }
       | calcula '/' valor         { $$ = novo_op_binaria('/', $1, $3); }
       | '(' calcula ')'           { $$ = $2; }
       ;

valor: NUMERO                      { $$ = novo_numero($1); }
     | VAR                         { $$ = novo_variavel($1); }
     | STRING                      { $$ = novo_texto($1); }
     | acesso_array                { $$ = $1; }
     ;

escreve: PRINT '(' argumentos ')'  { $$ = novo_impressao($3); }
    ;

argumentos: calcula ',' argumentos   { $$ = nova_lista_nos($1, $3); }
          | calcula                  { $$ = nova_lista_nos($1, NULL); }
          ;

declaracao: TIPO VAR '=' calcula     { $$ = novo_declaracao($1, $2, $4); }
          | TIPO VAR                 { $$ = novo_declaracao($1, $2, NULL); }
          | TIPO VAR '[' calcula ']' { $$ = novo_declaracao_array($1, $2, $4); }
          ;

lvalue: VAR                        { $$ = novo_variavel($1); }
      | acesso_array               { $$ = $1; }
      ;

atribuicao: lvalue '=' calcula     { $$ = novo_atribuicao($1, $3); }
          ;

entrada: STDIN '(' MASCARA ',' VAR ')' { $$ = novo_entrada($3, $5); }
       ;

acesso_array: VAR '[' calcula ']'  { $$ = novo_acesso_array($1, $3); }
            ;

%%

#include "lex.yy.c"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <nome_arquivo.rag>\n", argv[0]);
        return 1;
    }

    char *arquivo = argv[1];
    char *extansao = strrchr(arquivo, '.');

    if (!extansao || strcmp(extansao, ".rag") != 0) {
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
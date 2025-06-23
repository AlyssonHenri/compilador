# Recursos da Linguagem

## Comentários

Comentários de linha única começam com duas barras invertidas `\\`  
Tudo até o final da linha será ignorado pelo interpretador

---

## Tipos de Dados

- **NUM**: valores numéricos (inteiros e ponto flutuante)
- **STR**: valores de texto (strings)

---

## Operações Aritméticas

O compilador suporta operações aritméticas em valores `NUM`:

- `+` (Adição)
- `-` (Subtração)
- `*` (Multiplicação)
- `/` (Divisão)

---

## Declaração de Variáveis

Variáveis devem ser declaradas antes de serem utilizadas  
Podem ser inicializadas durante a declaração

**Sintaxe:**
```plaintext
TIPO NOME
TIPO NOME = valor_inicial
```

**Exemplo:**
```plaintext
INT NUMERO
STR NOME = "Lucas"
```

---

## Declaração de Arrays

O compilador suporta arrays dos tipos `NUM` e `STR`  
Arrays devem ser declarados com tamanho fixo

**Sintaxe:**
```plaintext
TIPO NOME[tamanho]
```

**Exemplo:**
```plaintext
STR NOMES[20]
```
---

## Atribuição

Podem ser atribuido valores a variáveis e elementos de array usando `=`

**Sintaxe:**
```plaintext
VAR = expressao
VAR[indice] = expressao
```

**Exemplo:**
```plaintext
NUMERO = 2+2
NOMES[0] = "Maria"
```

---

## Impressão

O comando `PRINT` exibe texto e valores no console  
Argumentos são separados por vírgulas, com espaço entre eles  

**Sintaxe:**
```plaintext
PRINT(argumento1, argumento2, ...)
```

**Exemplo:**
```plaintext
PRINT(nomes[0], nomes[1], 2+2, variavel, 5)
```

---

## Leitura de Entrada

A instrução `STDIN` lê a entrada do usuário e armazena em uma variável  
As mascáras para entrada são `%i` para NUM e `%s` para SRT

**Sintaxe:**
```plaintext
STDIN(MASCARA, VAR)
```

**Exemplo:**
```plaintext
STDIN('%i', numero)
```

---

## Condicional

A instrução `IF` executa blocos de código com base em uma condição   
A instrução `ELSE` executa o bloco caso o if anterior seja falso  

As condições incluem:

- `<`  (Menor)
- `>`  (Maior)
- `<=` (Menor Igual)
- `>=` (Maior Igual)
- `==` (Igual)
- `!=` (Diferente)

**Sintaxe:**
```plaintext
IF (condicao) {
    // Bloco se verdadeiro
}
ELSE {
    // Bloco se falso
}
```

**Exemplo:**
```plaintext
IF (1 <= 2) {
    PRINT("CASO UM")
}
ELSE {
    PRINT("CASO DOIS")
}
```

---

## Laço de Repetição

A instrução `WHILE` repete um bloco de código enquanto a condição for verdadeira

**Sintaxe:**
```plaintext
WHILE (condicao) {
    // Bloco repetido
}
```
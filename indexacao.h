#ifndef INDEXACAO_H
#define INDEXACAO_H

#include "carros.h"

typedef struct NoCarroAno {
    Carro *carro;
    struct NoCarroAno *prox;
} NoCarroAno;

typedef struct NoAno {
    int ano;
    NoCarroAno *carros_do_ano;
    struct NoAno *prox;
} NoAno;


typedef struct NoArvore {
    int chave_int;      
    float chave_float;  
    Carro *carro;
    struct NoArvore *esq, *dir;
} NoArvore;

NoAno* criar_lista_de_ano(Carro *carros, int qtd);

NoArvore* inserir_km(NoArvore *raiz, int km, Carro *carro);
NoArvore* inserir_preco(NoArvore *raiz, float valor, Carro *carro);

void liberar_lista_ano(NoAno *lista);
void liberar_arvore(NoArvore *raiz);

#endif

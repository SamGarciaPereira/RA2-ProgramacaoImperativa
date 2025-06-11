#include <stdlib.h>
#include "indexacaonovo.h"

NoAno* criar_lista_de_ano(Carro *carros, int qtd) {
    NoAno *lista = NULL;

    for (int i = 0; i < qtd; i++) {
        int ano = carros[i].ano;
        NoAno *atual = lista, *anterior = NULL;

        while (atual && atual->ano != ano) {
            anterior = atual;
            atual = atual->prox;
        }

        if (!atual) {
            NoAno *novo_ano = malloc(sizeof(NoAno));
            novo_ano->ano = ano;
            novo_ano->carros_do_ano = NULL;
            novo_ano->prox = NULL;

            if (!anterior) lista = novo_ano;
            else anterior->prox = novo_ano;

            atual = novo_ano;
        }

        NoCarroAno *novo_carro = malloc(sizeof(NoCarroAno));
        novo_carro->carro = &carros[i];
        novo_carro->prox = atual->carros_do_ano;
        atual->carros_do_ano = novo_carro;
    }

    return lista;
}

NoArvore* inserir_km(NoArvore *raiz, int km, Carro *carro) {
    if (!raiz) {
        NoArvore *novo = malloc(sizeof(NoArvore));
        novo->chave_int = km;
        novo->carro = carro;
        novo->esq = novo->dir = NULL;
        return novo;
    }

    if (km < raiz->chave_int)
        raiz->esq = inserir_km(raiz->esq, km, carro);
    else
        raiz->dir = inserir_km(raiz->dir, km, carro);

    return raiz;
}

NoArvore* inserir_preco(NoArvore *raiz, float valor, Carro *carro) {
    if (!raiz) {
        NoArvore *novo = malloc(sizeof(NoArvore));
        novo->chave_float = valor;
        novo->carro = carro;
        novo->esq = novo->dir = NULL;
        return novo;
    }

    if (valor < raiz->chave_float)
        raiz->esq = inserir_preco(raiz->esq, valor, carro);
    else
        raiz->dir = inserir_preco(raiz->dir, valor, carro);

    return raiz;
}

void liberar_lista_ano(NoAno *lista) {
    while (lista) {
        NoCarroAno *c = lista->carros_do_ano;
        while (c) {
            NoCarroAno *tmp = c;
            c = c->prox;
            free(tmp);
        }
        NoAno *tmp = lista;
        lista = lista->prox;
        free(tmp);
    }
}

void liberar_arvore(NoArvore *raiz) {
    if (!raiz) return;
    liberar_arvore(raiz->esq);
    liberar_arvore(raiz->dir);
    free(raiz);
}

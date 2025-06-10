#ifndef CARROS_H
#define CARROS_H

typedef struct {
    char marca[20];
    char modelo[30];
    int ano;
    int km;
    float valor;
} Carro;

int ler_carros_csv(const char *nome_csv, Carro carros[], int max_carros);
int salvar_carros_binario(const char *nome_bin, Carro carros[], int quantidade);

Carro* ler_carros_binario(const char *nome_arquivo, int *quantidade);
void imprimir_cabecalho();
void imprimir_carro(const Carro *c);

#endif

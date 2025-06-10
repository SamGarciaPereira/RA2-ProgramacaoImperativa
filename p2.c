#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char marca[20];
    char modelo[30];
    int ano;
    int km;
    float valor;
} Carro;

Carro* ler_carros_binario(const char *nome_arquivo, int *quantidade) {
    FILE *f = fopen(nome_arquivo, "rb");
    if (!f) {
        perror("Erro ao abrir o arquivo binário");
        return NULL;
    }

    if (fread(quantidade, sizeof(int), 1, f) != 1) {
        perror("Erro ao ler a quantidade de registros");
        fclose(f);
        return NULL;
    }

    Carro *carros = malloc(sizeof(Carro) * (*quantidade));
    if (!carros) {
        perror("Erro ao alocar memória para os carros");
        fclose(f);
        return NULL;
    }

    if (fread(carros, sizeof(Carro), *quantidade, f) != *quantidade) {
        perror("Erro ao ler os dados dos carros");
        free(carros);
        fclose(f);
        return NULL;
    }

    fclose(f);
    return carros;
}

void imprimir_cabecalho() {
    printf("%-20s %-30s %-6s %-10s %-10s\n", "Marca", "Modelo", "Ano", "Kilometragem", "Preço (R$)");
    printf("-------------------------------------------------------------------------------------\n");
}

void imprimir_carro(const Carro *c) {
    printf("%-20s %-30s %-6d %-10d %10.2f\n", c->marca, c->modelo, c->ano, c->km, c->valor);
}

extern Carro* ler_carros_binario(const char *nome_arquivo, int *quantidade);

int main() {
    system("chcp 65001"); // para os acentos funcionarem

    int qtd;
    Carro *carros = ler_carros_binario("carros.bin", &qtd);
    if (!carros) return 1;

    imprimir_cabecalho();
    for (int i = 0; i < qtd; i++) {
        imprimir_carro(&carros[i]);
    }

    free(carros);
    return 0;
}

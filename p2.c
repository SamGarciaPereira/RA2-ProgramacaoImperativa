#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "carros.h"

// Remove a definição de 'Carro' daqui, já está no carros.h

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


void imprimir_menu() { // menu criado
    printf("\nMenu:\n");
    printf("1) Exibir todos os carros\n");
    printf("2) Buscar modelo por substring (ordenado por preço)\n");
    printf("3) Buscar por ano\n");
    printf("6) Sair\n");
    printf("Escolha uma opção: ");
}

int comparar_por_preco(const void *a, const void *b) {
    Carro *c1 = (Carro *)a;
    Carro *c2 = (Carro *)b;

    if (c1->valor < c2->valor) {
        return -1;  // c1 é mais barato
    } else if (c1->valor > c2->valor) {
        return 1;   // c1 é mais caro
    } else {
        return 0;   // iguais
    }
}

void buscar_por_substring(Carro *carros, int qtd) { // função que pega como parametro o vetor de carros e a quantidade de carros

    char substr[50];
    printf("Digite a substring do modelo: ");
    scanf(" %[^\n]", substr); // lê a substring (permite espaços)

    Carro *resultado = malloc(sizeof(Carro) * qtd); // aloca memória para o vetor de resultados
    if (resultado == NULL) { 
        printf("Erro ao alocar memória.\n");
        return;
    }

    int count = 0; // quantos carros foram encontrados

    for (int i = 0; i < qtd; i++) {
        if (strstr(carros[i].modelo, substr)) {
            resultado[count] = carros[i]; // guarda o carro encontrado
            count++; // aumenta o número de encontrados
        }
    }

    if (count == 0) {
        printf("Nenhum carro encontrado com \"%s\".\n", substr);
    } else {
        qsort(resultado, count, sizeof(Carro), comparar_por_preco);
        imprimir_cabecalho();
        for (int i = 0; i < count; i++) {
            imprimir_carro(&resultado[i]);
        }
    }

    free(resultado);
}

void lista_de_ano(Carro *carros, int qtd) { // função que pega como parametro o vetor de carros e a quantidade de carros
    int ano_limite;
    printf("Digite o ano mínimo: ");
    scanf("%d", &ano_limite);

    int encontrou = 0;
    imprimir_cabecalho();
    for (int i = 0; i < qtd; i++) {
        if (carros[i].ano >= ano_limite) {
            imprimir_carro(&carros[i]);
            encontrou = 1;
        }
    }

    if (!encontrou) {
        printf("Nenhum carro encontrado com ano >= %d.\n", ano_limite);
    }
}

int main(int argc, char *argv[]) {
    system("chcp 65001"); // para acentos funcionarem no Windows

    if (argc < 2) {
        printf("Uso: %s <arquivo_binario>\n", argv[0]);
        return 1;
    }

    int qtd;
    Carro *carros = ler_carros_binario(argv[1], &qtd);
    if (!carros) return 1;

    char opcao;
    do {
        imprimir_menu();
        scanf(" %c", &opcao);

        switch (tolower(opcao)) {
            case '1':
                imprimir_cabecalho();
                for (int i = 0; i < qtd; i++) {
                    imprimir_carro(&carros[i]);
                }
                break;
            case '2':
                buscar_por_substring(carros, qtd);
                break;
            case '3':
                lista_de_ano(carros, qtd);
                break;
            case '6':
                printf("Encerrando...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } 
    
    while (tolower(opcao) != '6');

    free(carros);
    return 0;
}

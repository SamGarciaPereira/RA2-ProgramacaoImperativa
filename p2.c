#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "carros.h"


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

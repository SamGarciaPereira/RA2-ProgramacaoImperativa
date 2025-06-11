#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "carros.h"

int main(int argc, char *argv[])
{
    system("chcp 65001"); // para acentos funcionarem no Windows

    if (argc < 2)
    {
        printf("Uso: %s <arquivo_binario>\n", argv[0]);
        return 1;
    }

    int qtd;
    Carro *carros = ler_carros_binario(argv[1], &qtd);
    NoArvore *arvore_km = NULL;

    if (!carros)
        return 1;
    for (int i = 0; i < qtd; i++) {
        arvore_km = inserir_km(arvore_km, carros[i].km, &carros[i]);
    }

    char opcao;
    do
    {
        imprimir_menu();
        scanf(" %c", &opcao);

        switch (tolower(opcao))
        {
        case '1':
            imprimir_cabecalho();
            for (int i = 0; i < qtd; i++)
            {
                imprimir_carro(&carros[i]);
            }
            break;
        case '2':
            buscar_por_substring(carros, qtd);
            break;
        case '3':
            lista_de_ano(carros, qtd);
            break;
        case '4':
            int min, max;
            printf("Digite a kilometragem mínima: ");
            scanf("%d", &min);
            printf("Digite a kilometragem máxima: ");
            scanf("%d", &max);

            printf("\nCarros com kilometragem entre %d e %d:\n\n", min, max);
            imprimir_cabecalho();
            exibir_km_intervalo(arvore_km, min, max);
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

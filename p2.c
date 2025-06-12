#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "carros.h"

// Função principal do programa
int main(int argc, char *argv[])
{
    system("chcp 65001"); // Configura o terminal para UTF-8 no Windows (acentuação)

    // Verifica se o nome do arquivo binário foi passado como argumento
    if (argc < 2)
    {
        printf("Uso: %s <arquivo_binario>\n", argv[0]);
        return 1;
    }

    int qtd; // Quantidade de carros lidos
    // Lê os carros do arquivo binário informado
    Carro *carros = ler_carros_binario(argv[1], &qtd);
    NoArvore *arvore_km = NULL;    // Árvore para busca por km
    NoArvore *arvore_preco = NULL; // Árvore para busca por preço

    if (!carros)
        return 1;

    // Insere cada carro nas árvores de km e preço
    for (int i = 0; i < qtd; i++)
    {
        arvore_km = inserir_km(arvore_km, carros[i].km, &carros[i]);
        arvore_preco = inserir_preco(arvore_preco, carros[i].valor, &carros[i]);
    }

    char opcao;
    // Loop principal do menu
    do
    {
        imprimir_menu(); // Exibe o menu de opções
        scanf(" %c", &opcao);

        switch (tolower(opcao))
        {
        case '1':
            // Imprime todos os carros
            imprimir_cabecalho();
            for (int i = 0; i < qtd; i++)
            {
                imprimir_carro(&carros[i]);
            }
            break;
        case '2':
            // Busca carros por substring no nome/modelo
            buscar_por_substring(carros, qtd);
            break;
        case '3':
            // Lista carros agrupados por ano
            lista_de_ano(carros, qtd);
            break;
        case '4':
        {
            // Exibe carros dentro de um intervalo de km
            int min_km, max_km;
            printf("Digite o valor mínimo de km: ");
            scanf("%d", &min_km);
            printf("Digite o valor máximo de km: ");
            scanf("%d", &max_km);
            exibir_km_intervalo(arvore_km, min_km, max_km);
            break;
        }
        case '5':
        {
            // Exibe carros dentro de um intervalo de preço
            int min_preco, max_preco;
            printf("Digite o valor mínimo: ");
            scanf("%d", &min_preco);
            printf("Digite o valor máximo: ");
            scanf("%d", &max_preco);
            exibir_preco_intervalo(arvore_preco, min_preco, max_preco);
            break;
        }
        case '6':
            // Encerra o programa
            printf("Encerrando...\n");
            break;
        default:
            // Opção inválida
            printf("Opção inválida.\n");
        }
    } while (tolower(opcao) != '6');

    free(carros); // Libera memória alocada
    return 0;
}

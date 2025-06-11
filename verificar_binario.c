#include <stdio.h>
#include <stdlib.h>
#include "carros.h"

int main() {
    // Abre o arquivo binário no modo de leitura
    FILE *bin = fopen("carros.bin", "rb");
    if (!bin) {
        perror("Erro ao abrir o arquivo carros.bin");
        return 1;
    }

    // Variável para armazenar a quantidade de carros lida do arquivo
    int quantidade_de_carros;

    if (fread(&quantidade_de_carros, sizeof(int), 1, bin) != 1) {
        fprintf(stderr, "Erro ao ler a quantidade de carros do arquivo.\n");
        fclose(bin);
        return 1;
    }

    printf("Arquivo binário contém %d registros.\n", quantidade_de_carros);

    Carro c; // Instância para armazenar os dados de cada carro

    // Cabeçalho para exibição dos dados no console
    printf("--------------------------------------------------------------------------------------\n");
    printf("%-20s %-30s %-10s %-10s %-10s\n", "Marca", "Modelo", "Ano", "KM", "Valor");
    printf("--------------------------------------------------------------------------------------\n");

    // ✅ PASSO 2: Ler exatamente a quantidade de carros que o arquivo diz ter
    for (int i = 0; i < quantidade_de_carros; i++) {
        // Lê um registro de Carro do arquivo
        if (fread(&c, sizeof(Carro), 1, bin)) {
            // Exibe os dados do carro formatados
            printf("%-20s %-30s %-10d %-10d R$%-10.2f\n", c.marca, c.modelo, c.ano, c.km, c.valor);
        } else {
            fprintf(stderr, "Erro: O arquivo terminou inesperadamente no registro %d.\n", i + 1);
            break; // Sai do loop se houver um erro de leitura
        }
    }

    // Fecha o arquivo binário
    fclose(bin);

    return 0;
}
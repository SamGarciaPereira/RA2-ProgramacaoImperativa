#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "carros.h"

#define MAX_CARROS 1000

// Função auxiliar para remover aspas do início e do fim da string
void remover_aspas_token(char *token) {
    if (token[0] == '"') token++;
    size_t len = strlen(token);
    if (len > 0 && token[len - 1] == '"') token[len - 1] = '\0';
}

int ler_carros_csv(const char *nome_csv, Carro carros[], int max_carros) {
    FILE *csv = fopen(nome_csv, "r");
    if (!csv) {
        perror("Erro ao abrir o arquivo CSV");
        return -1;
    }

    int count = 0;
    char linha[150];

    while (fgets(linha, sizeof(linha), csv) && count < max_carros) {
        linha[strcspn(linha, "\n")] = '\0';

        Carro c;
        char *token = strtok(linha, ",");
        if (token) {
            // remover aspas e copiar para marca
            if (token[0] == '"') token++; // avança o ponteiro se começar com aspas
            size_t len = strlen(token);
            if (len > 0 && token[len - 1] == '"') token[len - 1] = '\0'; // remove aspas do final
            strncpy(c.marca, token, sizeof(c.marca) - 1);
            c.marca[sizeof(c.marca) - 1] = '\0';
        } else continue;

        token = strtok(NULL, ",");
        if (token) {
            // remover aspas e copiar para modelo
            if (token[0] == '"') token++;
            size_t len = strlen(token);
            if (len > 0 && token[len - 1] == '"') token[len - 1] = '\0';
            strncpy(c.modelo, token, sizeof(c.modelo) - 1);
            c.modelo[sizeof(c.modelo) - 1] = '\0';
        } else continue;

        token = strtok(NULL, ",");
        if (token) c.ano = atoi(token);
        else continue;

        token = strtok(NULL, ",");
        if (token) c.km = atoi(token);
        else continue;

        token = strtok(NULL, ",");
        if (token) c.valor = atof(token);
        else continue;

        carros[count++] = c;
    }

    fclose(csv);
    return count;
}

int salvar_carros_binario(const char *nome_bin, Carro carros[], int quantidade) {
    FILE *bin = fopen(nome_bin, "wb");
    if (!bin) {
        perror("Erro ao criar o arquivo binário");
        return -1;
    }

    if (fwrite(&quantidade, sizeof(int), 1, bin) != 1) {
        perror("Erro ao escrever a quantidade no arquivo binário");
        fclose(bin);
        return -1;
    }

    if (fwrite(carros, sizeof(Carro), quantidade, bin) != (size_t)quantidade) {
        perror("Erro ao escrever os dados dos carros");
        fclose(bin);
        return -1;
    }

    fclose(bin);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <arquivo_entrada.csv> <arquivo_saida.bin>\n", argv[0]);
        return 1;
    }

    Carro carros[MAX_CARROS];
    int qtd = ler_carros_csv(argv[1], carros, MAX_CARROS);
    if (qtd < 0) return 1;

    if (salvar_carros_binario(argv[2], carros, qtd) != 0) return 1;

    printf("Arquivo %s gerado com sucesso com %d registros!\n", argv[2], qtd);
    return 0;
}

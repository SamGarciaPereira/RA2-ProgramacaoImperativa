#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char marca[20];
    char modelo[30];
    int ano;
    int km;
    float valor;
} Carro;

#define MAX_CARROS 1000

int main() {
    FILE *csv = fopen("carros.csv", "r");
    if (!csv) {
        perror("Erro ao abrir o arquivo carros.csv");
        return 1;
    }

    Carro carros[MAX_CARROS];
    int count = 0;
    char linha[150];

    while (fgets(linha, sizeof(linha), csv) && count < MAX_CARROS) {
        linha[strcspn(linha, "\n")] = '\0';

        Carro c;
        char *token = strtok(linha, ",");
        if (token) {
            // pra remover aspas de marca
            if (token[0] == '"') token++;
            size_t len = strlen(token);
            if (len > 0 && token[len - 1] == '"') token[len - 1] = '\0';
            strncpy(c.marca, token, sizeof(c.marca) - 1);
            c.marca[sizeof(c.marca) - 1] = '\0';
        } else continue;

        token = strtok(NULL, ",");
        if (token) {
            // pra remover aspas de modelo
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

    FILE *bin = fopen("carros.bin", "wb");
    if (!bin) {
        perror("Erro ao criar o arquivo carros.bin");
        return 1;
    }

    fwrite(&count, sizeof(int), 1, bin);
    fwrite(carros, sizeof(Carro), count, bin);
    fclose(bin);

    printf("Arquivo carros.bin gerado com sucesso com %d registros!\n", count);
    return 0;
}

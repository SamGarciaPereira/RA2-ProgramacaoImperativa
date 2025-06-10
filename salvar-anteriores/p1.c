#include <stdlib.h>
#include <string.h>

typedef struct {
    char marca[20];
    char modelo[30];
    int ano;
    int km;
    float valor;
} Carro;

int main() {
    FILE *csv = fopen("carros.csv", "r");
    if (!csv) {
        perror("Erro ao abrir o arquivo carros.csv");
        return 1;
    }

    FILE *bin = fopen("carros.bin", "wb");
    if (!bin) {
        perror("Erro ao criar o arquivo carros.bin");
        fclose(csv);
        return 1;
    }

    char linha[150];
    while (fgets(linha, sizeof(linha), csv)) {
        // Remover o caractere de nova linha, se existir
        linha[strcspn(linha, "\n")] = '\0';

        Carro c;
        char *token = strtok(linha, ",");
        if (token) strncpy(c.marca, token, sizeof(c.marca) - 1);
        else continue;

        token = strtok(NULL, ",");
        if (token) strncpy(c.modelo, token, sizeof(c.modelo) - 1);
        else continue;

        token = strtok(NULL, ",");
        if (token) c.ano = atoi(token);
        else continue;

        token = strtok(NULL, ",");
        if (token) c.km = atoi(token);
        else continue;

        token = strtok(NULL, ",");
        if (token) c.valor = atof(token);
        else continue;

        // Garantir que as strings terminem com '\0'
        c.marca[sizeof(c.marca) - 1] = '\0';
        c.modelo[sizeof(c.modelo) - 1] = '\0';

        // Escrever a estrutura no arquivo binário
        fwrite(&c, sizeof(Carro), 1, bin);
    }

    fclose(csv);
    fclose(bin);

    printf("Arquivo carros.bin gerado com sucesso!\n");
    return 0;
}
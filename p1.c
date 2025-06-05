#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura Carro, que contém os campos necessários para armazenar os dados de cada carro
typedef struct {
    char marca[20];   // Marca do carro (máximo de 20 caracteres)
    char modelo[30];  // Modelo do carro (máximo de 30 caracteres)
    int ano;          // Ano de fabricação do carro
    int km;           // Quilometragem do carro
    float valor;      // Valor do carro
} Carro;

int main() {
    // Abre o arquivo CSV no modo de leitura
    FILE *csv = fopen("carros.csv", "r");
    if (!csv) {
        perror("Erro ao abrir o arquivo carros.csv"); // Exibe erro caso o arquivo não seja encontrado
        return 1;
    }

    // Cria o arquivo binário no modo de escrita
    FILE *bin = fopen("carros.bin", "wb");
    if (!bin) {
        perror("Erro ao criar o arquivo carros.bin"); // Exibe erro caso o arquivo binário não possa ser criado
        fclose(csv);
        return 1;
    }

    char linha[150]; // Buffer para armazenar cada linha do arquivo CSV
    while (fgets(linha, sizeof(linha), csv)) { // Lê cada linha do arquivo CSV
        linha[strcspn(linha, "\n")] = '\0'; // Remove o caractere de nova linha, se existir

        Carro c; // Instância da estrutura Carro para armazenar os dados da linha atual

        // Divide a linha do CSV em tokens separados por vírgulas
        char *token = strtok(linha, ",");
        if (token) strncpy(c.marca, token, sizeof(c.marca) - 1); // Copia a marca
        else continue;

        token = strtok(NULL, ",");
        if (token) strncpy(c.modelo, token, sizeof(c.modelo) - 1); // Copia o modelo
        else continue;

        token = strtok(NULL, ",");
        if (token) c.ano = atoi(token); // Converte o ano para inteiro
        else continue;

        token = strtok(NULL, ",");
        if (token) c.km = atoi(token); // Converte a quilometragem para inteiro
        else continue;

        token = strtok(NULL, ",");
        if (token) c.valor = atof(token); // Converte o valor para float
        else continue;

        // Garante que as strings terminem com '\0'
        c.marca[sizeof(c.marca) - 1] = '\0';
        c.modelo[sizeof(c.modelo) - 1] = '\0';

        // Escreve a estrutura Carro no arquivo binário
        fwrite(&c, sizeof(Carro), 1, bin);
    }

    // Fecha os arquivos
    fclose(csv);
    fclose(bin);

    printf("Arquivo carros.bin gerado com sucesso!\n"); // Mensagem de sucesso
    return 0;
}
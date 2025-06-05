#include <stdio.h>
#include <stdlib.h>

// Definição da estrutura Carro, que corresponde ao formato do arquivo binário
typedef struct {
    char marca[20];   // Marca do carro (máximo de 20 caracteres)
    char modelo[30];  // Modelo do carro (máximo de 30 caracteres)
    int ano;          // Ano de fabricação do carro
    int km;           // Quilometragem do carro
    float valor;      // Valor do carro
} Carro;

int main() {
    // Abre o arquivo binário no modo de leitura
    FILE *bin = fopen("carros.bin", "rb");
    if (!bin) {
        perror("Erro ao abrir o arquivo carros.bin"); // Exibe erro caso o arquivo não seja encontrado
        return 1;
    }

    Carro c; // Instância da estrutura Carro para armazenar os dados lidos do arquivo binário

    // Cabeçalho para exibição dos dados no console
    printf("Dados do arquivo carros.bin:\n");
    printf("--------------------------------------------------------------------------------------\n");
    printf("%-20s %-30s %-10s %-10s %-10s\n", "Marca", "Modelo", "Ano", "KM", "Valor");
    printf("--------------------------------------------------------------------------------------\n");

    // Lê os registros do arquivo binário e exibe no console
    while (fread(&c, sizeof(Carro), 1, bin)) {
        // Exibe os dados do carro formatados
        printf("%-20s %-30s %-10d %-10d R$%-10.2f\n", c.marca, c.modelo, c.ano, c.km, c.valor);
    }

    // Fecha o arquivo binário
    fclose(bin);

    return 0;
}
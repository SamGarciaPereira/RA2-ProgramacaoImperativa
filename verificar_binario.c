#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char marca[20];
    char modelo[30];
    int ano;
    int km;
    float valor;
} Carro;

int main() {
    FILE *bin = fopen("carros.bin", "rb");
    if (!bin) {
        perror("Erro ao abrir o arquivo carros.bin");
        return 1;
    }

    Carro c;
    printf("Dados do arquivo carros.bin:\n");
    printf("--------------------------------------------------------------------------------------\n");
    printf("%-20s %-30s %-10s %-10s %-10s\n", "Marca", "Modelo", "Ano", "KM", "Valor");
    printf("--------------------------------------------------------------------------------------\n");

    while (fread(&c, sizeof(Carro), 1, bin)) {
        printf("%-20s %-30s %-10d %-10d R$%-10.2f\n", c.marca, c.modelo, c.ano, c.km, c.valor);
    }

    fclose(bin);
    return 0;
}
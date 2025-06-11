#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "carros.h"

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

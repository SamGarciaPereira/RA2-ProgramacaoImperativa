#ifndef CARROS_H
#define CARROS_H

typedef struct {
    char marca[20];
    char modelo[30];
    int ano;
    int km;
    float valor;
} Carro;

typedef struct NoCarroAno {
    Carro *carro;
    struct NoCarroAno *prox;
} NoCarroAno;

typedef struct NoAno {
    int ano;
    NoCarroAno *carros_do_ano;
    struct NoAno *prox;
} NoAno;


typedef struct NoArvore {
    int chave_int;      
    float chave_float;  
    Carro *carro;
    struct NoArvore *esq, *dir;
} NoArvore;


int ler_carros_csv(const char *nome_csv, Carro carros[], int max_carros);
int salvar_carros_binario(const char *nome_bin, Carro carros[], int quantidade);

Carro* ler_carros_binario(const char *nome_arquivo, int *quantidade);
void imprimir_cabecalho();
void imprimir_carro(const Carro *c);

/*FUNÇÕES P1*/

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

/*FUNÇÕES P2*/

Carro* ler_carros_binario(const char *nome_arquivo, int *quantidade) {
    FILE *f = fopen(nome_arquivo, "rb");
    if (!f) {
        perror("Erro ao abrir o arquivo binário");
        return NULL;
    }

    if (fread(quantidade, sizeof(int), 1, f) != 1) {
        perror("Erro ao ler a quantidade de registros");
        fclose(f);
        return NULL;
    }

    Carro *carros = malloc(sizeof(Carro) * (*quantidade));
    if (!carros) {
        perror("Erro ao alocar memória para os carros");
        fclose(f);
        return NULL;
    }

    if (fread(carros, sizeof(Carro), *quantidade, f) != *quantidade) {
        perror("Erro ao ler os dados dos carros");
        free(carros);
        fclose(f);
        return NULL;
    }

    fclose(f);
    return carros;
}

void imprimir_cabecalho() {
    printf("%-20s %-30s %-6s %-10s %-10s\n", "Marca", "Modelo", "Ano", "Kilometragem", "Preço (R$)");
    printf("-------------------------------------------------------------------------------------\n");
}

void imprimir_carro(const Carro *c) {
    printf("%-20s %-30s %-6d %-10d %10.2f\n", c->marca, c->modelo, c->ano, c->km, c->valor);
}


void imprimir_menu() { // menu criado
    printf("\nMenu:\n");
    printf("1) Exibir todos os carros\n");
    printf("2) Buscar modelo por substring (ordenado por preço)\n");
    printf("3) Buscar por ano\n");
    printf("6) Sair\n");
    printf("Escolha uma opção: ");
}

int comparar_por_preco(const void *a, const void *b) {
    Carro *c1 = (Carro *)a;
    Carro *c2 = (Carro *)b;

    if (c1->valor < c2->valor) {
        return -1;  // c1 é mais barato
    } else if (c1->valor > c2->valor) {
        return 1;   // c1 é mais caro
    } else {
        return 0;   // iguais
    }
}

void buscar_por_substring(Carro *carros, int qtd) { // função que pega como parametro o vetor de carros e a quantidade de carros

    char substr[50];
    printf("Digite a substring do modelo: ");
    scanf(" %[^\n]", substr); // lê a substring (permite espaços)

    Carro *resultado = malloc(sizeof(Carro) * qtd); // aloca memória para o vetor de resultados
    if (resultado == NULL) { 
        printf("Erro ao alocar memória.\n");
        return;
    }

    int count = 0; // quantos carros foram encontrados

    for (int i = 0; i < qtd; i++) {
        if (strstr(carros[i].modelo, substr)) {
            resultado[count] = carros[i]; // guarda o carro encontrado
            count++; // aumenta o número de encontrados
        }
    }

    if (count == 0) {
        printf("Nenhum carro encontrado com \"%s\".\n", substr);
    } else {
        qsort(resultado, count, sizeof(Carro), comparar_por_preco);
        imprimir_cabecalho();
        for (int i = 0; i < count; i++) {
            imprimir_carro(&resultado[i]);
        }
    }

    free(resultado);
}

void lista_de_ano(Carro *carros, int qtd) { // função que pega como parametro o vetor de carros e a quantidade de carros
    int ano_limite;
    printf("Digite o ano mínimo: ");
    scanf("%d", &ano_limite);

    int encontrou = 0;
    imprimir_cabecalho();
    for (int i = 0; i < qtd; i++) {
        if (carros[i].ano >= ano_limite) {
            imprimir_carro(&carros[i]);
            encontrou = 1;
        }
    }

    if (!encontrou) {
        printf("Nenhum carro encontrado com ano >= %d.\n", ano_limite);
    }
}

NoAno* criar_lista_de_ano(Carro *carros, int qtd) {
    NoAno *lista = NULL;

    for (int i = 0; i < qtd; i++) {
        int ano = carros[i].ano;
        NoAno *atual = lista, *anterior = NULL;

        while (atual && atual->ano != ano) {
            anterior = atual;
            atual = atual->prox;
        }

        if (!atual) {
            NoAno *novo_ano = malloc(sizeof(NoAno));
            novo_ano->ano = ano;
            novo_ano->carros_do_ano = NULL;
            novo_ano->prox = NULL;

            if (!anterior) lista = novo_ano;
            else anterior->prox = novo_ano;

            atual = novo_ano;
        }

        NoCarroAno *novo_carro = malloc(sizeof(NoCarroAno));
        novo_carro->carro = &carros[i];
        novo_carro->prox = atual->carros_do_ano;
        atual->carros_do_ano = novo_carro;
    }

    return lista;
}

NoArvore* inserir_km(NoArvore *raiz, int km, Carro *carro) {
    if (!raiz) {
        NoArvore *novo = malloc(sizeof(NoArvore));
        novo->chave_int = km;
        novo->carro = carro;
        novo->esq = novo->dir = NULL;
        return novo;
    }

    if (km < raiz->chave_int)
        raiz->esq = inserir_km(raiz->esq, km, carro);
    else
        raiz->dir = inserir_km(raiz->dir, km, carro);

    return raiz;
}

NoArvore* inserir_preco(NoArvore *raiz, float valor, Carro *carro) {
    if (!raiz) {
        NoArvore *novo = malloc(sizeof(NoArvore));
        novo->chave_float = valor;
        novo->carro = carro;
        novo->esq = novo->dir = NULL;
        return novo;
    }

    if (valor < raiz->chave_float)
        raiz->esq = inserir_preco(raiz->esq, valor, carro);
    else
        raiz->dir = inserir_preco(raiz->dir, valor, carro);

    return raiz;
}

void liberar_lista_ano(NoAno *lista) {
    while (lista) {
        NoCarroAno *c = lista->carros_do_ano;
        while (c) {
            NoCarroAno *tmp = c;
            c = c->prox;
            free(tmp);
        }
        NoAno *tmp = lista;
        lista = lista->prox;
        free(tmp);
    }
}

void liberar_arvore(NoArvore *raiz) {
    if (!raiz) return;
    liberar_arvore(raiz->esq);
    liberar_arvore(raiz->dir);
    free(raiz);
}


#endif

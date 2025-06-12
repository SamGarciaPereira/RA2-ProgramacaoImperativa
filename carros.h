#ifndef CARROS_H
#define CARROS_H

/*
 * Definição da estrutura Carro, que representa um veículo com marca, modelo, ano, km e valor.
 */
typedef struct
{
    char marca[20];  // Marca do carro
    char modelo[30]; // Modelo do carro
    int ano;         // Ano de fabricação
    int km;          // Quilometragem
    float valor;     // Preço do carro
} Carro;

/*
 * Estruturas para listas encadeadas de carros agrupados por ano.
 */
typedef struct NoCarroAno
{
    Carro *carro;            // Ponteiro para o carro
    struct NoCarroAno *prox; // Próximo nó da lista de carros do mesmo ano
} NoCarroAno;

typedef struct NoAno
{
    int ano;                   // Ano de referência
    NoCarroAno *carros_do_ano; // Lista de carros desse ano
    struct NoAno *prox;        // Próximo nó da lista de anos
} NoAno;

/*
 * Estrutura para árvore binária de busca, usada para indexar carros por km ou valor.
 */
typedef struct NoArvore
{
    int chave_int;              // Chave inteira (ex: km)
    float chave_float;          // Chave float (ex: valor)
    Carro *carro;               // Ponteiro para o carro
    struct NoArvore *esq, *dir; // Filhos esquerdo e direito
} NoArvore;

/*
 * Lê carros de um arquivo CSV e armazena no vetor carros.
 * Retorna a quantidade lida ou -1 em caso de erro.
 */
int ler_carros_csv(const char *nome_csv, Carro carros[], int max_carros);

/*
 * Salva o vetor de carros em um arquivo binário.
 * Retorna 0 em sucesso ou -1 em erro.
 */
int salvar_carros_binario(const char *nome_bin, Carro carros[], int quantidade);

/*
 * Lê carros de um arquivo binário, aloca dinamicamente e retorna ponteiro para o vetor.
 * Armazena a quantidade lida em *quantidade.
 */
Carro *ler_carros_binario(const char *nome_arquivo, int *quantidade);

/*
 * Imprime o cabeçalho da tabela de carros.
 */
void imprimir_cabecalho();

/*
 * Imprime os dados de um carro formatados.
 */
void imprimir_carro(const Carro *c);

/* --- FUNÇÕES P1 --- */

#define MAX_CARROS 1000 // Quantidade máxima de carros suportada

/*
 * Remove aspas do início e fim de um token de string.
 */
void remover_aspas_token(char *token)
{
    if (token[0] == '"')
        token++;
    size_t len = strlen(token);
    if (len > 0 && token[len - 1] == '"')
        token[len - 1] = '\0';
}

/*
 * Lê carros de um arquivo CSV, tratando aspas e separadores.
 * Preenche o vetor carros e retorna a quantidade lida.
 */
int ler_carros_csv(const char *nome_csv, Carro carros[], int max_carros)
{
    FILE *csv = fopen(nome_csv, "r");
    if (!csv)
    {
        perror("Erro ao abrir o arquivo CSV");
        return -1;
    }

    int count = 0;
    char linha[150];

    while (fgets(linha, sizeof(linha), csv) && count < max_carros)
    {
        linha[strcspn(linha, "\n")] = '\0';

        Carro c;
        char *token = strtok(linha, ",");
        if (token)
        {
            // Remover aspas e copiar para marca
            if (token[0] == '"')
                token++;
            size_t len = strlen(token);
            if (len > 0 && token[len - 1] == '"')
                token[len - 1] = '\0';
            strncpy(c.marca, token, sizeof(c.marca) - 1);
            c.marca[sizeof(c.marca) - 1] = '\0';
        }
        else
            continue;

        token = strtok(NULL, ",");
        if (token)
        {
            // Remover aspas e copiar para modelo
            if (token[0] == '"')
                token++;
            size_t len = strlen(token);
            if (len > 0 && token[len - 1] == '"')
                token[len - 1] = '\0';
            strncpy(c.modelo, token, sizeof(c.modelo) - 1);
            c.modelo[sizeof(c.modelo) - 1] = '\0';
        }
        else
            continue;

        token = strtok(NULL, ",");
        if (token)
            c.ano = atoi(token);
        else
            continue;

        token = strtok(NULL, ",");
        if (token)
            c.km = atoi(token);
        else
            continue;

        token = strtok(NULL, ",");
        if (token)
            c.valor = atof(token);
        else
            continue;

        carros[count++] = c;
    }

    fclose(csv);
    return count;
}

/*
 * Salva o vetor de carros em um arquivo binário.
 * Escreve primeiro a quantidade, depois os dados dos carros.
 */
int salvar_carros_binario(const char *nome_bin, Carro carros[], int quantidade)
{
    FILE *bin = fopen(nome_bin, "wb");
    if (!bin)
    {
        perror("Erro ao criar o arquivo binário");
        return -1;
    }

    if (fwrite(&quantidade, sizeof(int), 1, bin) != 1)
    {
        perror("Erro ao escrever a quantidade no arquivo binário");
        fclose(bin);
        return -1;
    }

    if (fwrite(carros, sizeof(Carro), quantidade, bin) != (size_t)quantidade)
    {
        perror("Erro ao escrever os dados dos carros");
        fclose(bin);
        return -1;
    }

    fclose(bin);
    return 0;
}

/* --- FUNÇÕES P2 --- */

/*
 * Lê carros de um arquivo binário, alocando dinamicamente o vetor.
 * Retorna ponteiro para o vetor ou NULL em erro.
 */
Carro *ler_carros_binario(const char *nome_arquivo, int *quantidade)
{
    FILE *f = fopen(nome_arquivo, "rb");
    if (!f)
    {
        perror("Erro ao abrir o arquivo binário");
        return NULL;
    }

    if (fread(quantidade, sizeof(int), 1, f) != 1)
    {
        perror("Erro ao ler a quantidade de registros");
        fclose(f);
        return NULL;
    }

    Carro *carros = malloc(sizeof(Carro) * (*quantidade));
    if (!carros)
    {
        perror("Erro ao alocar memória para os carros");
        fclose(f);
        return NULL;
    }

    if (fread(carros, sizeof(Carro), *quantidade, f) != *quantidade)
    {
        perror("Erro ao ler os dados dos carros");
        free(carros);
        fclose(f);
        return NULL;
    }

    fclose(f);
    return carros;
}

/*
 * Imprime o cabeçalho da tabela de carros.
 */
void imprimir_cabecalho()
{
    printf("%-20s %-30s %-6s %-10s %-10s\n", "Marca", "Modelo", "Ano", "Kilometragem", "Preço (R$)");
    printf("-------------------------------------------------------------------------------------\n");
}

/*
 * Imprime um carro formatado.
 */
void imprimir_carro(const Carro *c)
{
    printf("%-20s %-30s %-6d %-10d %10.2f\n", c->marca, c->modelo, c->ano, c->km, c->valor);
}

/*
 * Exibe o menu principal do programa.
 */
void imprimir_menu()
{
    printf("\nMenu:\n");
    printf("1) Exibir todos os carros\n");
    printf("2) Buscar modelo por substring (ordenado por preço)\n");
    printf("3) Buscar por ano\n");
    printf("4) Buscar por kilometragem\n");
    printf("5) Buscar por preço\n");
    printf("6) Sair\n");
    printf("Escolha uma opção: ");
}

/*
 * Função de comparação para qsort, ordena carros por preço crescente.
 */
int comparar_por_preco(const void *a, const void *b)
{
    Carro *c1 = (Carro *)a;
    Carro *c2 = (Carro *)b;

    if (c1->valor < c2->valor)
        return -1;
    else if (c1->valor > c2->valor)
        return 1;
    else
        return 0;
}

/*
 * Busca carros cujo modelo contém uma substring (case-insensitive), ordena por preço e exibe.
 */
void buscar_por_substring(Carro *carros, int qtd)
{
    char substr[50];
    printf("Digite a substring do modelo: ");
    scanf(" %[^\n]", substr);

    // Converter substring para minúsculo
    for (int i = 0; substr[i]; i++)
        substr[i] = tolower(substr[i]);

    Carro *resultado = malloc(sizeof(Carro) * qtd);
    if (resultado == NULL)
    {
        printf("Erro ao alocar memória.\n");
        return;
    }

    int count = 0;
    for (int i = 0; i < qtd; i++)
    {
        char modelo_lower[30];
        strncpy(modelo_lower, carros[i].modelo, sizeof(modelo_lower) - 1);
        modelo_lower[sizeof(modelo_lower) - 1] = '\0';
        // Converter modelo para minúsculo
        for (int j = 0; modelo_lower[j]; j++)
            modelo_lower[j] = tolower(modelo_lower[j]);

        if (strstr(modelo_lower, substr))
        {
            resultado[count] = carros[i];
            count++;
        }
    }

    if (count == 0)
    {
        printf("Nenhum carro encontrado com \"%s\".\n", substr);
    }
    else
    {
        qsort(resultado, count, sizeof(Carro), comparar_por_preco);
        imprimir_cabecalho();
        for (int i = 0; i < count; i++)
        {
            imprimir_carro(&resultado[i]);
        }
    }

    free(resultado);
}

/*
 * Lista carros com ano maior ou igual ao informado.
 */
void lista_de_ano(Carro *carros, int qtd)
{
    int ano_limite;
    printf("Digite o ano mínimo: ");
    scanf("%d", &ano_limite);

    int encontrou = 0;
    imprimir_cabecalho();
    for (int i = 0; i < qtd; i++)
    {
        if (carros[i].ano >= ano_limite)
        {
            imprimir_carro(&carros[i]);
            encontrou = 1;
        }
    }

    if (!encontrou)
    {
        printf("Nenhum carro encontrado com ano >= %d.\n", ano_limite);
    }
}

/*
 * Cria uma lista encadeada de anos, cada um com sua lista de carros.
 * Retorna ponteiro para o início da lista.
 */
NoAno *criar_lista_de_ano(Carro *carros, int qtd)
{
    NoAno *lista = NULL;

    for (int i = 0; i < qtd; i++)
    {
        int ano = carros[i].ano;
        NoAno *atual = lista, *anterior = NULL;

        // Busca se já existe o ano na lista
        while (atual && atual->ano != ano)
        {
            anterior = atual;
            atual = atual->prox;
        }

        // Se não existe, cria novo nó de ano
        if (!atual)
        {
            NoAno *novo_ano = malloc(sizeof(NoAno));
            novo_ano->ano = ano;
            novo_ano->carros_do_ano = NULL;
            novo_ano->prox = NULL;

            if (!anterior)
                lista = novo_ano;
            else
                anterior->prox = novo_ano;

            atual = novo_ano;
        }

        // Insere o carro na lista de carros do ano
        NoCarroAno *novo_carro = malloc(sizeof(NoCarroAno));
        novo_carro->carro = &carros[i];
        novo_carro->prox = atual->carros_do_ano;
        atual->carros_do_ano = novo_carro;
    }

    return lista;
}

/*
 * Insere um carro em uma árvore binária de busca por km.
 * Retorna a nova raiz da árvore.
 */
NoArvore *inserir_km(NoArvore *raiz, int km, Carro *carro)
{
    if (!raiz)
    {
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

/*
 * Exibe carros na árvore de km cujo valor está no intervalo [min_km, max_km].
 */
void exibir_km_intervalo(NoArvore *raiz, int min_km, int max_km)
{
    if (!raiz)
        return;

    exibir_km_intervalo(raiz->esq, min_km, max_km);

    if (raiz->chave_int >= min_km && raiz->chave_int <= max_km)
    {
        imprimir_carro(raiz->carro);
    }

    exibir_km_intervalo(raiz->dir, min_km, max_km);
}

/*
 * Insere um carro em uma árvore binária de busca por valor.
 * Retorna a nova raiz da árvore.
 */
NoArvore *inserir_preco(NoArvore *raiz, float valor, Carro *carro)
{
    if (!raiz)
    {
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

/*
 * Exibe carros na árvore de valor cujo preço está no intervalo [min_preco, max_preco].
 */
void exibir_preco_intervalo(NoArvore *raiz, int min_preco, int max_preco)
{
    if (!raiz)
        return;

    exibir_preco_intervalo(raiz->esq, min_preco, max_preco);

    if (raiz->carro && raiz->carro->valor >= min_preco && raiz->carro->valor <= max_preco)
    {
        imprimir_carro(raiz->carro);
    }

    exibir_preco_intervalo(raiz->dir, min_preco, max_preco);
}

/*
 * Libera toda a memória alocada para a lista de anos e suas listas de carros.
 */
void liberar_lista_ano(NoAno *lista)
{
    if (!lista)
        return;

    while (lista)
    {
        NoCarroAno *c = lista->carros_do_ano;
        while (c)
        {
            NoCarroAno *tmp = c;
            c = c->prox;
            free(tmp);
        }
        NoAno *tmp = lista;
        lista = lista->prox;
        free(tmp);
    }
}

/*
 * Libera toda a memória alocada para a árvore binária.
 */
void liberar_arvore(NoArvore *raiz)
{
    if (raiz == NULL)
        return;

    liberar_arvore(raiz->esq);
    liberar_arvore(raiz->dir);
    free(raiz);
}

#endif // CARROS_H

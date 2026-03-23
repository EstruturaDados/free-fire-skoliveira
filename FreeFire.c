#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Código da Ilha – Edição Free Fire
// Nível: Mestre
// Este programa simula o gerenciamento avançado de uma mochila com componentes coletados durante a fuga de uma ilha.
// Ele introduz ordenação com critérios e busca binária para otimizar a gestão dos recursos.

// Constantes
#define MAX_ITENS 10
#define MAX_STR_LEN_NOME 30
#define MAX_STR_LEN_TIPO 20

// --- Estruturas ----
// Struct Item:
// Representa um componente com nome, tipo, quantidade e prioridade (1 a 5).
// A prioridade indica a importância do item na montagem do plano de fuga.
typedef struct {
    char nome[MAX_STR_LEN_NOME];
    char tipo[MAX_STR_LEN_TIPO];
    int quantidade;
    int prioridade;
} Item;

// Enum CriterioOrdenacao:
// Define os critérios possíveis para a ordenação dos itens (nome, tipo ou prioridade).
typedef enum {
    NAO_ORDENADO,
    ORDENADO_POR_NOME,
    ORDENADO_POR_TIPO,
    ORDENADO_POR_PRIORIDADE
} CriterioOrdenacao;

// Vetor mochila:
// Armazena até 10 itens coletados.
// Variáveis de controle: numItens (quantidade atual), comparacoes (análise de desempenho), ordenadaPorNome (para controle da busca binária).
typedef struct  {
    Item itens[MAX_ITENS];  // lista estática de itens (tamanho fixo)
    int quantidade;         // quantidade atual de itens na mochila
    CriterioOrdenacao criterioAtual; // Status da ordenação da mochila
} Mochila;

// --- Prototipagem das funções ----
void limparTela();
void exibirMenu(Mochila *mochila);
void inserirItem(Mochila *mochila);
void removerItem(Mochila *mochila);
void menuDeOrdenacao();
void bubbleSortNome(Mochila *mochila);
void insertionSortTipo(Mochila *mochila);
void selectionSortPrioridade(Mochila *mochila);
void buscaBinariaPorNome(Mochila *mochila);
void listarItens(Mochila *mochila);
void limparBufferEntrada();

int main() {
    // Menu principal com opções:
    // 1. Adicionar um item
    // 2. Remover um item
    // 3. Listar todos os itens
    // 4. Ordenar os itens por critério (nome, tipo, prioridade)
    // 5. Realizar busca binária por nome
    // 0. Sair

    // A estrutura switch trata cada opção chamando a função correspondente.
    // A ordenação e busca binária exigem que os dados estejam bem organizados.

    Mochila mochila;
    mochila.quantidade = 0; // inicializa a mochila
    mochila.criterioAtual = NAO_ORDENADO;   // inicializa como não ordenado

    int opçao;
    do {
        exibirMenu(&mochila);
        printf("\nEscolha uma opcao: ");
        scanf("%d", &opçao);
        limparBufferEntrada();

        switch (opçao)
        {
        case 1:
            inserirItem(&mochila);
            listarItens(&mochila);
            break;
        case 2:
            removerItem(&mochila);
            listarItens(&mochila);
            break;
        case 3:
            listarItens(&mochila);
            break;
        case 4:
            int criterio;
            limparTela();
            menuDeOrdenacao();
            printf("\nEscolha o criterio: ");
            scanf("%d", &criterio);
            limparBufferEntrada();

            switch (criterio)
            {
            case 1:
                bubbleSortNome(&mochila);
                listarItens(&mochila);
                break;
            case 2:
                insertionSortTipo(&mochila);
                listarItens(&mochila);
                break;
            case 3:
                selectionSortPrioridade(&mochila);
                listarItens(&mochila);
                break;
            case 0:
                break;
            default:
                printf("\nDigite um indice válido!");
                printf("\nPressione Enter para continuar...");
                getchar();
                break;
            }
            break;
        case 5:
            buscaBinariaPorNome(&mochila);
            break;
        case 0:
            return 0;

        default:
            printf("\nDigite um indice válido!");
            printf("\nPressione Enter para continuar...");
            getchar();
            break;
        }

    } while (1);
    

    return 0;
}

// limparTela():
// Simula a limpeza da tela imprimindo várias linhas em branco.
void limparTela() {
    for (int i = 0; i < 10; i++) { printf("\n"); }  
}

// exibirMenu():
// Apresenta o menu principal ao jogador, com destaque para status da ordenação.
void exibirMenu(Mochila *mochila) {
    printf("\n==============================================================");
    printf("\n   MOCHILA DE SOBREVIVENCIA - CODIGO DA ILHA (NIVEL MESTRE)");
    printf("\n==============================================================");
    printf("\nItens na Mochila: %d/%d", mochila->quantidade, MAX_ITENS);
    CriterioOrdenacao criterio = mochila->criterioAtual;
    printf("\nStatus da Ordenacao por Nome: %s\n", (criterio == ORDENADO_POR_NOME) ? "ORDENADO" : "NAO ORDENADO");
    printf("\n1. Adicionar Componente");
    printf("\n2. Descartar Componente");
    printf("\n3. Listar Componentes (Inventario)");
    printf("\n4. Organizar Mochila (Ordenar Componentes)");
    printf("\n5. Buscar Binaria por Componente-Chave (por nome)");
    printf("\n0. ATIVAR TORRE DE FULGA (Sair)");
    printf("\n--------------------------------------------------------------");
}

// inserirItem():
// Adiciona um novo componente à mochila se houver espaço.
// Solicita nome, tipo, quantidade e prioridade.
// Após inserir, marca a mochila como "não ordenada por nome".
void inserirItem(Mochila *mochila) {
    // Verifica se a mochila está cheia
    if (mochila->quantidade == MAX_ITENS) {
        printf("\nMochila Lotada! Descarte primeiro um item para adicinar...\n");
        return;
    }

    // Insere novo item no fim do vetor
    Item *novo = &mochila->itens[mochila->quantidade];

    printf("\n--- Coletando Novo Componente ---");

    printf("\nNome: ");
    fgets(novo->nome, MAX_STR_LEN_NOME, stdin);
    novo->nome[strcspn(novo->nome, "\n")] = '\0';

    printf("Tipo (Estrutural, Eletronico, Energia): ");
    fgets(novo->tipo, MAX_STR_LEN_TIPO, stdin);
    novo->tipo[strcspn(novo->tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &novo->quantidade);
    limparBufferEntrada();

    printf("Prioridade de Montagem (1-5): ");
    scanf("%d", &novo->prioridade);
    limparBufferEntrada();

    mochila->quantidade++;

    printf("\n\nComponente '%s' adicionado!\n", novo->nome);
    mochila->criterioAtual = NAO_ORDENADO; // status como não ordenado
}

// removerItem():
// Permite remover um componente da mochila pelo nome.
// Se encontrado, reorganiza o vetor para preencher a lacuna.
void removerItem(Mochila *mochila) {
    int indice = -1;

    char nome[MAX_STR_LEN_NOME];

    printf("\nNome do item: ");
    fgets(nome, MAX_STR_LEN_NOME, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    // Busca o índice do item com o nome fornecido
    for (int i = 0; i < mochila->quantidade; i++) {
        if (strcmp(mochila->itens[i].nome, nome) == 0) {
            indice = i;
            break;
        }
    }

    // Se não encontrou, encerra
    if (indice == -1) {
        printf("\n\nItem '%s' não encontrado na mochila.\n", nome);
        return;
    }

    // Desloca os elementos à direita uma posição para a esquerda
    for (int i = indice; i < mochila->quantidade - 1; i++) {
        mochila->itens[i] = mochila->itens[i + 1];
    }

    mochila->quantidade--;
    printf("\n\nItem '%s' removido com sucesso.\n", nome);
}

// listarItens():
// Exibe uma tabela formatada com todos os componentes presentes na mochila.
void listarItens(Mochila *mochila) {
    if (mochila->quantidade == 0) {
        printf("\nSua mochila está vazia!\n");
        printf("\nPressione Enter para continuar...");
        getchar();
        return;
    }

    printf("\n--- ITENS NA MOCHILA (%d/%d) ---", mochila->quantidade, MAX_ITENS);
    printf("\n----------------------------------------------------------");
    printf("\nNOME\t\t| TIPO\t\t| QUANTIDADE | PRIORIDADE");
    printf("\n----------------------------------------------------------");
    for (int i = 0; i < mochila->quantidade; i++) {
        printf("\n%-15s | %-13s | %-10d | %d", 
               mochila->itens[i].nome,
               mochila->itens[i].tipo,
               mochila->itens[i].quantidade,
               mochila->itens[i].prioridade);
    }
    printf("\n----------------------------------------------------------\n");

    printf("\nPressione Enter para continuar...");
    getchar();
}

// menuDeOrdenacao():
// Permite ao jogador escolher como deseja ordenar os itens.
// Utiliza a função insertionSort() com o critério selecionado.
// Exibe a quantidade de comparações feitas (análise de desempenho).
void menuDeOrdenacao() {
    printf("\n--- Estrategia de Organizacao ---");
    printf("\nComo deseja ordenar os componentes?");
    printf("\n1. Por Nome (Ordem Alfabetica)");
    printf("\n2. Por Tipo");
    printf("\n3. Por Prioridade de Montagem");
    printf("\n0. Cancelar");
}

// insertionSort():
// Implementação do algoritmo de ordenação por inserção.
// Funciona com diferentes critérios de ordenação:
// - Por nome (ordem alfabética)
// - Por tipo (ordem alfabética)
// - Por prioridade (da mais alta para a mais baixa)
void insertionSortTipo(Mochila *mochila) {
    int n = mochila->quantidade;

    for (int i = 1; i < n; i++) {
        Item chave = mochila->itens[i]; // Guarda o item atual
        int j = i - 1;

        // Desloca os itens maiores que a chave uma posição à frente
        while (j >= 0 && strcmp(mochila->itens[j].tipo, chave.tipo) > 0) {
            mochila->itens[j + 1] = mochila->itens[j];
            j--;
        }

        // Insere a chave na posição correta
        mochila->itens[j + 1] = chave;
    }
    mochila->criterioAtual = ORDENADO_POR_TIPO;
}

void selectionSortPrioridade(Mochila *mochila) {
    int n = mochila->quantidade;

    for (int i = 0; i < n - 1; i++) {
        int indiceMaior = i; // Assume o maior como o índice atual

        // Encontra o índice do MAIOR elemento no restante do array
        for (int j = i + 1; j < n; j++) {
            if ((int)mochila->itens[j].prioridade > (int)mochila->itens[indiceMaior].prioridade) {
                indiceMaior = j;
            }
        }

        // Só troca se encontrou um elemento maior
        if (indiceMaior != i) {
            Item temp = mochila->itens[i];
            mochila->itens[i] = mochila->itens[indiceMaior];
            mochila->itens[indiceMaior] = temp;
        }
    }
    mochila->criterioAtual = ORDENADO_POR_PRIORIDADE;
}

void bubbleSortNome(Mochila *mochila) {
    int n = mochila->quantidade;
    int trocou;

    for (int i = 0; i < n - 1; i++) {
        trocou = 0;

        for (int j = 0; j < n - 1 - i; j++) {
            // Compara os nomes dos itens adjacentes
            if (strcmp(mochila->itens[j].nome, mochila->itens[j + 1].nome) > 0) {
                // Troca os itens de posição
                Item temp = mochila->itens[j];
                mochila->itens[j] = mochila->itens[j + 1];
                mochila->itens[j + 1] = temp;
                trocou = 1;
            }
        }

        // Se não houve troca, o array já está ordenado
        if (!trocou) break;
    }
    mochila->criterioAtual = ORDENADO_POR_NOME;
}

// buscaBinariaPorNome():
// Realiza busca binária por nome, desde que a mochila esteja ordenada por nome.
// Se encontrar, exibe os dados do item buscado.
// Caso contrário, informa que não encontrou o item.
void buscaBinariaPorNome(Mochila *mochila) {
    if(mochila->criterioAtual != ORDENADO_POR_NOME) {
        printf("\nALERTA: A busca binaria requer que a mochila esteja ordenada por Nome.");
        printf("\nUse a Opcao 4 para organizar a mochila primeiro.\n");
        printf("\nPressione Enter para continuar...");
        getchar();
        return;
    }

    int indice = -1;

    char nome[MAX_STR_LEN_NOME];

    printf("\n--- Busca Binaria por Componente-Chave ---");
    printf("\nNome do componente a buscar: ");
    fgets(nome, MAX_STR_LEN_NOME, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    // Busca binária (requer que mochila->itens esteja ordenado por nome)
    int esquerda = 0, direita = mochila->quantidade - 1;

    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        int cmp = strcmp(mochila->itens[meio].nome, nome);

        if (cmp == 0) {
            indice = meio;
            break;
        } else if (cmp < 0) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }

    // Se não encontrou, encerra
    if (indice == -1) {
        printf("\nResultado: Componente '%s' NAO foi encontrado na mochila.\n", nome);
        printf("\nPressione Enter para continuar...");
        getchar();
        return;
    }

    // Encontrou
    printf("\n--- Componente-Chave Encontrado! ---");
    printf("\nNome: %s, Tipo: %s, Qtd: %d, Prio: %d\n",
        nome,
        mochila->itens[indice].tipo,
        mochila->itens[indice].quantidade,
        mochila->itens[indice].prioridade);

    printf("\nPressione Enter para continuar...");
    getchar();
}

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
void limparBufferEntrada() {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}
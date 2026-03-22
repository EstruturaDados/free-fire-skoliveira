#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Código da Ilha – Edição Free Fire
// Nível: Mestre
// Este programa simula o gerenciamento avançado de uma mochila com componentes coletados durante a fuga de uma ilha.
// Ele introduz ordenação com critérios e busca binária para otimizar a gestão dos recursos.

#define MAX_ITENS 10
#define MAX_STR_LEN_NOME 30
#define MAX_STR_LEN_TIPO 20

// --- Estruturas ----
typedef struct {
    char nome[MAX_STR_LEN_NOME];
    char tipo[MAX_STR_LEN_TIPO];
    int quantidade;
} Item;

typedef struct  {
    Item itens[MAX_ITENS];  // lista estática de itens (tamanho fixo)
    int quantidade;         // quantidade atual de itens na mochila
} Mochila;

// --- Prototipagem das funções ----
void exibirMenu(Mochila *mochila);
void inserirItem(Mochila *mochila);
void removerItem(Mochila *mochila);
void listarItens(Mochila *mochila);
void buscarItem(Mochila *mochila);
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
            buscarItem(&mochila);
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

// Struct Item:
// Representa um componente com nome, tipo, quantidade e prioridade (1 a 5).
// A prioridade indica a importância do item na montagem do plano de fuga.

// Enum CriterioOrdenacao:
// Define os critérios possíveis para a ordenação dos itens (nome, tipo ou prioridade).

// Vetor mochila:
// Armazena até 10 itens coletados.
// Variáveis de controle: numItens (quantidade atual), comparacoes (análise de desempenho), ordenadaPorNome (para controle da busca binária).

// limparTela():
// Simula a limpeza da tela imprimindo várias linhas em branco.

// exibirMenu():
// Apresenta o menu principal ao jogador, com destaque para status da ordenação.
void exibirMenu(Mochila *mochila) {
    printf("\n===============================================");
    printf("\n   MOCHILA DE SOBREVIVENCIA - CODIGO DA ILHA");
    printf("\n===============================================");
    printf("\nItens na Mochila: %d/%d\n", mochila->quantidade, MAX_ITENS);
    printf("\n1. Adicionar Item (Loot)");
    printf("\n2. Remover Item");
    printf("\n3. Listar Itens na Mochila");
    printf("\n4. Buscar Item por Nome");
    printf("\n0. Sair");
    printf("\n----------------------------------");
}

// inserirItem():
// Adiciona um novo componente à mochila se houver espaço.
// Solicita nome, tipo, quantidade e prioridade.
// Após inserir, marca a mochila como "não ordenada por nome".
void inserirItem(Mochila *mochila) {
    // Verifica se a mochila está cheia
    if (mochila->quantidade == MAX_ITENS) {
        return;
    }

    // Insere novo item no fim do vetor
    Item *novo = &mochila->itens[mochila->quantidade];

    printf("\n--- Adicionar Novo Item ---");

    printf("\nNome do item: ");
    fgets(novo->nome, MAX_STR_LEN_NOME, stdin);
    novo->nome[strcspn(novo->nome, "\n")] = '\0';

    printf("Tipo do item (arma, municao, cura, etc.): ");
    fgets(novo->tipo, MAX_STR_LEN_TIPO, stdin);
    novo->tipo[strcspn(novo->tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &novo->quantidade);
    limparBufferEntrada();

    mochila->quantidade++;

    printf("\n\nItem '%s' adicionado com sucesso!\n", novo->nome);
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

void buscarItem(Mochila *mochila) {
    int indice = -1;

    char nome[MAX_STR_LEN_NOME];

    printf("\n--- Buscar Item na Mochila ---");
    printf("\nDigite o nome do item que deseja buscar: ");
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
        printf("\nResultado: Item '%s' NAO foi encontrado na mochila.\n", nome);
        printf("\nPressione Enter para continuar...");
        getchar();
        return;
    }

    // Encontrou
    printf("\n--- Item Encontrado! ---");
    printf("\nNome: %s", nome);
    printf("\nTipo: %s", mochila->itens[indice].tipo);
    printf("\nQuantidade: %d", mochila->itens[indice].quantidade);
    printf("\n------------------------\n");

    printf("\nPressione Enter para continuar...");
    getchar();
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
    printf("\n--------------------------------------------");
    printf("\nNOME\t\t| TIPO\t\t| QUANTIDADE");
    printf("\n--------------------------------------------");
    for (int i = 0; i < mochila->quantidade; i++) {
        printf("\n%-15s | %-13s | %d",
               mochila->itens[i].nome,
               mochila->itens[i].tipo,
               mochila->itens[i].quantidade);
    }
    printf("\n--------------------------------------------\n");

    printf("\nPressione Enter para continuar...");
    getchar();
}

// menuDeOrdenacao():
// Permite ao jogador escolher como deseja ordenar os itens.
// Utiliza a função insertionSort() com o critério selecionado.
// Exibe a quantidade de comparações feitas (análise de desempenho).

// insertionSort():
// Implementação do algoritmo de ordenação por inserção.
// Funciona com diferentes critérios de ordenação:
// - Por nome (ordem alfabética)
// - Por tipo (ordem alfabética)
// - Por prioridade (da mais alta para a mais baixa)

// buscaBinariaPorNome():
// Realiza busca binária por nome, desde que a mochila esteja ordenada por nome.
// Se encontrar, exibe os dados do item buscado.
// Caso contrário, informa que não encontrou o item.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
void limparBufferEntrada() {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}
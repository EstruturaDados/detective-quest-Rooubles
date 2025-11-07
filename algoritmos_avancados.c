// Desafio nivel aventureiro

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------- Estruturas ----------
typedef struct {
    char nome[50];
    char pista[100];
} Comodo;

typedef struct no {
    Comodo mansao;
    struct no* esquerdo;
    struct no* direito;
} no;

typedef struct {
    char historico[10][100];
    int inicio;
    int fim;
    int total;
} nav;

// ---------- Dados ----------
Comodo comodos[10] = {
    {"Hall de entrada", ""},
    {"Sala de estar", "Pegada de lama"},
    {"Biblioteca", "Livro faltando pag"},
    {"Quarto", "Lençol manchado"},
    {"Cozinha", ""},
    {"Porao", "Gaveta perdida"},
    {"Sotao", "Chave perdida"},
    {"Varanda", ""},
    {"Banheiro", "espelho embaçado"},
    {"Area de servico", ""}
};

// ---------- Utilitários ----------
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pausa() {
    printf("\nTecle enter para continuar...\n");
    getchar();
}

// ---------- Árvore ----------
no* criarSala(Comodo c) {
    no* novo = malloc(sizeof(no));
    if (!novo) {
        printf("Erro de memória\n");
        exit(1);
    }
    strcpy(novo->mansao.nome, c.nome);
    strcpy(novo->mansao.pista, c.pista);
    novo->esquerdo = NULL;
    novo->direito = NULL;
    return novo;
}

no* inserir(no* raiz, Comodo c) {
    if (raiz == NULL) 
        return criarSala(c);

    if (strcmp(c.nome, raiz->mansao.nome) < 0)
        raiz->esquerdo = inserir(raiz->esquerdo, c);
    else if (strcmp(c.nome, raiz->mansao.nome) > 0)
        raiz->direito = inserir(raiz->direito, c);

    return raiz;
}

void liberar(no* raiz) {
    if (raiz != NULL) {
        liberar(raiz->esquerdo);
        liberar(raiz->direito);
        free(raiz);
    }
}

// ---------- Histórico ----------
void inicializarHistorico(nav* h) {
    h->inicio = 0;
    h->fim = 0;
    h->total = 0;
}

void registrarPasso(nav* h, const char* sala) {
    if (h->total < 10) {
        strcpy(h->historico[h->fim], sala);
        h->fim++;
        h->total++;
    } else {
        printf("Histórico cheio!\n");
    }
}

void mostrarHistorico(nav* h, const char* titulo) {
    printf("\n%s:\n", titulo);
    if (h->total == 0) {
        printf("Nenhum registro ainda.\n\n");
        return;
    }
    for (int i = 0; i < h->total; i++) {
        printf(" - %s\n", h->historico[i]);
    }
    printf("\n");
}

// ---------- Jogo ----------
int main() {
    no* mansao = NULL;
    nav historico;
    nav pistas;
    inicializarHistorico(&historico);
    inicializarHistorico(&pistas);

    // Criar a mansão (árvore binária)
    for (int i = 0; i < 10; i++) {
        mansao = inserir(mansao, comodos[i]);
    }

    no* posicao_atual = mansao;
    int opcao;

    printf("\n============= Detective Quest =============\n");

    do {
        printf("\nVocê está em: %s\n", posicao_atual->mansao.nome);

        // Se houver pista, mostrar e registrar
        if (strlen(posicao_atual->mansao.pista) > 0) {
            printf("Há uma pista aqui: %s\n", posicao_atual->mansao.pista);
            registrarPasso(&pistas, posicao_atual->mansao.pista);
        }

        printf("===========================================\n");
        printf("(1) Ir para a sala à esquerda\n");
        printf("(2) Ir para a sala à direita\n");
        printf("(3) Ver histórico de navegação\n");
        printf("(4) Ver histórico de pistas\n");
        printf("(0) Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                if (posicao_atual->esquerdo) {
                    posicao_atual = posicao_atual->esquerdo;
                    registrarPasso(&historico, posicao_atual->mansao.nome);
                } else {
                    printf("Não há sala à esquerda!\n");
                }
                pausa();
                break;
            case 2:
                if (posicao_atual->direito) {
                    posicao_atual = posicao_atual->direito;
                    registrarPasso(&historico, posicao_atual->mansao.nome);
                } else {
                    printf("Não há sala à direita!\n");
                }
                pausa();
                break;
            case 3:
                mostrarHistorico(&historico, "Histórico de navegação");
                pausa();
                break;
            case 4:
                mostrarHistorico(&pistas, "Pistas encontradas");
                pausa();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
                pausa();
        }

    } while (opcao != 0);

    liberar(mansao);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura do nó (Sala)
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

Sala* criarSala(char* nome) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala) {
        strcpy(novaSala->nome, nome);
        novaSala->esquerda = NULL;
        novaSala->direita = NULL;
    }
    return novaSala;
}

// Lógica de exploração interativa
void explorarMansao(Sala* atual) {
    char escolha;

    while (atual != NULL) {
        printf("\nVoce esta na sala: %s\n", atual->nome)

        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Voce chegou ao fim deste caminho. Nao existe salar para ir.\n");
            break;
        }

        printf("Para onde deseja ir? (1: Esquerda, 2: Direita, 3: Sair): ");
        scanf(" %c", &escolha);

        if (escolha == '3') {
            printf("Saindo da exploracao...\n");
            break;
        } else if (escolha == '1') {
            if (atual->esquerda) atual = atual->esquerda;
            else printf("Caminho bloqueado a esquerda!\n");
        } else if (escolha == '2') {
            if (atual->direita) atual = atual->direita;
            else printf("Caminho bloqueado a direita!\n");
        } else {
            printf("Comando invalido!\n");
        }
    }
}

int main() {
    // Montagem da árvore (Mapa da Mansão)
    Sala* raiz = criarSala("Hall de Entrada");
    
    // Nível 1
    raiz->esquerda = criarSala("Cozinha");
    raiz->direita = criarSala("Biblioteca");

    // Nível 2 (Folhas)
    raiz->esquerda->esquerda = criarSala("Despensa Escura");
    raiz->esquerda->direita = criarSala("Area de Servico");
    raiz->direita->esquerda = criarSala("Sala de Leitura");
    raiz->direita->direita = criarSala("Passagem Secreta");

    printf("BEM-VINDO A MANSAO BINARIA\n");
    explorarMansao(raiz);

    printf("\nObrigado por jogar!\n");
    return 0;
}
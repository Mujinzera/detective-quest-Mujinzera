#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para as Pistas
typedef struct Pista {
    char texto[50];
    struct Pista *esq, *dir;
} Pista;

// Estrutura para as Salas (Mapa)
typedef struct Sala {
    char nome[50];
    char pistaLocal[50]; // Pista escondida na sala (se houver)
    struct Sala *esquerda, *direita;
} Sala;

// pistas

Pista* inserirPista(Pista* raiz, char* texto) {
    if (raiz == NULL) {
        Pista* novo = (Pista*)malloc(sizeof(Pista));
        strcpy(novo->texto, texto);
        novo->esq = novo->dir = NULL;
        return novo;
    }
    if (strcmp(texto, raiz->texto) < 0)
        raiz->esq = inserirPista(raiz->esq, texto);
    else if (strcmp(texto, raiz->texto) > 0)
        raiz->dir = inserirPista(raiz->dir, texto);
    
    return raiz;
}

void exibirPistasEmOrdem(Pista* raiz) {
    if (raiz != NULL) {
        exibirPistasEmOrdem(raiz->esq);
        printf("- %s\n", raiz->texto);
        exibirPistasEmOrdem(raiz->dir);
    }
}

Sala* criarSala(char* nome, char* pista) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pistaLocal, pista);
    nova->esquerda = nova->direita = NULL;
    return nova;
}

void explorar(Sala* atual, Pista** inventario) {
    char escolha;
    while (atual != NULL) {
        printf("\n[SALA]: %s\n", atual->nome);

        // Se houver pista na sala
        if (strcmp(atual->pistaLocal, "") != 0) {
            printf("(!) Voce encontrou uma pista: '%s'\n", atual->pistaLocal);
            *inventario = inserirPista(*inventario, atual->pistaLocal);
            strcpy(atual->pistaLocal, ""); // Limpa a sala para não coletar repetido
        }

        printf("Menu: 1. Direita\n, 2. Esquerda\n, 3. Pistas\n, 4. Sair ");
        scanf(" %c", &escolha);

        if (escolha == '3') {
            printf("\nCaderno de Pistas:\n");
            if (*inventario == NULL) printf("(Vazio)\n");
            else exibirPistasEmOrdem(*inventario);
        } else if (escolha == '2' && atual->esquerda) atual = atual->esquerda;
        else if (escolha == '1' && atual->direita) atual = atual->direita;
        else if (escolha == '4') break;
        else printf("Caminho sem saida ou comando invalido.\n");
    }
}

int main() {
    Pista* inventario = NULL;

    // mansão com pistas
    Sala* hall = criarSala("Hall", "");
    hall->esquerda = criarSala("Cozinha", "Faca de Prata");
    hall->direita = criarSala("Biblioteca", "Diario Antigo");
    hall->direita->esquerda = criarSala("Sala de Leitura", "Chave Dourada");
    hall->direita->direita = criarSala("Sotão", "Anel de Sinete");

    printf("NIVEL AVENTUREIRO: BUSCA PELAS PISTAS\n");
    explorar(hall, &inventario);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_HASH 10
#define TOTAL_PISTAS 9

typedef struct NodoHash {
    char pista[50];
    char suspeito[50];
    struct NodoHash* proximo;
} NodoHash;

typedef struct {
    NodoHash* tabela[TAM_HASH];
} TabelaHash;

typedef struct Pista {
    char texto[50];
    struct Pista *esq, *dir;
} Pista;

typedef struct Sala {
    char nome[50];
    char pistaLocal[50];
    struct Sala *esquerda, *direita;
} Sala;

// --- SISTEMA DE RANDOMIZAÇÃO ---

// Corrigido: Adicionadas vírgulas entre as strings
char* listaPistas[] = {
    "Faca de Prata", 
    "Diario Antigo", 
    "Chave Dourada", 
    "Anel de Sinete", 
    "Luva Suja",
    "Jaleco",
    "Colar da Familia",
    "Parte da Roupa",
    "Frasco de Veneno"
};

void embaralharPistas(char* arr[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char* temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

int funcaoHash(char* str) {
    int soma = 0;
    for (int i = 0; str[i] != '\0'; i++) soma += str[i];
    return soma % TAM_HASH;
}

void cadastrarSuspeito(TabelaHash* ht, char* pista, char* suspeito) {
    int indice = funcaoHash(pista);
    NodoHash* novo = (NodoHash*)malloc(sizeof(NodoHash));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = ht->tabela[indice];
    ht->tabela[indice] = novo;
}

char* buscarSuspeito(TabelaHash* ht, char* pista) {
    int indice = funcaoHash(pista);
    NodoHash* atual = ht->tabela[indice];
    while (atual) {
        if (strcmp(atual->pista, pista) == 0) return atual->suspeito;
        atual = atual->proximo;
    }
    return "Desconhecido";
}

Pista* adicionarAoInventario(Pista* raiz, char* texto) {
    if (raiz == NULL) {
        Pista* novo = (Pista*)malloc(sizeof(Pista));
        strcpy(novo->texto, texto);
        novo->esq = novo->dir = NULL;
        return novo;
    }
    if (strcmp(texto, raiz->texto) < 0) raiz->esq = adicionarAoInventario(raiz->esq, texto);
    else if (strcmp(texto, raiz->texto) > 0) raiz->dir = adicionarAoInventario(raiz->dir, texto);
    return raiz;
}

void listarPistas(Pista* raiz, TabelaHash* ht, int* m, int* j) {
    if (raiz != NULL) {
        listarPistas(raiz->esq, ht, m, j);
        char* suspeito = buscarSuspeito(ht, raiz->texto);
        printf("- %-17s | Suspeito: %s\n", raiz->texto, suspeito);
        if (strcmp(suspeito, "Mordomo") == 0) (*m)++;
        else if (strcmp(suspeito, "Jardineiro") == 0) (*j)++;
        listarPistas(raiz->dir, ht, m, j);
    }
}

Sala* criarSala(char* nome, char* pista) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pistaLocal, (pista == NULL) ? "" : pista);
    nova->esquerda = nova->direita = NULL;
    return nova;
}

void explorar(Sala* atual, Pista** inventario, TabelaHash* ht) {
    char escolha;
    while (atual != NULL) {
        printf("\n==================================");
        printf("\nSALA ATUAL: %s", atual->nome);
        
        if (strcmp(atual->pistaLocal, "") != 0) {
            printf("\nVocê encontrou algo: [%s]!", atual->pistaLocal);
            *inventario = adicionarAoInventario(*inventario, atual->pistaLocal);
            strcpy(atual->pistaLocal, ""); 
        }

        printf("\n----------------------------------");
        printf("\n[e] Esquerda: %s", atual->esquerda ? atual->esquerda->nome : "Bloqueado");
        printf("\n[d] Direita:  %s", atual->direita ? atual->direita->nome : "Bloqueado");
        printf("\n[i] Inventário | [f] Acusar Suspeito\nEscolha: ");
        scanf(" %c", &escolha);

        if (escolha == 'i') {
            int m = 0, j = 0;
            printf("\n--- EVIDÊNCIAS COLETADAS ---\n");
            if (*inventario == NULL) printf("Vazio.\n");
            else listarPistas(*inventario, ht, &m, &j);
        } else if (escolha == 'e' && atual->esquerda) atual = atual->esquerda;
        else if (escolha == 'd' && atual->direita) atual = atual->direita;
        else if (escolha == 'f') {
            int m = 0, j = 0;
            printf("\n--- VEREDITO FINAL ---");
            if (*inventario == NULL) printf("\nVocê não tem provas!");
            else {
                printf("\nRelatório:\n");
                listarPistas(*inventario, ht, &m, &j);
                if (m > j) printf("\nO MORDOMO FOI PRESO! (%d provas contra ele)\n", m);
                else if (j > m) printf("\nO JARDINEIRO FOI PRESO! (%d provas contra ele)\n", j);
                else printf("\nEmpate técnico! Mais investigações são necessárias.\n");
            }
            return;
        }
    }
}

int main() {
    srand(time(NULL));
    TabelaHash ht;
    for(int i=0; i<TAM_HASH; i++) ht.tabela[i] = NULL; // Inicializando a Hash
    Pista* inventario = NULL;

    cadastrarSuspeito(&ht, "Faca de Prata", "Mordomo");
    cadastrarSuspeito(&ht, "Chave Dourada", "Mordomo");
    cadastrarSuspeito(&ht, "Anel de Sinete", "Mordomo");
    cadastrarSuspeito(&ht, "Diario Antigo", "Jardineiro");
    cadastrarSuspeito(&ht, "Luva Suja", "Jardineiro");
    cadastrarSuspeito(&ht, "Jaleco", "Jardineiro");
    cadastrarSuspeito(&ht, "Colar da Familia", "Mordomo");
    cadastrarSuspeito(&ht, "Parte da Roupa", "Jardineiro");
    cadastrarSuspeito(&ht, "Frasco de Veneno", "Jardineiro");

    embaralharPistas(listaPistas, TOTAL_PISTAS);

    Sala* hall = criarSala("Hall de Entrada", "");
    hall->esquerda = criarSala("Cozinha", listaPistas[0]);
    hall->direita = criarSala("Biblioteca", listaPistas[1]);
    
    hall->esquerda->esquerda = criarSala("Despensa", listaPistas[2]);
    hall->esquerda->direita = criarSala("Sala de Jantar", listaPistas[6]);
    
    hall->direita->direita = criarSala("Sotão", listaPistas[3]);
    hall->direita->esquerda = criarSala("Jardim", listaPistas[4]);
    
    hall->direita->esquerda->direita = criarSala("Quarto de Hospedes", listaPistas[5]);
    hall->esquerda->esquerda->esquerda = criarSala("Adega", listaPistas[7]);
    hall->direita->direita->direita = criarSala("Suite Master", listaPistas[8]);

    printf("--- DETETIVE NIVEL MESTRE ---\n");
    explorar(hall, &inventario, &ht);

    return 0;
}
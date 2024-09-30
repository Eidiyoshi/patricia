#include <stdio.h>
#include <string.h>

typedef struct patricia_node {
    int bit; // Posição do bit que discrimina este nó
    char *key; // Chave armazenada no nó, NULL para nós intermediários
    struct patricia_node *left; // Subárvore esquerda
    struct patricia_node *right; // Subárvore direita
} PatriciaNode;

void inicializa(PatriciaNode **arvore) {
    *arvore = malloc(sizeof(PatriciaNode));
    (*arvore)->chave = UINT_MAX;
    (*arvore)->esq = (*arvore)->dir = *arvore;
    (*arvore)->bit = -1;
}


#include <stdio.h>
#include <string.h>

typedef struct patricia_node {
    int bit; // Posi��o do bit que discrimina este n�
    char *key; // Chave armazenada no n�, NULL para n�s intermedi�rios
    struct patricia_node *left; // Sub�rvore esquerda
    struct patricia_node *right; // Sub�rvore direita
} PatriciaNode;

void inicializa(PatriciaNode **arvore) {
    *arvore = malloc(sizeof(PatriciaNode));
    (*arvore)->chave = UINT_MAX;
    (*arvore)->esq = (*arvore)->dir = *arvore;
    (*arvore)->bit = -1;
}


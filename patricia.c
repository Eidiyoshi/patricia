#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

int tamanho;

typedef struct patricia_node {
    int bit; // Posição do bit que discrimina este nó
    unsigned chave; // Chave armazenada no nó, NULL para nós intermediários // ? como assim nos intermediario
    struct patricia_node *esq; // Subárvore esquerda
    struct patricia_node *dir; // Subárvore direita
} PatriciaNode;

unsigned bit(unsigned chave, int k) {
    return chave >> ( tamanho - 1 - k) & 1;
}

PatriciaNode *busca_rec(PatriciaNode *arvore, unsigned x, int nivel) {
    if (arvore == NULL){
        return NULL;
    }
    if (x == arvore->chave){
    return arvore;
    }
    if (bit(x, nivel) == 0){
        return busca_rec(arvore->esq, x, nivel+1);
    }else{
        return busca_rec(arvore->dir, x, nivel+1);
    }
}

PatriciaNode *insere_rec(PatriciaNode *arvore, unsigned chave, int w, PatriciaNode *pai) {
    PatriciaNode *novo;
    if ((arvore->bit >= w) || (arvore->bit <= pai->bit)) {
        novo = (PatriciaNode*) malloc(sizeof(PatriciaNode));
        novo->chave = chave;
        novo->bit = w;
        if (bit(chave, novo->bit) == 1) {
            novo->esq = arvore;
            novo->dir = novo;
        } else {
        novo->esq = novo;
        novo->dir = arvore;
        }
        return novo;
    }

    if (bit(chave, arvore->bit) == 0){
        arvore->esq = insere_rec(arvore->esq, chave, w, arvore);
    }else{
        arvore->dir = insere_rec(arvore->dir, chave, w, arvore);
    }

    return arvore;
}

PatriciaNode* inicializa(){
    PatriciaNode *arvore = malloc(sizeof(PatriciaNode));
    arvore->chave = UINT_MAX;
    arvore->esq = arvore->dir = arvore;
    arvore->bit = -1;

    printf("Digite a primeira chave: ");
    unsigned chave;
    fflush(stdin);
    scanf("%u",&chave);

    arvore->esq = insere_rec(arvore, chave, 0, arvore);

    return arvore;
}

void insere(PatriciaNode *arvore, unsigned chave) {
    int i;
    PatriciaNode *t = busca_rec(arvore->esq, chave, -1);

    if (chave == t->chave){
        return;
    }

    for (i = 0; bit(chave, i) == bit(t->chave, i); i++) ;
    arvore->esq = insere_rec(arvore->esq, chave, i, arvore);
}

PatriciaNode * busca(PatriciaNode *arvore, unsigned x) {
    return busca_rec(arvore, x, 0);
}


void main(){
    printf("Bem vindo trabalho arvore Patricia\n");
    printf("Os inputs sao numeros reais nao binarios\n\n");

    printf("Digite o tamanho das chaves binarias: ");
    scanf("%d", &tamanho);

    PatriciaNode* raiz = inicializa();
    unsigned chave;

    int escolha;
    do{
        printf("1- Inserir chave\n");
        printf("2- Buscar chave\n");
        printf("3- Printar arvore\n");
        printf("4- Remover ( nao funcionando )\n");

        printf("99- Sair para sempre desse mundo\n\n");


        fflush(stdin);
        printf("Digite sua escolha: ");
        scanf("%d",&escolha);

        switch(escolha){
            case 1:
                printf("Qual valor deseja inserir?\n");

                scanf("%u",&chave);

                insere(raiz,chave);
                break;

            case 2:
                printf("Qual valor deseja buscar?\n");

                scanf("%u",&chave);

                if( busca(raiz,chave) != NULL ){

                }
                break;

            case 3:
                //printar;
                break;

            case 4:
                //remover;
                break;

            default:
                break;
        }

    }while ( escolha != 99);

}

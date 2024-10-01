#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <math.h>

int tamanho;

typedef struct patricia_node {
    int bit; // Posição do bit que discrimina este nó
    unsigned chave; // Chave armazenada no nó, NULL para nós intermediários // ? como assim nós intermediario
    struct patricia_node *esq; // Subárvore esquerda
    struct patricia_node *dir; // Subárvore direita
} PatriciaNode;

unsigned bit(unsigned chave, int k) {
    return chave >> ( tamanho - 1 - k) & 1;
}

PatriciaNode *busca_rec(PatriciaNode *arvore, unsigned x, int w) {
    if (arvore->bit <= w){
        return arvore;
    }
    if (bit(x, arvore->bit) == 0){
        return busca_rec(arvore->esq, x, arvore->bit);
    }
    else{
        return busca_rec(arvore->dir, x, arvore->bit);
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

PatriciaNode *busca(PatriciaNode *arvore, unsigned x) {
    PatriciaNode *t = busca_rec(arvore->esq, x, -1);
    return t->chave == x ? t : NULL; // se t->chave == x, retorna t, caso falso null
}

void printar_info(PatriciaNode *raiz){
    printf("  \n");
    if( raiz->bit != -1 ){ // criacao de espacos
        for(int i = 0; i <= raiz->bit; i++){
            printf("     ");
        }
    }


    char buffer[tamanho];
    itoa(raiz->chave, buffer, 2); // transformacao da chave pra binario

    char zeros[tamanho];
    strcpy(zeros,"");

    int contador = tamanho, modificador = strlen(buffer);
    while( ( contador - modificador ) > 0 ){
        strcat(zeros, "0");
        modificador++;
    }

    strcat(zeros,buffer);

    if(raiz->bit == -1){
        printf("Bit: -1, Chave:MAX_INT\n");
    }else{
        printf("Bit: %d, Chave: %s\n", raiz->bit, zeros );
    }
}

void pegar_rec(PatriciaNode *raiz){

    if(raiz == NULL){
        return;
    }

    if( raiz->esq->bit > raiz->bit ){
        for(int i = 0; i <= raiz->bit; i++){
            printf("      ");
        }
        pegar_rec(raiz->esq);
    }else{
        printf("\n");
    }

    printar_info(raiz);

    if( raiz->dir->bit > raiz->bit){

        for(int i = 0; i <= raiz->bit; i++){
            printf("      ");
        }
        pegar_rec(raiz->dir);
    }else{
        printf("\n");
    }
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
        printf("\n\n1- Inserir chave\n");
        printf("2- Buscar chave\n");
        printf("3- Printar arvore\n");
        printf("4- Remover ( nao funcionando )\n");

        printf("99- Sair\n\n");


        fflush(stdin);
        printf("Digite sua escolha: ");
        scanf("%d",&escolha);

        switch(escolha){
            case 1:
                printf("Valor que deseja inserir: ");

                scanf("%u",&chave);

                if( pow(2,tamanho) - 1 <= chave  ){
                    printf("\nATENCAO!!! VALOR EXCEDIDO, POR FAVOR DIGITE OUTRO !!!\n");
                }else{
                    insere(raiz,chave);
                    printf("Valor inserido com sucesso");
                }
                break;

            case 2:
                printf("Qual valor deseja buscar: ");

                scanf("%u",&chave);

                if( busca(raiz,chave) != NULL ){
                    printf("\nEla existe\n");
                }else{
                    printf("\nNao existe\n");
                }
                break;

            case 3:
                pegar_rec(raiz);
                break;

            case 4:
                //remover;
                break;

            default:
                break;
        }

    }while ( escolha != 99);

}

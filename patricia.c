#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <math.h>

// Trabalho ED2 arvore patricia
// Cristian Eidi Yoshimura
// a raiz max int sempre usa a arvore a esquerda


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
    strcpy(zeros,""); // nullificando os 0 q serao printados

    int contador = tamanho, modificador = strlen(buffer);
    while( ( contador - modificador ) > 0 ){
        strcat(zeros, "0");
        modificador++; // complementar os 0 a esquerda
    }

    strcat(zeros,buffer);

    if(raiz->bit == -1){
        printf("Bit: -1, Chave:MAX_INT\n"); // printar o max int sendo o tamanho de bits desejado menor que o max int dava problemas
    }else{
        printf("Bit: %d, Chave: %s, Valor: %d\n", raiz->bit, zeros, raiz->chave );
    }
}

void printar_rec(PatriciaNode *raiz){

    if(raiz == NULL){
        return;
    }

    if( raiz->dir->bit > raiz->bit){
        for(int i = 0; i <= raiz->bit; i++){
            printf("      ");
        }
        printar_rec(raiz->dir);
    }else{
        printf("\n");
    }

    printar_info(raiz);

    if( raiz->esq->bit > raiz->bit ){
        for(int i = 0; i <= raiz->bit; i++){
            printf("      ");
        }
        printar_rec(raiz->esq);
    }else{
        printf("\n");
    }

}

void removerSolo(PatriciaNode *arvore, unsigned chave){
    PatriciaNode *t = arvore;
    if(arvore->esq->chave == chave){ // o filho esquerdo eh oq qr remover
        t = t->esq;
        if( t->esq == t ){
            arvore->esq = t->dir;
        }else{
            arvore->esq = t->esq;
        }
        free(t);
        return;
    }

    if(arvore->dir->chave == chave ){ // o filho direito eh oq qr remover
        t = t->dir;
        if( t->esq == t ){
            arvore->dir = t->dir;
        }else{
            arvore->dir = t->esq;
        }
        free(t);
        return;
    }


    if (bit(chave, arvore->bit) == 0){
        removerSolo(arvore->esq, chave);
    }else{
        removerSolo(arvore->dir, chave);
    }

}

void removerMeio(PatriciaNode *arvore, unsigned chave){
    PatriciaNode *ponteiroDebaixo = arvore;
    PatriciaNode *paiDebaixo;
    while( ponteiroDebaixo->esq->chave != chave && ponteiroDebaixo->dir->chave != chave ){
        if( bit(chave, ponteiroDebaixo->bit ) == 0 ){
            paiDebaixo = ponteiroDebaixo;
            ponteiroDebaixo = ponteiroDebaixo->esq;
        }else{
            paiDebaixo = ponteiroDebaixo;
            ponteiroDebaixo = ponteiroDebaixo->dir;
        }
    }

    PatriciaNode *ponteiroMaisBaixo = ponteiroDebaixo; // achar o q aponta pro ponteiroDebaixo
    while( ponteiroMaisBaixo->esq->chave != ponteiroDebaixo->chave && ponteiroMaisBaixo->dir->chave != ponteiroDebaixo->chave &&
           ponteiroMaisBaixo->bit >= ponteiroDebaixo->bit ){
        if( bit(chave, ponteiroMaisBaixo->bit ) == 0 ){
            ponteiroMaisBaixo = ponteiroMaisBaixo->esq;
        }else{
            ponteiroMaisBaixo = ponteiroMaisBaixo->dir;
        }
    }

    arvore->chave = ponteiroDebaixo->chave; // transformando a chave a deletar no cara q aponta pra ela

    if( ponteiroMaisBaixo->esq == ponteiroDebaixo ){ // oq apontava pro debaixo agr aponta pra o original
        ponteiroMaisBaixo->esq = arvore;
    }else{
        ponteiroMaisBaixo->dir = arvore;
    }

    if( paiDebaixo->esq == ponteiroDebaixo ){ // "sumir" com o no do ponteiro debaixo, fazendo com q o pai aponte pro filho q n seja
        if( ponteiroDebaixo->esq == arvore ){ // a chave a deletar
            paiDebaixo->esq = ponteiroDebaixo->dir;
        }else{
            paiDebaixo->esq = ponteiroDebaixo->esq;
        }
    }else{ // ponteiro do pai para o debaixo for pra direita
        if( ponteiroDebaixo->esq == arvore ){
            paiDebaixo->dir = ponteiroDebaixo->dir;
        }else{
            paiDebaixo->dir = ponteiroDebaixo->esq;
        }
    }

    free(ponteiroDebaixo);
}

void remover(PatriciaNode *raiz, unsigned chave){
    PatriciaNode *t = busca_rec(raiz->esq,chave,-1);
    if( t->chave == chave ){ // a chave existe
        if( t->esq == t || t->dir == t ){ // um ponteiro apontado pra ele
            removerSolo(raiz, chave);
        }else{ // nenhum ponteiro sobre ele
            removerMeio(t, chave);
        }

        printf("Removido com sucesso\n");
        return;
    } else{ // chave nao existe
        printf("Nao encontrada\n");
        system("pause");
        return;
    }
}

void main(){
    printf("Bem vindo trabalho arvore Patricia\n");
    printf("Os inputs sao numeros reais nao binarios\n");
    printf("O print eh horizontal, com a primeira arvore a esquerda da raiz\n");

    printf("Digite o tamanho das chaves binarias: ");
    scanf("%d", &tamanho);

    PatriciaNode* raiz = inicializa();
    unsigned chave;

    int escolha;
    do{
        printf("\n\n1- Inserir chave\n");
        printf("2- Buscar chave\n");
        printf("3- Printar arvore horizontalmente\n");
        printf("4- Remover\n");

        printf("99- Sair\n\n");


        fflush(stdin);
        printf("Digite sua escolha: ");
        scanf("%d",&escolha);

        switch(escolha){
            case 1:
                printf("Valor que deseja inserir: ");
                scanf("%u",&chave);

                if( (pow(2,tamanho) <= chave) ){
                    printf("\Atencao! Valor excedido, por favor digite outro!\n");
                    system("pause");
                    break;
                }

                if( chave == 0 ){
                    printf("\nVoce parece ter escrito um valor invalido\nSeu valor eh o numero 0?");
                    printf(" Caso o valor inserido seja 0, insira 1, caso contrario, insira 0: ");
                    int certeza;
                    fflush(stdin);
                    scanf("%d",&certeza);
                    if(!certeza){
                        printf("saindo...");
                        break;
                    }
                }

                insere(raiz,chave);
                printf("Valor inserido com sucesso");
                printar_rec(raiz);
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
                printar_rec(raiz);
                break;

            case 4:
                printf("Qual valor deseja remover: ");
                fflush(stdin);
                scanf("%u",&chave);

                remover(raiz, chave);
                printar_rec(raiz);
                break;

            default:
                printf("?");
                break;
        }

    }while ( escolha != 99);

}

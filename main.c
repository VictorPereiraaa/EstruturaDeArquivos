/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Variaveis Globais
int qtdPalavras = 0;

//Essa struct será  usada como
//uma lista ligada de palavras
typedef struct Palavras {
    char palavra[50];
    struct Palavras * prox;
}Palavras;

//Conta as palavras do arquivo
//passado por parametro
void contaPalavras(FILE *  arq) {
    
    char aux;

    while (!feof(arq)) {
        aux = fgetc(arq);

        if (aux == ' ' ||  aux == '\n')
            qtdPalavras++;
    }

    //utilizamos esse if para adicionar a ultima palavra
    //ja que a mesma não está separada por um espaço,
    //assim não estava contabilizando ao total de palavras
    if (qtdPalavras > 0) {
        qtdPalavras++;
    }
    //volta para o inicio do arquivo
    rewind(arq);

}

//Faz o cabeçalho
//
void cabecalho() {
    FILE *arq;
    char byte1, byte2, character;
    int contByte1 = 0, contByte2 = 0;

    //conta o numero de palavras do arquivo
    arq = fopen("arquivotexto.txt", "r");
    while (!feof(arq)) {
        character = fgetc(arq);
        if (character == ' ') {
            contByte2++;
            if (contByte2 == 10) {
                contByte1++;
                contByte2 = 0;
            }
        }
    }
    fclose(arq);

    //soma a ultima palavra
    if (contByte2 == 10) {
        contByte1++;
        contByte2 = 0;
    } else {
        contByte2++;
    }

    //converte int para char
    byte1 = contByte1 + 48;
    byte2 = contByte2 + 48;

    //escreve no arq compactado
    arq = fopen("arquivoCompactado.txt", "w");
    fprintf(arq, "%c%c", byte1, byte2);

    fclose(arq);

}

// Esta função remove as palavras
// duplicadas que estão  na lista ligada
void removerPalavrasDuplicadas(Palavras * p) {

    Palavras * p1, *p2, *duplicada;
    p1 = p;

    //pegar elemento por elemento e
    //compara com a arvore auxiliar
    while(p1 != NULL && p1->prox != NULL) {
        p2 = p1;

        //comparar esses elementos com outros elementos
        
        while(p2->prox != NULL) {

            //se tiver duplicado, deleta o nó
            if(strcmp(p1->palavra, p2->prox->palavra) == 0) {
                duplicada = p2->prox;
                p2->prox = p2->prox->prox;
                free(duplicada);
            } else {
                p2 = p2->prox;
            }
        }
        p1 = p1->prox;
    }

    
}

//Essa função exclui a primeira palavra
int excluiPalavra(Palavras ** p) {
    char aux[5];
    Palavras * proxPalavra = NULL;

    if(p  == NULL) {
        return -1;
    }

    proxPalavra = (*p)->prox;
    strcpy(aux, (*p)->palavra);
    free(*p);
    *p = proxPalavra;
    
    return 0;
}

//Printar todas palavras da lista
void printarPalavras(Palavras * p) {

    while(p != NULL) {
        printf("%s ", p->palavra);
        p = p->prox;
    }
    printf("\n");
}

//Essa função vai ler as palavras
//do Arquivo e salvar na lista
int lerPalavrasDoArq(Palavras * p, FILE * arq) {
    int j = 0;
    char c[50];
    
    while(j < qtdPalavras) {
        fscanf(arq, "%[^ ]", c);
        fseek(arq, +1, SEEK_CUR);
        p->prox = (Palavras *) malloc(sizeof(Palavras));
        strcpy(p->palavra, c);
        p = p->prox;
        j++;
        
    }

    return 0;
}

int main() {
    Palavras * p = NULL;
    p = (Palavras *) malloc(sizeof(Palavras));

    FILE * arqTxt;
    FILE * arqCmp;
    
    //abre o txt para leitura e o cmp para escrita
    arqTxt = fopen("arquivotexto.txt", "r");
    arqCmp = fopen("arquivoCompactado.txt", "a+");

    contaPalavras(arqTxt);

    
    //cabecalho();
    //listarPalavras();
    lerPalavrasDoArq(p, arqTxt);
    printarPalavras(p);
    removerPalavrasDuplicadas(p);
    printarPalavras(p);
    printf("DEU BOM!!!\n");

    return 0;
}
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
    int posDezena;
    int posUnidade;
    struct Palavras * prox;
}Palavras;

//Conta as palavras do arquivo
//passado por parametro
void contaPalavras(FILE *  arq) {
    
    char aux = 0;
    int i = 0;
    fseek(arq, 0, SEEK_END);
    long tamanho = ftell(arq);
    rewind(arq);
    char* buffer = (char *) malloc(4096 * sizeof(char));
    while (aux < tamanho) {
        aux = fread(buffer, 1, 4096, arq);
        printf("aux: %d", aux);
        for (i = 0; i < aux; i++){
            if (buffer[i] == ' ' || buffer[i] ==  '\n')
                qtdPalavras++;
        }
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

void criarCabecalho(Palavras  * p, FILE * arquivoCompactado) {
    int contaDezena = 0, contaUnidade = 0;
    char byteDezena, byteUnidade;

    while(p->prox != NULL) {

        contaUnidade++;
        if(contaUnidade == 10)  {
            contaDezena++;
            contaUnidade =  0;
        }
        p = p->prox;
    }

    //Converte int para char
    byteDezena  = contaDezena;
    byteUnidade = contaUnidade;

    //escreve no arq compactado
    fprintf(arquivoCompactado, "%c%c ", byteDezena, byteUnidade);
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

void escreverPalavrasNoArquivo(Palavras * p) {
    FILE * arqCompactado;
    arqCompactado = fopen("arquivoCompactado.txt", "a+");
    int contaUnidade = 0, contaDezena = 0;
    while(p->prox != NULL) {

        //Somente vai escrever as palavras 
        //com mais de 3 letras
        if(strlen(p->palavra) > 3 ) {
            fprintf(arqCompactado, "%s,", p->palavra);
            p->posDezena = contaDezena;
            p->posUnidade = contaUnidade;
            printf("%s ", p->palavra);
            printf("%d %d", p->posDezena, p->posUnidade);
            contaUnidade++;
            if(contaUnidade == 10)  {
                contaDezena++;
                contaUnidade =  0;
            }
        }
        p =  p->prox;
    }
}

void escreverOrdemDasPalavrasNoArquivo(Palavras * p) {
    FILE * arqCompactado;
    arqCompactado = fopen("arquivoCompactado.txt", "a+");
    int contaDezena = 0, contaUnidade = 0;
    char byteDezena, byteUnidade;

    
    while(p->prox != NULL) {

        //Somente vai escrever as palavras 
        //com mais de 3 letras
        if(strlen(p->palavra) > 3 ) {
            fprintf(arqCompactado, "%c%c", contaDezena+48, contaUnidade+48);
            printf("%c%c ", contaDezena+48, contaUnidade+48);
            contaUnidade++;
            if(contaUnidade == 10)  {
                contaDezena++;
                contaUnidade =  0;
            }
        }
        p =  p->prox;
    }
}

//Esta função serve para excluir pontos e virgulas
//nas palavras, para depois conseguirmos verificar se
//há palavras iguais.
void removerVirgulasEPontosNasPalavras(Palavras * p) {

    char  c;
    int ultimoChar;

    while(p != NULL) {

        //Encontra as palavras que possuem ou virgula ou ponto
        //para poder excluir 
        char * virgulaPosicao = strchr(p->palavra, ',');
        char * pontoPosicao = strchr(p->palavra, '.');

        //caso ele encontre uma virgula
        //exclui a ultima letra da palavra
        //sendo ou uma virgula ou um ponto,
        //respectivamente
        if(virgulaPosicao != NULL) {

            //contador de letras
            ultimoChar = strlen(p->palavra); 
            p->palavra[ultimoChar - 1] = '\0'; 
        }

        if(pontoPosicao != NULL) {

            //contador de letras
            ultimoChar = strlen(p->palavra);
            p->palavra[ultimoChar - 1] = '\0';
        }
        p = p->prox;
    }
}

//Essa função vai ler as palavras
//do Arquivo e salvar na lista
int lerPalavrasDoArq(Palavras * p, FILE * arq) {
    int j = 0;
    char palavra[50];

    char aux = 0;
    int i = 0;
    fseek(arq, 0, SEEK_END);
    long tamanho = ftell(arq);
    rewind(arq);
    char* buffer = (char *) malloc(4096 * sizeof(char));
    while (aux < tamanho) {
        aux = fread(buffer, 1, 4096, arq);
        for (i = 0; i < aux; i++){
            if (buffer[i] == ' ' || buffer[i] ==  '\n'){
                p->prox = (Palavras *) malloc(sizeof(Palavras));
                strcpy(p->palavra, palavra);
                strcpy(palavra, "");
                p = p->prox;
                j = 0;
            } else {
                palavra[j] = buffer[i];
            }
        }
    }
    
    // while(j < qtdPalavras) {
    //     fscanf(arq, "%[^ ]", c);
    //     fseek(arq, +1, SEEK_CUR);
    //     p->prox = (Palavras *) malloc(sizeof(Palavras));
    //     strcpy(p->palavra, c);
    //     p = p->prox;
    //     j++;
    // }

    return 0;
}

int main() {
    Palavras * p = NULL;
    p = (Palavras *) malloc(sizeof(Palavras));

    FILE * arqTxt;
    FILE * arqCmp;
    
    //abre o txt para leitura e o cmp para escrita
    arqTxt = fopen("arquivotexto.txt", "r");
    arqCmp = fopen("arquivoCompactado.txt", "w");

    contaPalavras(arqTxt);

    
    
    // listarPalavras();
    lerPalavrasDoArq(p, arqTxt);
    printarPalavras(p);
    removerVirgulasEPontosNasPalavras(p);
    removerPalavrasDuplicadas(p);
    printarPalavras(p);
    criarCabecalho(p, arqCmp);
    escreverPalavrasNoArquivo(p);
    escreverOrdemDasPalavrasNoArquivo(p);
    printf("DEU BOM!!!\n");

    return 0;
}
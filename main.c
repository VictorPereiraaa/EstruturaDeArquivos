/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

//Variaveis Globais
int qtdPalavras = 0;

//Essa struct será  usada como
//uma lista ligada de palavras
typedef struct Palavras
{
    char palavra[50];
    int posDezena;
    int posUnidade;
    struct Palavras *prox;
} Palavras;

void limparPalavra(char *palavra, int tam)
{
    int i = 0;
    for (; i < tam; i++)
    {
        palavra[i] = '\0';
    }
}

//Conta as palavras do arquivo
//passado por parametro
void contaPalavras(Palavras *p)
{
    qtdPalavras = 0;
    p = p->prox;
    while (p != NULL)
    {
        qtdPalavras++;
        p = p->prox;
    }

    printf("qtdPalavras: %d", qtdPalavras);
}

int getWordRRN(Palavras *p, char *palavra)
{
    int pos = -1;
    int i = -1;

    while (p != NULL)
    {
        if (strcmp(p->palavra, palavra) == 0)
        {
            pos = i;
            break;
        }
        i++;
        p = p->prox;
    }

    return pos >= 0 ? pos : -1;
}

int escreverRRNsNoArquivo(Palavras *p, FILE *arquivoCompactado, FILE *arq)
{
    int j = 0;
    int i = 0;
    int posPalavra;
    int byteDezena = 0, byteUnidade = 0;

    char palavra[50];
    char aux = 0;

    limparPalavra(palavra, strlen(palavra));

    fseek(arq, 0, SEEK_END);
    long tamanho = ftell(arq);
    rewind(arq);
    char *buffer = (char *)malloc(4096 * sizeof(char));
    while (aux < tamanho)
    {
        aux = fread(buffer, 1, 4096, arq);
        for (i = 0; i < aux; i++)
        {
            if (buffer[i] < 48 || (buffer[i] > 57 && buffer[i] < 65) || (buffer[i] > 90 && buffer[i] < 97) || buffer[i] == '\n')
            {

                if (j > 3)
                {
                    posPalavra = getWordRRN(p, palavra);

                    if (posPalavra < 0)
                    {
                        printf("word not found...\n");
                        return 1;
                    }

                    byteDezena = posPalavra / 10;
                    byteUnidade = posPalavra % 10;

                    fprintf(arquivoCompactado, "%c%c%c%c", 255, byteDezena, byteUnidade, buffer[i]);
                }
                else
                {
                    fprintf(arquivoCompactado, "%s%c", palavra, buffer[i]);
                }

                limparPalavra(palavra, strlen(palavra));
                j = 0;
            }
            else
            {
                palavra[j] = buffer[i];
                j++;
            }
        }
    }

    return 0;
}

void escreverPalavrasNoArquivo(Palavras *p, FILE *arqCompactado)
{
    p = p->prox;
    while (p != NULL)
    {

        fprintf(arqCompactado, "%s,", p->palavra);
        p = p->prox;
    }
}

int criarCabecalho(Palavras *p, FILE *arquivoCompactado, FILE *arq)
{
    fprintf(arquivoCompactado, "%c%c", qtdPalavras / 10, qtdPalavras % 10);
    escreverPalavrasNoArquivo(p, arquivoCompactado);
    escreverRRNsNoArquivo(p, arquivoCompactado, arq);

    // int contaDezena = 0, contaUnidade = 0;
    // char byteDezena, byteUnidade;

    // //Converte int para char
    // byteDezena = contaDezena;
    // byteUnidade = contaUnidade;

    // //escreve no arq compactado

    return 0;
}

// Esta função remove as palavras
// duplicadas que estão  na lista ligada
void removerPalavrasDuplicadas(Palavras *p)
{

    Palavras *p1, *p2, *duplicada;
    p1 = p;

    //pegar elemento por elemento e
    //compara com a arvore auxiliar
    while (p1 != NULL && p1->prox != NULL)
    {
        p2 = p1;

        //comparar esses elementos com outros elementos

        while (p2->prox != NULL)
        {

            //se tiver duplicado, deleta o nó
            if (strcmp(p1->palavra, p2->prox->palavra) == 0)
            {
                duplicada = p2->prox;
                p2->prox = p2->prox->prox;
                free(duplicada);
            }
            else
            {
                p2 = p2->prox;
            }
        }
        p1 = p1->prox;
    }
}

//Essa função exclui a primeira palavra
int excluiPalavra(Palavras **p)
{
    char aux[5];
    Palavras *proxPalavra = NULL;

    if (p == NULL)
    {
        return -1;
    }

    proxPalavra = (*p)->prox;
    strcpy(aux, (*p)->palavra);
    free(*p);
    *p = proxPalavra;

    return 0;
}

//Printar todas palavras da lista
void printarPalavras(Palavras *p)
{

    while (p != NULL)
    {
        printf("%s ", p->palavra);
        p = p->prox;
    }
    printf("\n");
}

void escreverOrdemDasPalavrasNoArquivo(Palavras *p)
{
    FILE *arqCompactado;
    arqCompactado = fopen("arquivoCompactado.txt", "a+");
    int contaDezena = 0, contaUnidade = 0;
    char byteDezena, byteUnidade;

    while (p->prox != NULL)
    {

        //Somente vai escrever as palavras
        //com mais de 3 letras
        if (strlen(p->palavra) > 3)
        {
            fprintf(arqCompactado, "%c%c", contaDezena + 48, contaUnidade + 48);
            contaUnidade++;
            if (contaUnidade == 10)
            {
                contaDezena++;
                contaUnidade = 0;
            }
        }
        p = p->prox;
    }
}

//Esta função serve para excluir pontos e virgulas
//nas palavras, para depois conseguirmos verificar se
//há palavras iguais.
void removerVirgulasEPontosNasPalavras(Palavras *p)
{

    char c;
    int ultimoChar;

    while (p != NULL)
    {

        //Encontra as palavras que possuem ou virgula ou ponto
        //para poder excluir
        char *virgulaPosicao = strchr(p->palavra, ',');
        char *pontoPosicao = strchr(p->palavra, '.');

        //caso ele encontre uma virgula
        //exclui a ultima letra da palavra
        //sendo ou uma virgula ou um ponto,
        //respectivamente
        if (virgulaPosicao != NULL)
        {

            //contador de letras
            ultimoChar = strlen(p->palavra);
            p->palavra[ultimoChar - 1] = '\0';
        }

        if (pontoPosicao != NULL)
        {

            //contador de letras
            ultimoChar = strlen(p->palavra);
            p->palavra[ultimoChar - 1] = '\0';
        }
        p = p->prox;
    }
}

//Essa função vai ler as palavras
//do Arquivo e salvar na lista
int lerPalavrasDoArq(Palavras *p, FILE *arq)
{
    int j = 0;
    char palavra[50];

    limparPalavra(palavra, strlen(palavra));
    char aux = 0;
    int i = 0;
    fseek(arq, 0, SEEK_END);
    long tamanho = ftell(arq);

    rewind(arq);

    char *buffer = (char *)malloc(4096 * sizeof(char));
    while (aux < tamanho)
    {
        aux = fread(buffer, 1, 4096, arq);
        for (i = 0; i < aux; i++)
        {
            if (buffer[i] < 48 || (buffer[i] > 57 && buffer[i] < 65) || (buffer[i] > 90 && buffer[i] < 97) || buffer[i] == '\n')
            {

                if (strlen(palavra) > 3)
                {
                    if (p == NULL)
                    {
                        p = (Palavras *)malloc(sizeof(Palavras));
                        strcpy(p->palavra, palavra);
                    }
                    else
                    {
                        p->prox = (Palavras *)malloc(sizeof(Palavras));
                        strcpy(p->prox->palavra, palavra);
                        p = p->prox;
                    }
                }

                limparPalavra(palavra, strlen(palavra));
                j = 0;
            }
            else
            {
                palavra[j] = buffer[i];
                j++;
            }
        }
    }

    return 0;
}

void comprimir(char *arqName)
{
    Palavras *p = NULL;
    p = (Palavras *)malloc(sizeof(Palavras));

    char *compressFileName = arqName;

    strcat(compressFileName, ".cmp");

    FILE *arqTxt;
    FILE *arqCmp;

    //abre o txt para leitura e o cmp para escrita

    arqTxt = fopen("arquivotexto.txt", "r");
    arqCmp = fopen("arquivoCompactado.txt", "w");
    // arqTxt = fopen(arqName, "r");
    // arqCmp = fopen(compressFileName, "w");

    lerPalavrasDoArq(p, arqTxt);
    removerPalavrasDuplicadas(p);
    contaPalavras(p);
    printf("qtdPalavras: %d\n", qtdPalavras);
    criarCabecalho(p, arqCmp, arqTxt);

    printf("SUCESSO!\n");

    fclose(arqCmp);
    fclose(arqTxt);
}

char *getArqName(char *name)
{
    char *arqName;
    int i;
    for (i = 0; i < strlen(name); i++)
    {
        if (name[i] == '.')
            break;

        arqName[i] = name[i];
    }

    return arqName;
}

void descomprimir(char *arqCompressName)
{
    char *newArqName = getArqName(arqCompressName);
    FILE *arqCmp = fopen(arqCompressName, "r");
    FILE *arqTxt = fopen(newArqName, "r");
}

int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        if (strcmp(argv[1], "-c") == 0)
        {
            comprimir(argv[2]);
        }
        else if (strcmp(argv[1], "-d") == 0)
        {
            descomprimir(argv[2]);
        }
    }
    else
    {
        printf("Numero de parametros insuficiente...\n");
    }

    return 0;
}
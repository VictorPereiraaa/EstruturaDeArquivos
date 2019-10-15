/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <string.h>

//Variaveis Globais
int qtdPalavras = 0;

struct Palavras {
    char palavra[50];
};


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

void listarPalavras() {
    FILE * arqTxt;
    FILE * arqCmp;
    char c[50];
    int i = 0, j = 0;
    struct Palavras p[100];
    
    //abre o txt para leitura e o cmp para escrita
    arqTxt = fopen("arquivotexto.txt", "r");
    arqCmp = fopen("arquivoCompactado.txt", "a+");
    
    contaPalavras(arqTxt);


    //joga todas as palavras na struct
    while (j < qtdPalavras) {

        //pega tudo ate achar um espaço
        fscanf(arqTxt, "%[^ ]", c);
        //pula uma posição na leitura
        fseek(arqTxt, +1, SEEK_CUR);

        strcpy(p[j].palavra, c);
        j++;
    }

    j = 1;
    
    //exclui as repetidas
    //***funcionando parcialmente***///
    
    while (i != qtdPalavras) {
        while (j != qtdPalavras) {
            
            if (strcmp(p[i].palavra, p[j].palavra) == 0) {
                p[j] = p[j + 1];
                j++;
            } else {
                j++;
            }
        }
        i++;
        j = 0;
    }
    
    //excluir as repetidas

    //atribuir o RRN

    //printar no arqCmp


    for (int k = 0; k < qtdPalavras; k++) {
        printf("%d.%s ", k, p[k].palavra);
    }


    fclose(arqTxt);
}

int main() {

    cabecalho();
    listarPalavras();
    printf("DEU BOM!!!\n");

    return 0;
}
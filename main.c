/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <string.h>

struct Palavras {
    char palavra[50];
};

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
    arq = fopen("arquivoCompactado.txt.cmp", "w");
    fprintf(arq, "%c%c", byte1, byte2);

    fclose(arq);

}

void listarPalavras() {
    FILE * arqTxt;
    FILE * arqCmp;
    char c[50];
    char aux;
    int i = 0, j = 0;
    struct Palavras p[100];
    int contPalavras = 0;
    
    
    //abre o txt para leitura e o cmp para escrita
    arqTxt = fopen("arquivotexto.txt", "r");
    arqCmp = fopen("arquivoCompactado.txt.cmp", "w");

    //conta as palavras do arquivo txt
    while (!feof(arqTxt)) {
        aux = fgetc(arqTxt);
        if (aux == ' ')
            contPalavras++;
        if (aux == '\n' || aux == '\0')
            contPalavras++;
    }
    rewind(arqTxt);
    
    //joga todas as palavras na struct
    while (j < contPalavras) {
        //pega tudo ate achar um espaço
        fscanf(arqTxt, "%[^ ]", c);
        //pula uma posição na leitura
        fseek(arqTxt, +1, SEEK_CUR);

        strcpy(p[i].palavra, c);
        i++;
        j++;
    }

    i = 0;
    j = 1;
    
    //exclui as repetidas
    //***funcionando parcialmente***///
    while (i != contPalavras) {
        while (j != contPalavras) {
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

    for (int k = 0; k < contPalavras; k++) {
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
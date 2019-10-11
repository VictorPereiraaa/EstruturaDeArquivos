/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>



struct Palavras {
    char palavra[30];
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
    char c;
    int i = 0, j = 0;
    struct Palavras p[100];

    //abre o txt para leitura e o cmp para escrita
    arqTxt = fopen("arquivotexto.txt", "r");
    arqCmp = fopen("arquivoCompactado.txt", "w");

    //joga todas as palavras na struct
    //**implementar: tratar a vírgula e o ponto
    while (!feof(arqTxt)) {
        c = fgetc(arqTxt);
        if (c != ' ') {
            p[j].palavra[i] = c;
            i++;
        } else {
            j++;
            i = 0;
        }
    }

    //excluir as repetidas
    
    //atribuir o RRN
    
    //printar no arqCmp


    for (int k = 0; k < 10; k++) {
        printf("%d.%s", k, p[k].palavra);
    }


    fclose(arqTxt);
}

int main() {

    cabecalho();
    listarPalavras();
    printf("DEU BOM!!!\n");

    return 0;
}
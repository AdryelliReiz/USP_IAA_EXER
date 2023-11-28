//NOME: Adryelli Reis dos Santos
//NUSP: 14714019

#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FILENAME 256

struct quadrante {
    int cor;
    int maximal;
};


struct quadrante topR, topL, bottomR, bottomL;

//REFAZER PQ É A APENAS PARA CONTAR O MAIOR QUADRANTE OS DENTRO DELE NÃO
// Função para contar os quadrantes maximais totalmente brancos e pretos na matriz e gerar a saída
// Modificação: Adicionei as dimensões da matriz na assinatura da função
struct quadrante contarQuadrantes(int dimensao, int expDimensao, int matriz[dimensao][dimensao], int linha, int coluna, int matrizResposta[expDimensao][3]) {
    struct quadrante q_max;
    printf("OUTRA VEZ\n");

    for(int i = 0; i < 2; i ++) {
        for(int j = 0; j < 2; j ++) {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }

    if(dimensao == 1) {
        if(matriz[linha][coluna] == 0) {
            q_max.cor = 0;
        } else {
            q_max.cor = 1;
        }
        q_max.maximal = 1;

    } else {
        int tamanho = dimensao / 2;

        //O PROBLEMA TÁ AQUI!!!!! AAAAAAAAAAAAAAAAAHHHHHH
        int expDimensaoInterno = expDimensao - 1;

        topL = contarQuadrantes(tamanho, expDimensaoInterno, matriz, linha, coluna, matrizResposta);
        topR = contarQuadrantes(tamanho, expDimensaoInterno, matriz, linha, coluna + tamanho, matrizResposta);
        bottomL = contarQuadrantes(tamanho, expDimensaoInterno , matriz, linha + tamanho, coluna, matrizResposta);
        bottomR = contarQuadrantes(tamanho, expDimensaoInterno, matriz, linha + tamanho, coluna + tamanho, matrizResposta);

        //printf("DIMANSÃO INDEX 01: %d\n", expDimensao);
        /* IGNORA POR HORA
        if(topR.cor == topL.cor && topL.cor == bottomR.cor && bottomR.cor == bottomL.cor) {
            q_max.cor = topR.cor;
            q_max.maximal = 1;
        } else {
            //fazer para cada caso de quadrante cima/baixo/direita/esquerda
            if(topR.maximal == 1) {
                if(topR.cor == 0) matrizResposta[expDimensao][2] += 1;
                else if (topR.cor == 1) matrizResposta[expDimensao][1] += 1;
            }
            if(topL.maximal == 1) {
                if(topL.cor == 0) matrizResposta[expDimensao][2] += 1;
                else if (topL.cor == 1) matrizResposta[expDimensao][1] += 1;
            }
            if(bottomR.maximal == 1) {
                if(bottomR.cor == 0) matrizResposta[expDimensao][2] += 1;
                else if (bottomR.cor == 1) matrizResposta[expDimensao][1] += 1;
            }
            if(bottomL.maximal == 1) {
                if(q_max.maximal == 0) matrizResposta[expDimensao][2] += 1;
                else if (bottomL.cor == 1) matrizResposta[expDimensao][1] += 1;
            }
            printf("DIMANSÃO INDEX 02: %d\n", expDimensao);
            q_max.maximal = 0;
        }
        */
    }

    //printf("OQ TEM NO Q_MAX: %d | %d\n", q_max.cor, q_max.maximal);

    return q_max;

}

int main (int argumentos, char* argumentosValores[]) {
    //verificaçao se tem os argumentos necessarios
    if (argumentos != 3){
        printf("Este programa exige dois argumentos: número que representa a dimensão da matriz e o arquivo de entrada da matriz.\n"); /* COMPLETAR */
        exit(1);
    }

    //lendo o arquivo da matriz

    char filename[MAX_FILENAME] = "";
    strcpy(filename, argumentosValores[2]);
    FILE* fp = fopen(filename, "r");
    if (fp == NULL){
        fprintf(stderr, "Não foi possível abrir o arquivo %s.\n", filename);
        return -1;
    }

    //dimensão da matriz
    int dimensao = atoi(argumentosValores[1]);
    //matriz NxN (exponencial de base 2)
    int matriz[dimensao][dimensao];
    int inteiro = 0;
    int expDimensao = 0;
    int aux = dimensao;

    while(aux > 0) {
        aux /= 2;
        expDimensao++;
    }

    //TUDO OK
    //printf("ExpDimensao: %d\n", expDimensao);
    int matrizResposta[expDimensao][3];
    int valorED = 1;

    for(int i = 0; i < expDimensao; i++) {
        //adiciona os valores das dimensões dos quadrantes a matriz
        matrizResposta[i][0] = valorED;
        matrizResposta[i][1] = 0;
        matrizResposta[i][2] = 0;
        valorED = valorED * 2;

        //TUDO OK
        //printf("valor dentro da matriz: %d\n", matrizResposta[i][0]);
    }


    //loop para copiar os valores de entrada para a matriz e para os valores iniciais de matrizRepostas
    for(int i = 0; i < dimensao; i++) {
        for(int j = 0; j < dimensao; j++) {
            if (fscanf(fp, "%1d", &matriz[i][j]) != 1) {
                
                printf("Erro na leitura do arquivo.\n");
                fclose(fp);
                return 1;
            }
        }
    }

    for(int i = 0; i < 2; i ++) {
            for(int j = 0; j < 2; j ++) {
                printf("%d ", matriz[i][j]);
            }
            printf("\n");
        }

    expDimensao = expDimensao - 1;

    FILE *arquivoSaida = fopen("saida.txt", "w");

    printf("%d\n",expDimensao);

    struct quadrante quadranteNxN = contarQuadrantes(dimensao, expDimensao, matriz, 0, 0, matrizResposta);

    if(topR.cor == topL.cor && topL.cor == bottomR.cor && bottomR.cor == bottomL.cor) {
        if(topR.cor == 0) matrizResposta[expDimensao][2] += 1;
        else if (topR.cor == 1) matrizResposta[expDimensao][1] += 1;
        printf("tá marcando errado: %d %d %d %d\n", topR.cor, topL.cor, bottomR.cor, bottomL.cor);
    } else {
        //fazer para cada caso de quadrante cima/baixo/direita/esquerda
        if(topR.maximal == 1) {
            if(topR.cor == 0) matrizResposta[expDimensao][2] += 1;
            else if (topR.cor == 1) matrizResposta[expDimensao][1] += 1;
            topR.maximal = 0;
        }
        if(topL.maximal == 1) {
            if(topL.cor == 0) matrizResposta[expDimensao][2] += 1;
            else if (topL.cor == 1) matrizResposta[expDimensao][1] += 1;
            topL.maximal = 0;
        }
        if(bottomR.maximal == 1) {
            if(bottomR.cor == 0) matrizResposta[expDimensao][2] += 1;
            else if (bottomR.cor == 1) matrizResposta[expDimensao][1] += 1;
            bottomR.maximal = 0;
        }
        if(bottomL.maximal == 1) {
            if(bottomL.cor == 0) matrizResposta[expDimensao][2] += 1;
            else if (bottomL.cor == 1) matrizResposta[expDimensao][1] += 1;
        }
    }


    for(int i = expDimensao; i >= 0; i--) {
        printf("%dx%d %d %d\n", matrizResposta[i][0], matrizResposta[i][0], matrizResposta[i][1], matrizResposta[i][2]);
        fprintf(arquivoSaida, "%dx%d %d %d\n", matrizResposta[i][0], matrizResposta[i][0], matrizResposta[i][1], matrizResposta[i][2]);
    }

    return 0;
}
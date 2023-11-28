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

//função recursiva
struct quadrante contarQuadrantes(int dimensaoMax,int dimensao, int expDimensao, int **matriz, int linha, int coluna, int **matrizResposta) {
    struct quadrante q_max;

    //lê o pixel na dimensão 1x1
    if(dimensao == 1) {
        if(matriz[linha][coluna] == 0) {
            q_max.cor = 0;
        } else {
            q_max.cor = 1;
        }
        q_max.maximal = 1;

    } else {
        int tamanho = dimensao / 2;

        //dimensão exponencial de base 2 (1x1(0), 2x2(1), 4x4(2), 8x8(2))
        //para salvar na lista, por index, a dimensão a qual o quadrante pertence
        expDimensao = expDimensao - 1;

        struct quadrante topL = contarQuadrantes(dimensaoMax, tamanho, expDimensao, matriz, linha, coluna, matrizResposta);
        struct quadrante topR = contarQuadrantes(dimensaoMax, tamanho, expDimensao, matriz, linha, coluna + tamanho, matrizResposta);
        struct quadrante bottomL = contarQuadrantes(dimensaoMax, tamanho, expDimensao , matriz, linha + tamanho, coluna, matrizResposta);
        struct quadrante bottomR = contarQuadrantes(dimensaoMax, tamanho, expDimensao, matriz, linha + tamanho, coluna + tamanho, matrizResposta);
        
        //caso os quadrantes sejam iguais: 
        if(topR.cor == topL.cor && topL.cor == bottomR.cor && bottomR.cor == bottomL.cor) {
            //se for o maior quadrante: finaliza adicionando como maior quadrante NxN
            if(dimensaoMax == dimensao) {
                if(topR.cor == 0) matrizResposta[expDimensao + 1][2] += 1;
                else if (topR.cor == 1) matrizResposta[expDimensao + 1][1] += 1;
            } 
            //se não for: apenas salva as informações do quadrante atual
            else {
                q_max.cor = topR.cor;
                q_max.maximal = 1;
            }
            
        } 
        //caso os quadrantes sejam diferentes (juntos não são maximal): é necessário registrar o quadrente
        //verifica se cada area do quadrante já é um maximal ou não
        //registra cada parte do quadrante pela sua cor
        else {
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
                if(bottomL.cor == 0) matrizResposta[expDimensao][2] += 1;
                else if (bottomL.cor == 1) matrizResposta[expDimensao][1] += 1;
            }
            //registra quadrante como "misto" (mistura do zeros "0" e ums "1")
            q_max.cor = -1;
            //registra como quadrante "não maximal"
            q_max.maximal = 0;
        }

    }
    
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

    //alocando memoria para matriz

    //matriz NxN (exponencial de base 2)
    int **matriz = (int **)malloc(dimensao * sizeof(int *));

    for (int i = 0; i < dimensao; i++) {
        matriz[i] = (int *)malloc(dimensao * sizeof(int));
    }

    int inteiro = 0;
    int expDimensao = 0;
    int aux = dimensao;

    while(aux > 0) {
        aux /= 2;
        expDimensao++;
    }

    //matriz que contém a resposta passada para cada recursão da função
    //salvando a quantidade de quadrantes maximais da matriz

    int valorED = 1;
    int **matrizResposta = (int **)malloc(expDimensao * sizeof(int *));
    for (int i = 0; i < expDimensao; i++) {
        matrizResposta[i] = (int *)malloc(sizeof(int));
        matrizResposta[i][0] = valorED;
        matrizResposta[i][1] = 0;
        matrizResposta[i][2] = 0;
        valorED = valorED * 2;
    }


    //loop para copiar os valores de entrada para a matriz
    for(int i = 0; i < dimensao; i++) {
        for(int j = 0; j < dimensao; j++) {
            if (fscanf(fp, "%1d", &matriz[i][j]) != 1) {
                
                printf("Erro na leitura do arquivo.\n");
                fclose(fp);
                return 1;
            }
        }
    }

    expDimensao = expDimensao - 1;

    //criando o arquivo de saida

    FILE *arquivoSaida = fopen("saida.txt", "w");

    struct quadrante quadranteNxN = contarQuadrantes(dimensao, dimensao, expDimensao, matriz, 0, 0, matrizResposta);

    for(int i = expDimensao; i >= 0; i--) {
        fprintf(arquivoSaida, "%dx%d %d %d\n", matrizResposta[i][0], matrizResposta[i][0], matrizResposta[i][1], matrizResposta[i][2]);
    }

    return 0;
}
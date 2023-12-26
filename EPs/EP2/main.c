#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>

#define MAX_FILENAME 256
#define MAX_ATUALIZATIONS 100
#define MAX_HOURS 20

typedef struct atualizatios_T {
    int duration;
    int index; 
} AT;


//função de ordenação merge
void merge(AT v[], int start, int middle, int end) {
    int lMax = middle - start + 1;
    int rMax = end - middle;

    AT vLeft[lMax];
    AT vRight[rMax];

    // copia o array para os vetores auxiliares
    for(int i = 0; i < lMax; i++) {
        vLeft[i] = v[start + i];
    }
    for(int i = 0; i < rMax; i++) {
        vRight[i] = v[middle + 1 + i];
    }

    int i = 0, j = 0, k = start;

    // combina os arrays temporários e joga os valores para o array original
    while(i < lMax && j < rMax) {
        if(vLeft[i].duration <= vRight[j].duration) {
            v[k] = vLeft[i];
            i++;
        } else  {
            v[k] = vRight[j];
            j++;
        }
        k++;
    }

    // copia os elementos restantes de L[], se houver
    while (i < lMax) {
        v[k] = vLeft[i];
        i++;
        k++;
    }

    // copia os elementos restantes de R[], se houver
    while (j < rMax) {
        v[k] = vRight[j];
        j++;
        k++;
    }
}

void mergeSort(AT v[], int start, int end) {
    if(start < end) {
        int middle = ((end - start) / 2) + start;

        mergeSort(v, start, middle);
        mergeSort(v, middle + 1, end);

        merge(v,start, middle, end);
    }
}


//função para o primeiro caso de execução do algoritmo
int caseOne (AT v[], int n_v, int duration, int atualizations[]) {
    //recebe array ordenado

    int i = 0;
    while(i < n_v && v[i].duration <= duration) {
        duration -= v[i].duration;
        atualizations[i] = v[i].index;
        i++;
    }

    return i;
}

void swap(AT *a, AT *b) {
    AT temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(AT v[], int n, int i) {
    //mantém a propriedade de min-heap
    int smallest = i;
    int leftChild = 2 * i + 1;
    int rightChild = 2 * i + 2;

    if (leftChild < n && v[leftChild].duration < v[smallest].duration)
        smallest = leftChild;

    if (rightChild < n && v[rightChild].duration < v[smallest].duration)
        smallest = rightChild;

    if (smallest != i) {
        swap(&v[i], &v[smallest]);
        minHeapify(v, n, smallest);
    }
}

void buildMinHeap(AT v[], int n) {
    //constroi um min-heap
    for (int i = (n - 1) / 2; i >= 0; i--) {
        minHeapify(v, n, i);
    }
}

//encontra o elemento de atualizações da lista pelo index
int findAtualization(AT v[], int i_n_v, int n_v, int ch_index) {
    for(int i_v = i_n_v; i_v < n_v; i_v++) {
        if(v[i_v].index == ch_index) return i_v;
    }

    return -1;
}

void heapifyUp(AT v[], int i) {
    while (i > 0 && v[(i - 1) / 2].duration > v[i].duration) {
        swap(&v[i], &v[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

//atualiza o vetor caso tenha alguma operação pendente a ser feita
void operateUpdates(AT v[], int i_n_v, int* n_v, int time, int changes[100][3], int n_c, int* changes_index, int includes[100][3], int n_i, int* includes_index) {
    int hasUpdated = false;
    //para alteração na duração
    int i = *changes_index;
    while(i < n_c && changes[i][0] <=  time) {
        if(changes[i][0] == time) {
            int i_operation =  findAtualization(v, i_n_v, *n_v, changes[i][1]);
            //caso existe alguma mudança a ser feita... faça
            if(i_operation) {
                v[i_operation].duration = changes[i][2];

                *changes_index = i + 1;
            }

            heapifyUp(v, i_operation);
            minHeapify(v, *n_v, 0);

        }
        i++;
    }

    int j = *includes_index;
    while(j < n_i && includes[j][0] <=  time) {
        if(includes[j][0] == time) {
            //caso existe alguma inclusão a ser feita... faça
            v[*n_v].index = includes[j][1];
            v[*n_v].duration = includes[j][2];
            *includes_index = j + 1;

            //corrigi o min-heap subindo
            heapifyUp(v, *n_v);
            minHeapify(v, *n_v, 0);
            *n_v += 1;
        }
        j++;
    }
}

//função para o primeiro caso de execução do algoritmo
int caseTwo(AT v[], int* n_v, int duration, int atualizations[], int changes[100][3], int n_c, int includes[100][3], int n_i) {
    //recebe array ordenado
    int changesFinishs = 0;
    int includesFinishs = 0;
    int timeLeft = duration;
    int i = 0;

    while(0 < *n_v < MAX_ATUALIZATIONS && v[0].duration <= timeLeft) {
        timeLeft -= v[0].duration;
        atualizations[i] = v[0].index;
        int time = (duration - timeLeft);
        i++;

        //extraindo o menor elemento
        swap(&v[0], &v[*n_v - 1]);
        *n_v -= 1;

        for(int j = 0; j <= time; j++) {
            operateUpdates(v, 0, n_v, j, changes, n_c, &changesFinishs, includes,n_i, &includesFinishs);
        }

        minHeapify(v, *n_v, 0);
    }

    return i;
}

int main(int args, char* argsValues[]) {
    //verifica os argumentos que o programa recebe
    if (args != 4){
        printf("Este programa exige dois argumentos: número que representa a duração em horas e o arquivo de entrada das atualizações.\n"); /* COMPLETAR */
        exit(1);
    }

    //lendo o arquivo

    char filename[MAX_FILENAME] = "";
    strcpy(filename, argsValues[3]);
    FILE* fp = fopen(filename, "r");
    if (fp == NULL){
        fprintf(stderr, "Não foi possível abrir o arquivo %s.\n", filename);
        return -1;
    }

    //duração da atualização
    int duration = atoi(argsValues[1]);
    //caso que o programa deve executar
    int case_ = atoi(argsValues[2]);
    //número de atualizações
    int n_atualizations = 0;
    fscanf(fp, "%d ", &n_atualizations);

    //lista das durações das atualizações com tamanho MAX
    AT *atualizations = (AT *)malloc(100 * sizeof(AT));

    int atualizations_resp[MAX_HOURS];
    int n_atualizations_resp = 0;

    if (atualizations == NULL) {
        fprintf(stderr, "Falha na alocação de memória\n");
        return 1; // Saída com erro
    }

    // Inicializar todos os valores com -1
    for (int i = 0; i < MAX_ATUALIZATIONS; i++) {
        atualizations[i].index = -1;
        atualizations[i].duration = -1;
    }

    FILE *fileExit;

    if (case_ == 1) fileExit = fopen("saida1.txt", "w");
    else fileExit = fopen("saida2.txt", "w");


    for(int i = 0; i < n_atualizations; i++) {
        fscanf(fp, "%d", &atualizations[i].duration);
        atualizations[i].index = i + 1;
    }

    switch(case_) {
        //algoritmo do caso 1
        case 1:
            mergeSort(atualizations, 0, n_atualizations - 1);

            n_atualizations_resp = caseOne(atualizations, n_atualizations, duration, atualizations_resp);

            fprintf(fileExit, "%d ", n_atualizations_resp);

            for(int i = 0; i < n_atualizations_resp; i++) {
                fprintf(fileExit, "%d ", atualizations_resp[i]);
            }

            break;

        //algoritmo do caso 2
    
        case 2:
            int max_operations = 100 - n_atualizations;
            int (*changes)[3] = malloc(max_operations * sizeof(int[3]));
            int (*includes)[3] = malloc(max_operations * sizeof(int[3]));

            //pegar informações das mudanças e novas atualizações
            int i_c = 0;
            int j_i = 0;
            char l;
            while (fscanf(fp, " %c", &l) == 1 && (i_c + j_i) < 100) {
                if (l == 'c' && i_c < max_operations) {
                    fscanf(fp, "%d %d %d", &changes[i_c][0], &changes[i_c][1], &changes[i_c][2]);
                    i_c++;
                } else if (l == 'i' && (i_c + j_i) < 100) {
                    fscanf(fp, "%d %d %d", &includes[j_i][0], &includes[j_i][1], &includes[j_i][2]);
                    j_i++;
                }
            }

            //construir heap
            buildMinHeap(atualizations, n_atualizations);

            n_atualizations_resp = caseTwo(atualizations, &n_atualizations, duration, atualizations_resp, changes, i_c, includes, j_i);

            fprintf(fileExit, "%d ", n_atualizations_resp);

            for(int i = 0; i < n_atualizations_resp; i++) {
                fprintf(fileExit, "%d ", atualizations_resp[i]);
            }
            
            break;
        
        default:
            break;
    }

    return 0;
}
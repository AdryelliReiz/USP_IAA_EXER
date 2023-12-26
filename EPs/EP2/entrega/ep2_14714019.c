//NOME: Adryelli Reis dos Santos
//NUSP: 14714019

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#define MAX_FILENAME 256
#define MAX_UPDATES 100
#define MAX_HOURS 20

typedef struct atualizatios_T {
    int duration;
    int index; 
} AT;

void merge(AT v[], int start, int middle, int end) {
    int lMax = middle - start + 1;
    int rMax = end - middle;

    AT vLeft[lMax];
    AT vRight[rMax];

    for(int i = 0; i < lMax; i++) {
        vLeft[i] = v[start + i];
    }
    for(int i = 0; i < rMax; i++) {
        vRight[i] = v[middle + 1 + i];
    }

    int i = 0, j = 0, k = start;

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

    while (i < lMax) {
        v[k] = vLeft[i];
        i++;
        k++;
    }

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

int caseOne (AT v[], int n_v, int duration, int updates[]) {
    int i = 0;
    while(i < n_v && v[i].duration <= duration) {
        duration -= v[i].duration;
        updates[i] = v[i].index;
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
    for (int i = (n - 1) / 2; i >= 0; i--) {
        minHeapify(v, n, i);
    }
}

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

void operateUpdates(AT v[], int i_n_v, int* n_v, int time, int changes[100][3], int n_c, int* changes_index, int includes[100][3], int n_i, int* includes_index) {
    int i = *changes_index;

    while(i < n_c && changes[i][0] <=  time) {
        if(changes[i][0] == time) {
            int i_operation =  findAtualization(v, i_n_v, *n_v, changes[i][1]);

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
            v[*n_v].index = includes[j][1];
            v[*n_v].duration = includes[j][2];
            *includes_index = j + 1;

            heapifyUp(v, *n_v);
            minHeapify(v, *n_v, 0);
            *n_v += 1;
        }
        j++;
    }
}

int caseTwo(AT v[], int* n_v, int duration, int updates[], int changes[100][3], int n_c, int includes[100][3], int n_i) {
    int changesFinishs = 0;
    int includesFinishs = 0;
    int timeLeft = duration;
    int i = 0;

    while(0 < *n_v < MAX_UPDATES && v[0].duration <= timeLeft) {
        timeLeft -= v[0].duration;
        updates[i] = v[0].index;
        int time = (duration - timeLeft);
        i++;

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
    if (args != 4){
        printf("Este programa exige dois argumentos: número que representa a duração em horas e o arquivo de entrada das atualizações.\n"); /* COMPLETAR */
        exit(1);
    }

    char filename[MAX_FILENAME] = "";
    strcpy(filename, argsValues[3]);
    FILE* fp = fopen(filename, "r");
    if (fp == NULL){
        fprintf(stderr, "Não foi possível abrir o arquivo %s.\n", filename);
        return -1;
    }

    int duration = atoi(argsValues[1]);
    int case_ = atoi(argsValues[2]);
    int n_updates = 0;
    fscanf(fp, "%d ", &n_updates);

    AT *updates = (AT *)malloc(100 * sizeof(AT));

    int updates_resp[MAX_HOURS];
    int n_updates_resp = 0;

    if (updates == NULL) {
        fprintf(stderr, "Falha na alocação de memória\n");
        return 1;
    }

    for (int i = 0; i < MAX_UPDATES; i++) {
        updates[i].index = -1;
        updates[i].duration = -1;
    }

    FILE *fileExit;

    if (case_ == 1) fileExit = fopen("saida1.txt", "w");
    else fileExit = fopen("saida2.txt", "w");


    for(int i = 0; i < n_updates; i++) {
        fscanf(fp, "%d", &updates[i].duration);
        updates[i].index = i + 1;
    }

    switch(case_) {
        case 1:
            mergeSort(updates, 0, n_updates - 1);

            n_updates_resp = caseOne(updates, n_updates, duration, updates_resp);

            fprintf(fileExit, "%d ", n_updates_resp);

            for(int i = 0; i < n_updates_resp; i++) {
                fprintf(fileExit, "%d ", updates_resp[i]);
            }

            break;
    
        case 2:
            int max_operations = 100 - n_updates;
            int (*changes)[3] = malloc(max_operations * sizeof(int[3]));
            int (*includes)[3] = malloc(max_operations * sizeof(int[3]));

            int i_c = 0;
            int j_i = 0;
            char l;
            while (fscanf(fp, " %c", &l) == 1 && (i_c + j_i) < 100) {
                if (l == 'c' && i_c < max_operations) {
                    fscanf(fp, "%d %d %d", &changes[i_c][0], &changes[i_c][1], &changes[i_c][2]);
                    i_c++;
                } else if (l == 'i' && (i_c + j_i) < 100 && (n_updates + j_i) < 100) {
                    fscanf(fp, "%d %d %d", &includes[j_i][0], &includes[j_i][1], &includes[j_i][2]);
                    j_i++;
                }
            }

            buildMinHeap(updates, n_updates);

            n_updates_resp = caseTwo(updates, &n_updates, duration, updates_resp, changes, i_c, includes, j_i);

            fprintf(fileExit, "%d ", n_updates_resp);

            for(int i = 0; i < n_updates_resp; i++) {
                fprintf(fileExit, "%d ", updates_resp[i]);
            }

            free(changes);
            free(includes);
            
            break;
        
        default:
            break;
    }

    free(updates);
    fclose(fp);
    fclose(fileExit);
    
    return 0;
}
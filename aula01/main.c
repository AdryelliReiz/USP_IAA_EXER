#include <stdio.h>
#include <stdbool.h>

//busca um elemento dentro de uma lista sequencial
int buscaSequencial(int* array, int n, int size ) {
    int i;

    for(i = 0; i < size; i++) {
        if(array[i] == n) return i;
    }

    return -1;
}

//ToDo: fazer um código de Busca Binária

//ToDo: fazer um código de Busca Insertion sort (inserção direta)

//buscar maior número de um array
int max(int v[], int n) {
    int j, x;
    x = v[0];
    //prova de corretude por indução
    //propriedade invariante
    // x é um elemento máximo de v[0..j-1]
    for(j= 1; j < n; j++) {
        if(x < v[j]) x = v[j];
    }

    return x;
}
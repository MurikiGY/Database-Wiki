#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "hash.h"

//Funcao do qsort
static int compare(const void *p1, const void *p2){
    return (((print_t *)p1)->chave - ((print_t *)p2)->chave);
}

int hash1(int k){
    return k % M;
}

int hash2(int k){
    return floor(M*(k*0.9 - floor(k*0.9)));
}

void busca_hash(hash_t T1[], hash_t T2[], int k, int *T, int *pos){
    int index1 = hash1(k);
    int index2 = hash2(k);

    *T = 0;
    *pos = -1;
        
    //Testa T1 vazio
    if (T1[index1].status == VAZIO)
        return;

    //Testa T1 cheio
    if (T1[index1].status == CHEIO && T1[index1].chave == k){

        *T = 1;
        *pos = index1;

    //T1 excluido ou cheio e sem a chave, Testa T2
    } else if (T2[index2].status == CHEIO && T2[index2].chave == k){

        *T = 2;
        *pos = index2;

    }
}

void insere_hash(hash_t T1[], hash_t T2[], int k){
    int T, index1, index2;

    //Busca chave duplicada
    busca_hash(T1, T2, k, &T, &index1);

    //Se não ha chave duplicada
    if (T)
        return;

    //Se não ha chave duplicada
    index1 = hash1(k);
    if (T1[index1].status == CHEIO){
        index2 = hash2(T1[index1].chave);
        T2[index2].chave = T1[index1].chave;
        T2[index2].status = CHEIO;
    }

    T1[index1].chave = k;
    T1[index1].status =  CHEIO;    
    
}

void remove_hash(hash_t T1[], hash_t T2[], int k){
	int T, index;

    busca_hash(T1, T2, k, &T, &index);

    //Se encontrou
    if (T)
        if (T == 1)
            T1[index].status = EXCLUIDO;
        else
        if (T == 2)
            T2[index].status = EXCLUIDO;

}

void imprime_hash(hash_t T1[], hash_t T2[]){
    int i, j;
    print_t dataArr[2*M];

    j = 0;  //Contador do dataArr
    //Loop de inserção de dados
    for (i=0; i<M ;i++)
        if (T1[i].status == CHEIO){
            dataArr[j].chave = T1[i].chave;
            dataArr[j].index = i;
            dataArr[j].table = 1;
            j++;
        }
    for (i=0; i<M ;i++)
        if (T2[i].status == CHEIO){
            dataArr[j].chave = T2[i].chave;
            dataArr[j].index = i;
            dataArr[j].table = 2;
            j++;
        }

    //Ordenação dos dados
    qsort(dataArr, j, sizeof(print_t), compare);

    //Impressão dos dados
    for (i=0; i<j ;i++)
        printf("%d,T%d,%d\n", dataArr[i].chave, dataArr[i].table, dataArr[i].index);

}

void zera_hash(hash_t T[]){
    int i;

    for(i=0; i<M ;i++)
        T[i].status = 0;
}

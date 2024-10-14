#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"

int main (){
    char    oper;               // tipo de operacao
    int     valor;              // Valor da operacao
    hash_t  T1[M];              // Vetor de tabela 1
    hash_t  T2[M];              // Vetor de tabela 2

    int T;
    int pos;

    // Configura status das tabelas como vazio
    zera_hash(T1);
    zera_hash(T2);
    
    while(scanf("%c %d", &oper, &valor) != EOF){

        if (oper == 'i')
            insere_hash(T1, T2, valor);

        else if (oper == 'r')
            remove_hash(T1, T2, valor);

        else if (oper == 'b'){
            busca_hash(T1, T2, valor, &T, &pos);
            if (T)
                printf("O valor %d esta na tabela %d, posicao %d\n",valor,T,pos);
            else
                printf("O valor %d não foi encontrado\n", valor);
        }

        getchar();
    }

    imprime_hash(T1, T2);

    return 0;
}


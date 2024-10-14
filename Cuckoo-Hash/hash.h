#ifndef _AUX_
#define _AUX_

#define M 11
#define VAZIO 0
#define EXCLUIDO 1
#define CHEIO 2

struct hash {
    int chave;
    int status;
};
typedef struct hash hash_t;

struct print {
    int chave;
    int index; //Posição na tabela hash
    int table; //Tabela
};
typedef struct print print_t;

/* Status:
 * 0 - Posição vazia e nao excluido
 * 1 - Posição vazia e excluido
 * 2 - Posição com valor
 */

void insere_hash(hash_t T1[], hash_t T2[], int k);

void remove_hash(hash_t T1[], hash_t T2[], int k);

void busca_hash(hash_t T1[], hash_t T2[], int k, int *T, int *pos);

void imprime_hash(hash_t T1[], hash_t T2[]);

void zera_hash(hash_t T[]);

#endif

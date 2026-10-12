/* Bibliotecas usadas */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/* Header do módulo */
#include "../include/linear_algebra.h"

/* Facilita a liberação da memória para o cliente */
void free_vector(Vector *address){
    if (address == NULL) return;

    // Libera a memória alocada para o vetor e exclui a referencia do endereço passado
    free(address->values);
    address->values = NULL;
    address->len = 0;

    return;
}

void free_matrix(Matrix *address){
    if (address == NULL) return;

    if (address->values == NULL) return;

    // Libera a memória de cada linha
    for (int i = 0; i < address->rlen; i++){
        free(address->values[i]);
    }

    // Libera a memória alocada para os ponteiros de linha e exclui a referencia do endereço passado
    free(address->values);
    address->values = NULL;
    address->rlen = 0;
    address->clen = 0;

    return;
}

/* Utilitários */
// Faz uma deep copy de uma matriz
Matrix copy_matrix(Matrix a){
    Matrix resultado = {NULL, 0, 0};
    if (a.values == NULL || a.rlen <= 0 || a.clen <= 0) {
            return resultado;
    }

    int m = a.rlen;
    int n = a.clen;

    //Aloca o array externo da matriz
    resultado.values = malloc(m * sizeof *resultado.values);
    if (resultado.values == NULL) { 
        return resultado; //Verifica se o malloc funcionou
    }

    //Aloca as arrays internas da matriz
    for (int i = 0; i < m; i++){
        resultado.values[i] = malloc(n * sizeof *resultado.values[i]);
        //Verifica se o malloc funcionou, caso não, libera memória previamente alocada
        if (resultado.values[i] == NULL){
            while (i > 0){ //faz um "backtracking" dos arrays internos alocados e libera
                i--;
                free(resultado.values[i]);
            }
            free(resultado.values); //libera as arryas externas
            resultado.values = NULL;
            return resultado;
        }
    }

    resultado.rlen = m;
    resultado.clen = n;

    //Copia os elementos
    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){
            resultado.values[i][j] = a.values[i][j];
        }
    }

    return resultado;
}

/* Operações */
// Cria um vetor com 2^n entradas, uma para cada combinação possível de n bits
Vector create_vector(int n){
    int vecSize = 1 << n;

    Vector v = {NULL, 0};

    v.values = calloc(vecSize, sizeof *v.values); // Inicializa o vetor com 0 em todas as entradas
    if (v.values == NULL) return v;
    v.len = vecSize;

    return v;
}

// Realiza o produto tensorial entre 2 vetores
Vector tensor_product_vec(Vector a, Vector b){
    Vector result = {NULL, 0};

    result.values = calloc((a.len * b.len), sizeof *result.values);
    if (result.values == NULL) return result;
    result.len = a.len * b.len;

    for (int i = 0; i < a.len; i++){
        for (int j = 0; j < b.len; j++){
            result.values[i*b.len + j] = complex_mul(a.values[i], b.values[j]); // Realiza o produto entre cada entrada de a com o vetor b inteiro
        }
    }

    return result;
}

// Realiza o produto tensorial entre 2 matrizes
Matrix tensor_product_matrix(Matrix a, Matrix b){
    int m = a.rlen;
    int n = a.clen;

    int p = b.rlen;
    int q = b.clen;

    Matrix result = {NULL, 0, 0};

    result.values = malloc((m * p) * sizeof *result.values);
    if (result.values == NULL) return result;

    for (int i = 0; i < m*p; i++){
        result.values[i] = calloc((n * q), sizeof *result.values[i]);

        // Libera as linhas que já foram preenchidas e o espaço alocado para os ponteiros de linha
        if (result.values[i] == NULL){
            while (i != 0){
                i--;
                free(result.values[i]);
            }

            free(result.values);
            return result;
        }
    }

    result.rlen = m*p;
    result.clen = n*q;

    for (int ia = 0; ia < m; ia++){
        for (int ja = 0; ja < n; ja++){
            for (int ib = 0; ib < p; ib++){
                for (int jb = 0; jb < q; jb++){
                    result.values[ia*p + ib][ja * q + jb] = complex_mul(a.values[ia][ja], b.values[ib][jb]); // Realiza o produto entre cada entrada de a com a matriz b inteira
                }
            }
        }
    }

    return result;
}

Matrix matrix_scale_mul(Matrix a, Complex alfa){
    // Multiplica cada elemento pelo escalar usando complex_mul
        for (int i = 0; i < a.rlen; i++) {
            for (int j = 0; j < a.clen; j++) {
                a.values[i][j] = complex_mul(a.values[i][j], alfa);
            }
        }

        return a;
}

// Realiza o produto entre 2 matrizes
Matrix matrix_mul(Matrix a, Matrix b){
    int m = a.rlen;
    int n = a.clen;

    int p = b.rlen;
    int q = b.clen;

    Matrix result = {NULL, 0, 0};

    // Checa a condição do produto de matrizes: número de colunas de a = número de linhas de b
    if (n != p){
        printf("ERRO: As matrizes não cumprem a condição do produto de matriz(col a = lin b)\n");
        return result;
    }

    result.values = malloc((m * p) * sizeof *result.values);
    if (result.values == NULL) return result;

    for (int i = 0; i < m*p; i++){
        result.values[i] = calloc((n * q), sizeof *result.values[i]);

        // Libera as linhas que já foram preenchidas e o espaço alocado para os ponteiros de linha
        if (result.values[i] == NULL){
            while (i != 0){
                i--;
                free(result.values[i]);
            }

            free(result.values);
            return result;
        }
    }

    result.rlen = m;
    result.clen = q;

    for (int ia = 0; ia < m; ia++){
        for (int jb = 0; jb < q; jb++){
            for (int ja = 0; ja < n; ja++){
                // Faz o produto matricial pela definição de combinação linear das linhas de a com as colunas de b
                result.values[ia][jb] = complex_add(result.values[ia][jb], complex_mul(a.values[ia][ja], b.values[ja][jb]));
            }
        }
    }

    return result;
}

// Realiza o produto matriz-vetor, definindo o vetor v como vetor coluna
Vector matrix_vec_mul(Matrix a, Vector v){
    int m = a.rlen;
    int n = a.clen;

    int c = v.len;

    Vector result = {NULL, 0};

    // Checa a condição do produto matriz-vetor: número de colunas de a = número de colunas de v
    if (n != c){
        printf("ERRO: A matriz não cumpre a condição do produto de matriz-vetor(col a = v)\n");
        return result;
    }

    result.values = malloc(m * sizeof *result.values);
    if (result.values == NULL) return result;

    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){
            // Faz o produto matriz-vetor usando a definição da combinação linear
            result.values[i] = complex_add(result.values[i], complex_mul(a.values[i][j], v.values[j]));
        }
    }

    return result;
}
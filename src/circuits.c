#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "complex.h"
#include "linear_algebra.h"
#include "gates.h"
#include "circuits.h"

// Define a seed para o random
void generate_seed(){
    srand(time(NULL));
}

// Inicializa n qubits no estado |00...0>
Vector init_qubits(int n) {
    Vector erro = {NULL, 0};
    if (n <= 0) {
        printf("ERRO: n deve ser positivo\n");
        return erro;
    }

    // Um estado de n qubits tem 2^n amplitudes
    Vector state = create_vector(1 << n);
    if (state.values == NULL) {
        printf("ERRO: falha na alocação dinâmica\n");
        return erro;
    }

    // |00...0> tem amplitude 1 no índice 0 (probabilidade 100% de medir 0)
    // create_vector já inicializa todos os valores com 0
    state.values[0] = complex_new(1.0, 0.0);

    return state;
}

// Aplica uma porta (matriz) a um estado
void apply_gate(Vector *s, Matrix gate) {
    if (s == NULL || s->values == NULL || gate.values == NULL){
        if (gate.values != NULL) free_matrix(&gate);
        return;
    }

    // A porta precisa ser 2^n x 2^n, o mesmo tamanho do vetor de amplitudes
    // Note que s.amplitudes.len sempre é uma potência de 2
    if (gate.rlen != s->len || gate.clen != s->len) {
        free_matrix(&gate);
        printf("ERRO: a porta deve ter dimensão 2^n x 2^n, com n = n_qubits\n");
        return;
    }

    // Novo estado = porta * estado
    Vector result = matrix_vec_mul(gate, *s);
    free_matrix(&gate);
    if (result.values == NULL) {
        printf("ERRO: falha na alocação dinâmica");
        return;
    }

    free_vector(s);
    *s = result;
}

// Mede o estado sem colapsá-lo, retornando o índice do estado base observado
int measure(const Vector *s) {
    if (s == NULL || s->values == NULL) return -1;

    // Sorteia r em [0, 1)
    // Como 0 <= rand() <= RAND_MAX, dividir por RAND_MAX +1 garante que r < 1
    double r = rand() / ((double) RAND_MAX + 1.0);

    double acumulado = 0.0;
    int ultimo_possivel = -1;

    // Soma as probabilidades até ultrapassar r: o estado em que isso acontece é o medido
    for (int i = 0; i < s->len; i++) {
        double prob = complex_mod2(s->values[i]);
        if (!(prob > 0.0)) continue; // Se a probabilidade for nula ou negativa, vai pra próxima iteração

        acumulado += prob;
        ultimo_possivel = i;

        if (r < acumulado) return i;
    }

    // Por erros de arredondamento, a soma de todas as probabilidades pode ser ligeiramente menor que 1;
    // assim, garante-se que essa função sempre retorne algum estado ao chegar nesse laço for
    return ultimo_possivel;
}

int *measure_shots(const Vector *state, int shots){
    if (state == NULL || state->values == NULL || shots < 0) return NULL;

    int *result = calloc(state->len, sizeof(int));
    if (result == NULL) return NULL;

    for (int i = 0; i < shots; i++){
        // Mede o estado para o shot atual
        int measured = measure(state);
        if (measured < 0){
            free(result);
            return NULL;
        }

        // No índice que foi observado, acrescenta uma medição
        result[measured]++;
    }

    return result;
}

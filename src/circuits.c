#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/complex.h"
#include "../include/linear_algebra.h"
#include "../include/gates.h"
#include "../include/circuits.h"
 
// Inicializa n qubits no estado |00...0>
QuantumState init_qubits(int n) {
    QuantumState erro = {{NULL, 0}, 0};
    if (n <= 0) {
        printf("ERRO: n deve ser positivo\n");
        return erro;
    }
 
    // Um estado de n qubits tem 2^n amplitudes
    Vector amplitudes = create_vector(pow(2, n));
    if (amplitudes.values == NULL) {
        printf("ERRO: falha na alocação dinâmica\n")
        return erro;
    }
 
    // |00...0> tem amplitude 1 no índice 0 (probabilidade 100% de medir 0)
    // create_vector já inicializa todos os valores com 0
    amplitudes.values[0] = complex_new(1.0, 0.0);
    QuantumState s = {amplitudes, n};
    return s;
}
 
// Libera a memória do estado e exclui a referência do endereço passado
void free_QuantumState(QuantumState *address) {
    if (address == NULL) return;
    free_vector(&address->amplitudes);
    address->n_qubits = 0;
    return;
}

// Aplica uma porta (matriz) a um estado, devolvendo um novo estado
QuantumState apply_gate(QuantumState s, Matrix gate) {
    QuantumState erro = {{NULL, 0}, 0};
    if (s.amplitudes.values == NULL || gate.values == NULL) return erro;
 
    // A porta precisa ser 2^n x 2^n, o mesmo tamanho do vetor de amplitudes
    // Note que s.amplitudes.len sempre é uma potência de 2
    if (gate.rlen != s.amplitudes.len || gate.clen != s.amplitudes.len) {
        printf("ERRO: a porta deve ter dimensão 2^n x 2^n, com n = n_qubits\n");
        return erro;
    }
 
    // Novo estado = porta * estado
    Vector novo = matrix_vec_mul(gate, s.amplitudes);
    if (novo.values == NULL) {
        printf("ERRO: falha na alocação dinâmica");    
        return erro;
    }
    QuantumState resultado = {novo, s.n_qubits};
    return resultado;
}
 
// Mede o estado sem colapsá-lo, retornando o índice do estado base observado
int measure(QuantumState s) {
    if (s.amplitudes.values == NULL) return -1; // Valor dummy que indica que não houve medida do estado
 
    // Sorteia r em [0, 1)
    // Como 0 <= rand() <= RAND_MAX, dividir por RAND_MAX +1 garante que r < 1
    double r = rand() / ((double) RAND_MAX + 1.0);
 
    double acumulado = 0.0;
    int ultimo_possivel = 0; // Último estado com probabilidade > 0
 
    // Soma as probabilidades até ultrapassar r: o estado em que isso acontece é o medido
    for (int i = 0; i < s.amplitudes.len; i++) {
        double prob = complex_mod2(s.amplitudes.values[i]);
        if (prob == 0.0) continue; // Estados com probabilidade zero não atualizam ultimo_possivel
        acumulado += prob;
        ultimo_possivel = i;
        if (r < acumulado) return i;
    }
 
    // Por erros de arredondamento, a soma de todas as probabilidades pode ser ligeiramente menor que 1;
    // assim, garante-se que essa função sempre retorne algum estado ao chegar nesse laço for
    return ultimo_possivel;
}
 
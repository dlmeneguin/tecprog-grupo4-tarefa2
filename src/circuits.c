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
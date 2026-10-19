#include linear_algebra.h

#ifndef circuits_h
#define circuits_h

typedef struct {
    Vector amplitudes;  // vetor de 2^n números complexos (2^n structs)
    int n_qubits;       // quantos qubits o estado representa
} QuantumState;

/**  
 * @brief Inicializa o estado |00...0>
 * @param n Número de qubits
 * @return Objeto QuantumState
 * @pre n deve ser positivo
*/
QuantumState init_qubits(int n);

/**
 * @brief Libera a memória alocada pelo estado de qubits
 * @param address Ponteiro para o vetor
 * @note É de responsabilidade do usuário chamar esta função após usar o QuantumState
*/
void free_QuantumState(QuantumState *address);

#endif
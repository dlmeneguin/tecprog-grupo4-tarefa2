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
 * @return Objeto que representa um estado de n qubits
 * @pre n deve ser positivo
*/
QuantumState init_qubits(int n);

/**
 * @brief Libera a memória alocada pelo estado de qubits
 * @param address Ponteiro para o estado
 * @note É de responsabilidade do usuário chamar esta função após usar o QuantumState
*/
void free_QuantumState(QuantumState *address);

/**
 * @brief Aplica uma porta quântica a um estado
 * @param s Estado desejado
 * @param gate Matriz que representa a porta
 * @return Objeto que representa o produto da porta pelo estado inicial
 * @pre gate deve ser uma matriz quadrada de dimensões 2^n x 2^n , com n = s.n_qubits
 * @note O estado inicial não é alterado, assim é de responsabilidade do usuário liberar seu espaço quando conveniente
*/
QuantumState apply_gate(QuantumState s, Matrix gate);

int measure(QuantumState s);

#endif
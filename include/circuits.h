#include "linear_algebra.h"

#ifndef circuits_h
#define circuits_h

/**
 * @brief Gera a seed para o uso do random
 * @note Deve ser chamada apenas uma vez
*/
void generate_seed();

/**  
 * @brief Inicializa o estado |00...0>
 * @param n Número de qubits
 * @return Objeto que representa um estado de n qubits
 * @pre n deve ser positivo
*/
Vector init_qubits(int n);

/**
 * @brief Aplica uma porta quântica a um estado
 * @param s Ponteiro para o vetor de estado
 * @param gate Matriz que representa a porta
 * @pre gate deve ser uma matriz quadrada de dimensões s->len x s->len
 * @post Caso ocorra sem falhas, o vetor de estado enviado é substituído pelo resultado da aplicação da porta quântica
 * @note O usuário, ao chamar a função, deve passar como parâmetro a própria função get_gate() do gates.c
*/
void apply_gate(Vector *s, Matrix gate);

/**
 * @brief Mede o estado sem colapsar o vetor de estado
 * @param s Ponteiro para o vetor de estado
 * @return Índice do estado observado(-1 em caso de erro)
 * @pre O vetor passado deve ser obrigatoriamente um vetor de estado(criado com init_qubits)
 * @note Não altera o vetor, somente medindo-o
*/
int measure(const Vector *s);

/**
 * @brief Realiza múltiplas medições do vetor de estado sem colapsá-lo
 * @param state Ponteiro para o vetor de estado
 * @param shots Número de medições realizadas
 * @return Um vetor que conta quantas vezes um estado foi medido
 * @pre O vetor passado deve ser obrigatoriamente um vetor de estado(criado com init_qubits) e shots deve ser não negativo
 * @note Não altera o vetor, somente medindo-o
 * @note É de responsabilidade do usuário a liberação do vetor retornado 
*/
int *measure_shots(const Vector *state, int shots);

#endif
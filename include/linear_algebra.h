#include "complex.h"

#ifndef LINEAR_ALGEBRA_H // Include guard
#define LINEAR_ALGEBRA_H

/* Criação de tipos */
typedef struct {
    Complex *values;
    int len;
} Vector;

typedef struct {
    Complex **values;
    int rlen;
    int clen;
} Matrix;


/* Headers */

/**
 * @brief Libera a memória alocada para um vetor.
 * @param address Ponteiro que aponta para o endereço do vetor.
 * @pre O paramêtro enviado deve ser um endereço &vector.
 * @note É de responsabilidade do usuário chamar esta função quando terminar de utilizar um vetor.
*/
void free_vector(Vector *address);

/**
 * @brief Libera a memória alocada para uma matriz.
 * @param address Ponteiro que aponta para o endereço do vetor.
 * @pre O paramêtro enviado deve ser um endereço &matrix.
 * @note É de responsabilidade do usuário chamar esta função quando terminar de utilizar uma matriz.
*/
void free_matrix(Matrix *address);

/**
 * @brief Realiza uma cópia profunda (deep copy) de uma matriz.
 * @param a Matriz de origem a ser duplicada.
 * @return Novo objeto Matrix com mesmos valores de a alocada na heap.
 * @pre a.values ser diferente de NULL e a.rlen, a.clen serem maiores ou iguais a 0.
 */
Matrix copy_matrix(Matrix a);

/**
 * @brief Cria um vetor de tamanho n.
 * @param n Tamanho do vetor.
 * @return Novo objeto Vector com todas as entradas zeradas.
 * @pre n deve ser positivo.
*/
Vector create_vector(int n);

/**
 * @brief Cria uma matriz de tamanho r x c.
 * @param r Número de linhas.
 * @param c Número de colunas.
 * @return Novo objeto Matrix com todas as entradas zeradas.
 * @pre r e c devem ser positivos.
*/
Matrix create_matrix(int r, int c);

/**
 * @brief Realiza o produto tensorial entre dois vetores.
 * @param a Primeiro termo(vetor) do produto tensorial.
 * @param b Segundo termo(vetor) do produto tensorial.
 * @return Objeto Vector resultado do produto tensorial.
*/
Vector tensor_product_vec(Vector a, Vector b);

/**
 * @brief Realiza o produto tensorial entre duas matrizes.
 * @param a Primeiro termo(matriz) do produto tensorial.
 * @param b Segundo termo(matriz) do produto tensorial.
 * @return Objeto Matrix resultado do produto tensorial.
*/
Matrix tensor_product_matrix(Matrix a, Matrix b);

/**
 * @brief Multiplica uma matriz por um escalar.
 * @param a Matriz que será multiplicada.
 * @param alfa Numero escalar complexo que multiplicará a matriz.
 * @return Objeto Matrix com a mesma matriz após a multiplicação.
 * @note As operações são realizadas diretamente na matriz fornecida.
 */
Matrix matrix_scale_mul(Matrix a, Complex alfa);

/**
 * @brief Realiza o produto entre duas matrizes.
 * @param a Primeiro termo(matriz) do produto matricial.
 * @param b Segundo termo(matriz) do produto matricial.
 * @return Objeto Matrix resultado do produto matricial.
 * @pre a.clen = b.rlen.
*/
Matrix matrix_mul(Matrix a, Matrix b);

/**
 * @brief Realiza o produto matriz-vetor.
 * @param a Matriz que será multiplicada.
 * @param v Vetor que multiplicará a matriz.
 * @return Objeto vector resultado do produto matriz-vetor.
 * @pre a.clen = v.len.
*/
Vector matrix_vec_mul(Matrix a, Vector v);

#endif
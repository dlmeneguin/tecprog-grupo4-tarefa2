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

/*
*
*/
void free_vector(Vector *address);

/*
*
*/
void free_matrix(Matrix *address);

/**
 * @brief Realiza uma cópia profunda (deep copy) de uma matriz.
 * @param a Matriz de origem a ser duplicada.
 * @return Novo objeto Matrix com mesmos valores de a alocada na heap.
 * @pre a.values ser diferente de NULL e a.rlen, a.clen serem maiores ou iguais a 0
 */
Matrix copy_matrix(Matrix a);

/*
*
*/
Vector create_vector(int n);

/*
*
*/
Vector tensor_product_vec(Vector a, Vector b);

/*
*
*/
Matrix tensor_product_matrix(Matrix a, Matrix b);

/**
 * @brief Multiplica uma matriz por um escalar.
 * @param a Matriz que será multiplicada
 * @param alfa Numero escalar complexo que multiplicará a matriz
 * @return Objeto Matrix com a mesma matriz após a multiplicação
 * @note As operações são realizadas diretamente na matriz fornecida
 */
Matrix matrix_scale_mul(Matrix a, Complex alfa);

/*
*
*/
Matrix matrix_mul(Matrix a, Matrix b);

/*
*
*/
Vector matrix_vec_mul(Matrix a, Vector v);

#endif
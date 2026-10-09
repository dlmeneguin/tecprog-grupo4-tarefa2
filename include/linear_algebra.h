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

/*
*
*/
Matrix matrix_mul(Matrix a, Matrix b);

/*
*
*/
Vector matrix_vec_mul(Matrix a, Vector v);

#endif
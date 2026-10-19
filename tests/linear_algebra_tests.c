#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "../include/linear_algebra.h"

//Função para verificar se as matrizes são iguais na medida do possível
int check_matrix(Matrix a, Matrix b){
    if (a.clen != b.clen || a.rlen != b.rlen) return 0;
    if (a.values == NULL || b.values == NULL) return 0;

    for (int i = 0; i < a.rlen; i++){
        for (int j = 0; j < a.clen; j++){
            //Valida igualdade quando a diferença é muito pequena
            double diferencaRe = a.values[i][j].re - b.values[i][j].re;
            double diferencaIm = a.values[i][j].im - b.values[i][j].im;
            if (diferencaRe < 0) diferencaRe = -diferencaRe; //módulo da diferença
            if (diferencaIm < 0) diferencaIm = -diferencaIm; //módulo da diferença

            if (diferencaRe > 0.00001 || diferencaIm > 0.00001) { //se a diferença for muito grande, diferentes
                return 0;
            }
        }
    }
    return 1;
}

//Função para verificar se os vetores são iguais na medida do possível
int check_vector(Vector a, Vector b){
    if (a.len != b.len) return 0;
    if (a.values == NULL || b.values == NULL) return 0;

    for (int i = 0; i < a.len; i++){
        //Valida igualdade quando a diferença é muito pequena
        double diferencaRe = a.values[i].re - b.values[i].re;
        double diferencaIm = a.values[i].im - b.values[i].im;
        if (diferencaRe < 0) diferencaRe = -diferencaRe; //módulo da diferença
        if (diferencaIm < 0) diferencaIm = -diferencaIm; //módulo da diferença

        if (diferencaRe > 0.00001 || diferencaIm > 0.00001) { //se a diferença for muito grande, diferentes
            return 0;
        }
    }
    return 1;
}

void test_free_vector(void){
    //Caso ponteiro nulo
    free_vector(NULL);

    //Caso vetor não inicializado / vazio
    Vector v = {NULL, 0};
    free_vector(&v);
    assert(v.values == NULL);
    assert(v.len == 0);

    //Caso base
    v = create_vector(2);
    assert(v.values != NULL);
    assert(v.len == 2);
    free_vector(&v);
    assert(v.values == NULL);
    assert(v.len == 0);
}

void test_free_matrix(void){
    //Caso ponteiro nulo
    free_matrix(NULL);

    //Caso matriz não inicializada / vazia
    Matrix A = {NULL, 0, 0};
    free_matrix(&A);
    assert(A.values == NULL);
    assert(A.rlen == 0);
    assert(A.clen == 0);

    //Caso base
    A = create_matrix(2, 2);
    assert(A.values != NULL);
    assert(A.rlen == 2 && A.clen == 2);
    free_matrix(&A);
    assert(A.values == NULL);
    assert(A.rlen == 0);
    assert(A.clen == 0);
}

void test_copy_matrix(void){
    //Casos inválidos, matriz com ponteiro nulo ou dimensões <= 0
    Matrix vazia = {NULL, 0, 0};
    Matrix copia = copy_matrix(vazia);
    assert(copia.values == NULL);
    assert(copia.rlen == 0 && copia.clen == 0);
    free_matrix(&copia);

    Matrix invalida = {NULL, -1, 2};
    copia = copy_matrix(invalida);
    assert(copia.values == NULL);
    free_matrix(&copia);

    //Caso base
    Matrix A = create_matrix(2, 2);
    A.values[0][0] = complex_new(1.0, 2.0);
    A.values[0][1] = complex_new(3.0, 4.0);
    A.values[1][0] = complex_new(2.0, 1.0);
    A.values[1][1] = complex_new(4.0, 3.0);

    Matrix B = copy_matrix(A);
    assert(check_matrix(A, B) == 1);

    //Modificação em B não deve alterar A (deep copy)
    B.values[0][0] = complex_new(99.0, 99.0);
    assert(A.values[0][0].re == 1.0 && A.values[0][0].im == 2.0);

    free_matrix(&A);
    free_matrix(&B);
}

void test_create_vector(void){
    //Casos inválidos, n <= 0
    Vector v = create_vector(0);
    assert(v.values == NULL);
    assert(v.len == 0);
    free_vector(&v);

    v = create_vector(-2);
    assert(v.values == NULL);
    assert(v.len == 0);
    free_vector(&v);

    //Caso base, n = 2
    v = create_vector(2);
    assert(v.values != NULL);
    assert(v.len == 2);
    for (int i = 0; i < 2; i++){
        assert(v.values[i].re == 0.0);
        assert(v.values[i].im == 0.0);
    }
    free_vector(&v);
}

void test_create_matrix(void){
    //Casos inválidos, r <= 0 ou c <= 0
    Matrix A = create_matrix(0, 2);
    assert(A.values == NULL);
    assert(A.rlen == 0 && A.clen == 0);
    free_matrix(&A);

    A = create_matrix(2, 0);
    assert(A.values == NULL);
    assert(A.rlen == 0 && A.clen == 0);
    free_matrix(&A);

    A = create_matrix(-1, 2);
    assert(A.values == NULL);
    assert(A.rlen == 0 && A.clen == 0);
    free_matrix(&A);

    A = create_matrix(2, -1);
    assert(A.values == NULL);
    assert(A.rlen == 0 && A.clen == 0);
    free_matrix(&A);

    //Caso base, r = 2, c = 2
    A = create_matrix(2, 2);
    assert(A.values != NULL);
    assert(A.rlen == 2 && A.clen == 2);
    for (int i = 0; i < A.rlen; i++){
        for (int j = 0; j < A.clen; j++){
            assert(A.values[i][j].re == 0.0);
            assert(A.values[i][j].im == 0.0);
        }
    }
    free_matrix(&A);
}

void test_tensor_product_vec(void){
    //Casos inválidos, vetores de tamanho <= 0
    Vector v_vazio = {NULL, 0};
    Vector v_valido = create_vector(2);
    Vector result = tensor_product_vec(v_vazio, v_valido);
    assert(result.values == NULL);
    assert(result.len == 0);
    free_vector(&result);

    result = tensor_product_vec(v_valido, v_vazio);
    assert(result.values == NULL);
    assert(result.len == 0);
    free_vector(&result);
    free_vector(&v_valido);

    //Caso base com vetor nulo
    Vector v1 = create_vector(2);
    Vector v2 = create_vector(2);
    v1.values[0] = complex_new(1.0, 0.0);
    v1.values[1] = complex_new(0.0, 1.0);

    result = tensor_product_vec(v1, v2);
    assert(result.len == v1.len * v2.len);
    for (int i = 0; i < result.len; i++){
        assert(result.values[i].re == 0.0 && result.values[i].im == 0.0);
    }
    free_vector(&result);

    //Caso genérico com valores não-nulos
    v2.values[0] = complex_new(2.0, 0.0);
    v2.values[1] = complex_new(0.0, 3.0);

    Vector expected = create_vector(4);
    expected.values[0] = complex_new(2.0, 0.0);  // (1, 0) * (2, 0)
    expected.values[1] = complex_new(0.0, 3.0);  // (1, 0) * (0, 3)
    expected.values[2] = complex_new(0.0, 2.0);  // (0, 1) * (2, 0)
    expected.values[3] = complex_new(-3.0, 0.0); // (0, 1) * (0, 3)

    result = tensor_product_vec(v1, v2);
    assert(check_vector(result, expected) == 1);

    free_vector(&v1);
    free_vector(&v2);
    free_vector(&result);
    free_vector(&expected);
}

void test_tensor_product_matrix(void){
    //Casos inválidos, matriz com dimensões <= 0
    Matrix m_vazia = {NULL, 0, 0};
    Matrix m_valida = create_matrix(2, 2);
    Matrix result = tensor_product_matrix(m_vazia, m_valida);
    assert(result.values == NULL);
    assert(result.rlen == 0 && result.clen == 0);
    free_matrix(&result);

    result = tensor_product_matrix(m_valida, m_vazia);
    assert(result.values == NULL);
    assert(result.rlen == 0 && result.clen == 0);
    free_matrix(&result);
    free_matrix(&m_valida);

    //Caso base com matriz nula
    Matrix A = create_matrix(2, 2);
    A.values[0][0] = complex_new(1.0, 2.0);
    A.values[0][1] = complex_new(3.0, 4.0);
    A.values[1][0] = complex_new(2.0, 1.0);
    A.values[1][1] = complex_new(4.0, 3.0);

    Matrix B = create_matrix(2, 2);

    result = tensor_product_matrix(A, B);
    assert(result.rlen == A.rlen * B.rlen);
    assert(result.clen == A.clen * B.clen);

    for (int i = 0; i < result.rlen; i++){
        for (int j = 0; j < result.clen; j++){
            assert(result.values[i][j].re == 0.0 && result.values[i][j].im == 0.0);
        }
    }
    free_matrix(&B);
    free_matrix(&result);

    //Caso genérico com matriz identidade
    Matrix I = create_matrix(2, 2);
    I.values[0][0] = complex_new(1.0, 0.0);
    I.values[1][1] = complex_new(1.0, 0.0);

    result = tensor_product_matrix(A, I);
    assert(result.rlen == 4 && result.clen == 4);
    assert(result.values[0][0].re == 1.0 && result.values[0][0].im == 2.0);
    assert(result.values[1][1].re == 1.0 && result.values[1][1].im == 2.0);
    assert(result.values[2][2].re == 4.0 && result.values[2][2].im == 3.0);
    assert(result.values[3][3].re == 4.0 && result.values[3][3].im == 3.0);

    free_matrix(&A);
    free_matrix(&I);
    free_matrix(&result);
}

void test_matrix_scale_mul(void){
    //Casos inválidos, matriz com dimensões <= 0
    Matrix m_vazia = {NULL, 0, 0};
    Complex alpha = complex_new(2.0, 0.0);
    Matrix result = matrix_scale_mul(m_vazia, alpha);
    assert(result.values == NULL);
    assert(result.rlen == 0 && result.clen == 0);
    free_matrix(&result);

    //Caso base
    Matrix A = create_matrix(2, 2);
    A.values[0][0] = complex_new(1.0, 2.0);
    A.values[0][1] = complex_new(3.0, 4.0);
    A.values[1][0] = complex_new(2.0, 1.0);
    A.values[1][1] = complex_new(4.0, 3.0);

    result = matrix_scale_mul(A, alpha);
    assert(result.rlen == A.rlen && result.clen == A.clen);

    for (int i = 0; i < result.rlen; i++){
        for (int j = 0; j < result.clen; j++){
            Complex esperado = complex_mul(alpha, A.values[i][j]);
            assert(result.values[i][j].re == esperado.re);
            assert(result.values[i][j].im == esperado.im);
        }
    }

    free_matrix(&A);
    free_matrix(&result);
}

void test_matrix_mul(void){
    //Casos inválidos, colunas de a diferente de linhas de b
    Matrix A = create_matrix(2, 3);
    Matrix B = create_matrix(2, 2);
    Matrix result = matrix_mul(A, B);
    assert(result.values == NULL);
    assert(result.rlen == 0 && result.clen == 0);
    free_matrix(&result);
    free_matrix(&A);
    free_matrix(&B);

    //Caso base, multiplicação por identidade
    A = create_matrix(2, 2);
    A.values[0][0] = complex_new(1.0, 2.0);
    A.values[0][1] = complex_new(3.0, 4.0);
    A.values[1][0] = complex_new(2.0, 1.0);
    A.values[1][1] = complex_new(4.0, 3.0);

    Matrix I = create_matrix(2, 2);
    I.values[0][0] = complex_new(1.0, 0.0);
    I.values[1][1] = complex_new(1.0, 0.0);

    result = matrix_mul(A, I);
    assert(result.rlen == A.rlen && result.clen == A.clen);
    assert(check_matrix(result, A) == 1);

    free_matrix(&I);
    free_matrix(&result);

    //Caso genérico, matrizes retangulares (2x3) * (3x2)
    Matrix M1 = create_matrix(2, 3);
    M1.values[0][0] = complex_new(1.0, 0.0);
    M1.values[0][1] = complex_new(2.0, 0.0);
    M1.values[0][2] = complex_new(3.0, 0.0);
    M1.values[1][0] = complex_new(4.0, 0.0);
    M1.values[1][1] = complex_new(5.0, 0.0);
    M1.values[1][2] = complex_new(6.0, 0.0);

    Matrix M2 = create_matrix(3, 2);
    M2.values[0][0] = complex_new(7.0, 0.0);
    M2.values[0][1] = complex_new(8.0, 0.0);
    M2.values[1][0] = complex_new(9.0, 0.0);
    M2.values[1][1] = complex_new(1.0, 0.0);
    M2.values[2][0] = complex_new(2.0, 0.0);
    M2.values[2][1] = complex_new(3.0, 0.0);

    Matrix expected = create_matrix(2, 2);
    expected.values[0][0] = complex_new(31.0, 0.0);
    expected.values[0][1] = complex_new(19.0, 0.0);
    expected.values[1][0] = complex_new(85.0, 0.0);
    expected.values[1][1] = complex_new(55.0, 0.0);

    result = matrix_mul(M1, M2);
    assert(check_matrix(result, expected) == 1);

    free_matrix(&A);
    free_matrix(&M1);
    free_matrix(&M2);
    free_matrix(&expected);
    free_matrix(&result);
}

void test_matrix_vec_mul(void){
    //Casos inválidos, colunas de a diferente do tamanho do vetor v
    Matrix A = create_matrix(2, 3);
    Vector v = create_vector(2);
    Vector result = matrix_vec_mul(A, v);
    assert(result.values == NULL);
    assert(result.len == 0);
    free_vector(&result);
    free_matrix(&A);
    free_vector(&v);

    //Caso base com vetor nulo
    A = create_matrix(2, 2);
    A.values[0][0] = complex_new(1.0, 2.0);
    A.values[0][1] = complex_new(3.0, 4.0);
    A.values[1][0] = complex_new(2.0, 1.0);
    A.values[1][1] = complex_new(4.0, 3.0);

    v = create_vector(2);
    result = matrix_vec_mul(A, v);
    assert(result.len == 2);
    for (int i = 0; i < result.len; i++){
        assert(result.values[i].re == 0.0 && result.values[i].im == 0.0);
    }
    free_vector(&result);

    //Caso genérico com vetor não-nulo
    v.values[0] = complex_new(1.0, 0.0);
    v.values[1] = complex_new(2.0, 0.0);

    Vector expected = create_vector(2);
    expected.values[0] = complex_new(7.0, 10.0);
    expected.values[1] = complex_new(10.0, 7.0);

    result = matrix_vec_mul(A, v);
    assert(check_vector(result, expected) == 1);

    free_matrix(&A);
    free_vector(&v);
    free_vector(&expected);
    free_vector(&result);
}

int main(void){
    printf("Iniciando os testes do módulo de álgebra linear\n");

    test_free_vector();
    printf("função free_vector OK\n");

    test_free_matrix();
    printf("função free_matrix OK\n");

    test_copy_matrix();
    printf("função copy_matrix OK\n");

    test_create_vector();
    printf("função create_vector OK\n");

    test_create_matrix();
    printf("função create_matrix OK\n");

    test_tensor_product_vec();
    printf("função tensor_product_vec OK\n");

    test_tensor_product_matrix();
    printf("função tensor_product_matrix OK\n");

    test_matrix_scale_mul();
    printf("função matrix_scale_mul OK\n");

    test_matrix_mul();
    printf("função matrix_mul OK\n");

    test_matrix_vec_mul();
    printf("função matrix_vec_mul OK\n");

    printf("\nTodos os testes do módulo de álgebra linear passaram\n");
    return 0;
}

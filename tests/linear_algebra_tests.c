#include <stdio.h>
#include "linear_algebra.h"

int test_free_vector(){
    Vector v = create_vector(1);
    free_vector(&v);
    if (v.values == NULL && v.len == 0) return 0;
    return 1;
}

int test_free_matrix(){
    Matrix A = create_matrix(2, 2);
    free_matrix(&A);
    if (A.values == NULL && A.rlen == 0 && A.clen == 0) return 0;
    return 1;
}

int test_copy_matrix(){
    Matrix A = create_matrix(2, 2);
    A.values[0][0] = (Complex) {1, 2};
    A.values[0][1] = (Complex) {3, 4};
    A.values[1][0] = (Complex) {2, 1};
    A.values[1][1] = (Complex) {4, 3};

    Matrix B = copy_matrix(A);

    if (A.rlen != B.rlen || A.clen != B.clen){
        free_matrix(&A);
        free_matrix(&B);
        return 1;
    }

    for (int i = 0; i < A.rlen; i++){
        for (int j = 0; j < A.clen; j++){
            if (A.values[i][j].re != B.values[i][j].re || A.values[i][j].im != B.values[i][j].im){
                free_matrix(&A);
                free_matrix(&B);
                return 1;
            }
        }
    }

    free_matrix(&A);
    free_matrix(&B);
    return 0;
}

int test_create_vector(){
    Vector v = create_vector(2);
    if (v.len != 2){
        free_vector(&v);
        return 1;
    }

    for (int i = 0; i < 2; i++){
        if (v.values[i].re != 0 || v.values[i].im != 0){
            free_vector(&v);
            return 1;
        }
    }

    free_vector(&v);
    return 0;
}

int test_create_matrix(){
    Matrix A = create_matrix(2, 2);

    if (A.rlen != 2 || A.clen != 2){
        free_matrix(&A);
        return 1;
    }

    for (int i = 0; i < A.rlen; i++){
        for (int j = 0; j < A.clen; j++){
            if (A.values[i][j].re != 0 || A.values[i][j].im != 0){
                free_matrix(&A);
                return 1;
            }
        }
    }

    free_matrix(&A);
    return 0;
}

int test_tensor_product_vec(){
    Vector v1 = create_vector(2);
    Vector v2 = create_vector(2);
    v1.values[0] = (Complex) {1, 0}; v1.values[1] = (Complex) {0, 1};

    Vector result = tensor_product_vec(v1, v2);

    if (result.len != v1.len * v2.len){
        free_vector(&v1);
        free_vector(&v2);
        free_vector(&result);
        return 1;
    }

    free_vector(&v1);
    free_vector(&v2);

    // Como v2 está zerado, é esperado que o resultado também tenha todas as entradas zeradas
    for (int i = 0; i < result.len; i++){
        if (result.values[i].re != 0 || result.values[i].im != 0){
            free_vector(&result);
            return 1;
        }
    }

    free_vector(&result);
    return 0;
}

int test_tensor_product_matrix(){
    Matrix A = create_matrix(2, 2);
    A.values[0][0] = (Complex) {1, 2};
    A.values[0][1] = (Complex) {3, 4};
    A.values[1][0] = (Complex) {2, 1};
    A.values[1][1] = (Complex) {4, 3};

    Matrix B = create_matrix(2, 2);

    Matrix result = tensor_product_matrix(A, B);

    if (result.rlen != A.rlen * B.rlen || result.clen != A.clen * B.clen){
        free_matrix(&A);
        free_matrix(&B);
        free_matrix(&result);
        return 1;
    }

    free_matrix(&A);
    free_matrix(&B);

    // Como B está zerada, é esperado que o resultado também tenha todas as entradas zeradas
    for (int i = 0; i < result.rlen; i++){
        for (int j = 0; j < result.clen; j++){
            if (result.values[i][j].re != 0 || result.values[i][j].im != 0){
                free_matrix(&result);
                return 1;
            }
        }
    }

    free_matrix(&result);
    return 0;
}

int test_matrix_scale_mul(){
    Matrix A = create_matrix(2, 2);
    A.values[0][0] = (Complex) {1, 2};
    A.values[0][1] = (Complex) {3, 4};
    A.values[1][0] = (Complex) {2, 1};
    A.values[1][1] = (Complex) {4, 3};

    Complex alpha = {2, 0};

    Matrix result = matrix_scale_mul(A, alpha);
    if (result.rlen != A.rlen || result.clen != A.clen){
        free_matrix(&A);
        free_matrix(&result);
        return 1;
    }

    for (int i = 0; i < result.rlen; i++){
        for (int j = 0; j < result.clen; j++){
            Complex curr_value = complex_mul(alpha, A.values[i][j]);
            if (result.values[i][j].re != curr_value.re || result.values[i][j].im != curr_value.im){
                free_matrix(&A);
                free_matrix(&result);
                return 1;
            }
        }
    }

    free_matrix(&A);
    free_matrix(&result);
    return 0;
}

int test_matrix_mul(){
    Matrix A = create_matrix(2, 2);
    A.values[0][0] = (Complex) {1, 2};
    A.values[0][1] = (Complex) {3, 4};
    A.values[1][0] = (Complex) {2, 1};
    A.values[1][1] = (Complex) {4, 3};

    Matrix I = create_matrix(2, 2);
    I.values[0][0] = (Complex) {1, 0};
    I.values[0][1] = (Complex) {0, 0};
    I.values[1][0] = (Complex) {0, 0};
    I.values[1][1] = (Complex) {1, 0};

    Matrix result = matrix_mul(A, I);
    if (result.rlen != A.rlen || result.clen != A.clen){
        free_matrix(&A);
        free_matrix(&I);
        free_matrix(&result);
        return 1;
    }

    free_matrix(&I);

    // Qualquer produto matricial pela identidade deve se manter igual
    for (int i = 0; i < result.rlen; i++){
        for (int j = 0; j < result.clen; j++){
            if (result.values[i][j].re != A.values[i][j].re || result.values[i][j].im != A.values[i][j].im){
                free_matrix(&A);
                free_matrix(&result);
                return 1;
            }
        }
    }

    free_matrix(&A);
    free_matrix(&result);
    return 0;
}

int test_matrix_vec_mul(){
    Matrix A = create_matrix(2, 2);
    A.values[0][0] = (Complex) {1, 2};
    A.values[0][1] = (Complex) {3, 4};
    A.values[1][0] = (Complex) {2, 1};
    A.values[1][1] = (Complex) {4, 3};

    Vector v = create_vector(2);

    Vector result = matrix_vec_mul(A, v);
    if (result.len != v.len){
        free_matrix(&A);
        free_vector(&v);
        free_vector(&result);
        return 1;
    }

    free_matrix(&A);
    free_vector(&v);

    // Como v está zerado, é esperado que o resultado também esteja
    for (int i = 0; i < result.len; i++){
        if (result.values[i].re != 0 || result.values[i].im != 0){
            free_vector(&result);
            return 1;
        }
    }

    free_vector(&result);
    return 0;
}

int main(){
    int total = 0;
    int failed = 0;

    total++;
    if (test_free_vector() != 0) {
        printf("free_vector falhou\n");
        failed++;
    }

    total++;
    if (test_free_matrix() != 0) {
        printf("free_matrix falhou\n");
        failed++;
    }

    total++;
    if (test_copy_matrix() != 0) {
        printf("copy_matrix falhou\n");
        failed++;
    }

    total++;
    if (test_create_vector() != 0) {
        printf("create_vector falhou\n");
        failed++;
    }

    total++;
    if (test_create_matrix() != 0) {
        printf("create_matrix falhou\n");
        failed++;
    }

    total++;
    if (test_tensor_product_vec() != 0) {
        printf("tensor_product_vec falhou\n");
        failed++;
    }

    total++;
    if (test_tensor_product_matrix() != 0) {
        printf("tensor_product_matrix falhou\n");
        failed++;
    }

    total++;
    if (test_matrix_scale_mul() != 0) {
        printf("matrix_scale_mul falhou\n");
        failed++;
    }

    total++;
    if (test_matrix_mul() != 0) {
        printf("matrix_mul falhou\n");
        failed++;
    }

    total++;
    if (test_matrix_vec_mul() != 0) {
        printf("matrix_vec_mul falhou\n");
        failed++;
    }

    printf("%d/%d testes falharam.\n", failed, total);

    return 0;
}

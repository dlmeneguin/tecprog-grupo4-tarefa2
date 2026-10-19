#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "../include/gates.h"

//Função para verificar se as matrizes são iguais na medida do possível
int check_matrix(Matrix a, Matrix b){
    if (a.clen != b.clen || a.rlen != b.rlen) return 0;
    if (a.values == NULL || b.values == NULL) return 0;

    for (int i = 0; i < a.rlen; i++){
        for (int j = 0; j < a.clen; j++){
            //Valida igualdade quando a diferença é muito pequena
            double diferencaRe = a.values[i][j].re - b.values[i][j].re;
            double diferencaIm = a.values[i][j].im - b.values[i][j].im;
            if (diferencaRe < 0) diferencaRe = -diferencaRe; //módulo ds difenreça
            if (diferencaIm < 0) diferencaIm = -diferencaIm; //módulo ds difenreça
            
            if (diferencaRe > 0.00001 || diferencaIm > 0.00001) { //se a diferença for muito grande, diferentes
                return 0;
            }
        }
    }
    return 1;
}

void test_expand_gate(void){
    //Matriz base para os testes
    Matrix base = create_matrix(2, 2);
    base.values[0][0] = complex_new(3.0, 4.0);
    base.values[0][1] = complex_new(5.0, 0.0);
    base.values[1][0] = complex_new(1.0, 2.0);
    base.values[1][1] = complex_new(4.0, 0.0);
    
    //Teste caso inválido, n <= 0
    Matrix resultado = expand_gate(0, copy_matrix(base));
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    //Teste caso base, n = 1
    resultado = expand_gate(1, copy_matrix(base));
    assert(check_matrix(resultado, base) == 1);
    free_matrix(&resultado);

    //Matriz esperada 8x8 para expand_gate(3, base)
    Complex expected_vals[8][8] = {
        { {-117.0,  44.0}, {-35.0, 120.0}, {-35.0, 120.0}, {75.0, 100.0}, {-35.0, 120.0}, {75.0, 100.0}, {75.0, 100.0}, {125.0,   0.0} },
        { { -55.0,  10.0}, {-28.0,  96.0}, {-25.0,  50.0}, {60.0,  80.0}, {-25.0,  50.0}, {60.0,  80.0}, {25.0,  50.0}, {100.0,   0.0} },
        { { -55.0,  10.0}, {-25.0,  50.0}, {-28.0,  96.0}, {60.0,  80.0}, {-25.0,  50.0}, {25.0,  50.0}, {60.0,  80.0}, {100.0,   0.0} },
        { { -25.0,   0.0}, {-20.0,  40.0}, {-20.0,  40.0}, {48.0,  64.0}, {-15.0,  20.0}, {20.0,  40.0}, {20.0,  40.0}, { 80.0,   0.0} },
        { { -55.0,  10.0}, {-25.0,  50.0}, {-25.0,  50.0}, {25.0,  50.0}, {-28.0,  96.0}, {60.0,  80.0}, {60.0,  80.0}, {100.0,   0.0} },
        { { -25.0,   0.0}, {-20.0,  40.0}, {-15.0,  20.0}, {20.0,  40.0}, {-20.0,  40.0}, {48.0,  64.0}, {20.0,  40.0}, { 80.0,   0.0} },
        { { -25.0,   0.0}, {-15.0,  20.0}, {-20.0,  40.0}, {20.0,  40.0}, {-20.0,  40.0}, {20.0,  40.0}, {48.0,  64.0}, { 80.0,   0.0} },
        { { -11.0,  -2.0}, {-12.0,  16.0}, {-12.0,  16.0}, {16.0,  32.0}, {-12.0,  16.0}, {16.0,  32.0}, {16.0,  32.0}, { 64.0,   0.0} }
    };
    Matrix expected = create_matrix(8, 8);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            expected.values[i][j] = expected_vals[i][j];
        }
    }

    //Teste caso genérico, n = 3
    resultado = expand_gate(3, copy_matrix(base));
    assert(check_matrix(resultado, expected) == 1);

    free_matrix(&resultado);
    free_matrix(&base);
    free_matrix(&expected);
}

void test_get_identity(void){
    //Caso inválido, n <= 0
    Matrix resultado = get_identity(0);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    //Caso base, n = 1
    Matrix expected = create_matrix(2, 2);
    expected.values[0][0] = complex_new(1.0, 0.0);
    expected.values[1][1] = complex_new(1.0, 0.0);

    resultado = get_identity(1);
    assert(check_matrix(resultado, expected) == 1);
    free_matrix(&resultado);
    free_matrix(&expected);

    //Caso genérico, n = 2 
    expected = create_matrix(4, 4);
    for (int i = 0; i < 4; i++) {
        expected.values[i][i] = complex_new(1.0, 0.0);
    }

    resultado = get_identity(2);
    assert(check_matrix(resultado, expected) == 1);
    free_matrix(&resultado);
    free_matrix(&expected);
}

void test_get_hadamard(void){
    //Caso inválido, n <= 0
    Matrix resultado = get_hadamard(0);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    //Caso base, n = 1
    Matrix expected = create_matrix(2, 2);
    double inv_sqrt2 = 1.0 / sqrt(2.0);
    expected.values[0][0] = complex_new(inv_sqrt2, 0.0);
    expected.values[0][1] = complex_new(inv_sqrt2, 0.0);
    expected.values[1][0] = complex_new(inv_sqrt2, 0.0);
    expected.values[1][1] = complex_new(-inv_sqrt2, 0.0);

    resultado = get_hadamard(1);
    assert(check_matrix(resultado, expected) == 1);
    free_matrix(&resultado);
    free_matrix(&expected);

    //Caso genérico, n = 2
    expected = create_matrix(4, 4);
    Complex vals_h2[4][4] = {
        { { 0.5, 0.0}, { 0.5, 0.0}, { 0.5, 0.0}, { 0.5, 0.0} },
        { { 0.5, 0.0}, {-0.5, 0.0}, { 0.5, 0.0}, {-0.5, 0.0} },
        { { 0.5, 0.0}, { 0.5, 0.0}, {-0.5, 0.0}, {-0.5, 0.0} },
        { { 0.5, 0.0}, {-0.5, 0.0}, {-0.5, 0.0}, { 0.5, 0.0} }
    };
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            expected.values[i][j] = vals_h2[i][j];
        }
    }

    resultado = get_hadamard(2);
    assert(check_matrix(resultado, expected) == 1);
    free_matrix(&resultado);
    free_matrix(&expected);
}

void test_get_rotateX(void){
    //Caso inválido, n <= 0
    Matrix resultado = get_rotateX(0, 0.0);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    //Caso theta = 0, matriz identidade
    Matrix expected = get_identity(1);
    resultado = get_rotateX(1, 0.0);
    assert(check_matrix(resultado, expected) == 1);
    free_matrix(&resultado);
    free_matrix(&expected);

    //Caso theta = pi
    expected = create_matrix(2, 2);
    expected.values[0][1] = complex_new(0.0, -1.0);
    expected.values[1][0] = complex_new(0.0, -1.0);

    resultado = get_rotateX(1, M_PI);
    assert(check_matrix(resultado, expected) == 1);
    free_matrix(&resultado);
    free_matrix(&expected);

    //Caso genérico, n = 2 com theta = 0
    expected = get_identity(2);
    resultado = get_rotateX(2, 0.0);
    assert(check_matrix(resultado, expected) == 1);
    free_matrix(&resultado);
    free_matrix(&expected);
}

void test_get_pauliX(void){
    //Caso inválido, n <= 0
    Matrix resultado = get_pauliX(0);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    //Caso base, n = 1
    Matrix expected = create_matrix(2, 2);
    expected.values[0][1] = complex_new(1.0, 0.0);
    expected.values[1][0] = complex_new(1.0, 0.0);

    resultado = get_pauliX(1);
    assert(check_matrix(resultado, expected) == 1);
    free_matrix(&resultado);
    free_matrix(&expected);

    //Caso genérico, n = 2
    expected = create_matrix(4, 4);
    expected.values[0][3] = complex_new(1.0, 0.0);
    expected.values[1][2] = complex_new(1.0, 0.0);
    expected.values[2][1] = complex_new(1.0, 0.0);
    expected.values[3][0] = complex_new(1.0, 0.0);

    resultado = get_pauliX(2);
    assert(check_matrix(resultado, expected) == 1);
    free_matrix(&resultado);
    free_matrix(&expected);
}

void test_get_rotateY(void){
    //Caso inválido, n <= 0
    Matrix resultado = get_rotateY(0, 0.0);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    //Caso theta = 0, matriz identidade
    Matrix expected = get_identity(1);
    resultado = get_rotateY(1, 0.0);
    assert(check_matrix(resultado, expected) == 1);
    free_matrix(&resultado);
    free_matrix(&expected);

    //Caso theta = pi
    expected = create_matrix(2, 2);
    expected.values[0][1] = complex_new(-1.0, 0.0);
    expected.values[1][0] = complex_new(1.0, 0.0);

    resultado = get_rotateY(1, M_PI);
    assert(check_matrix(resultado, expected) == 1);
    free_matrix(&resultado);
    free_matrix(&expected);
}

void test_get_pauliY(void){
    //Caso inválido, n <= 0
    Matrix resultado = get_pauliY(0);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    //Caso base, n = 1
    Matrix expected = create_matrix(2, 2);
    expected.values[0][1] = complex_new(0.0, -1.0);
    expected.values[1][0] = complex_new(0.0, 1.0);

    resultado = get_pauliY(1);
    assert(check_matrix(resultado, expected) == 1);
    free_matrix(&resultado);
    free_matrix(&expected);

    //Caso genérico, n = 2
    expected = create_matrix(4, 4);
    expected.values[0][3] = complex_new(-1.0, 0.0);
    expected.values[1][2] = complex_new(1.0, 0.0);
    expected.values[2][1] = complex_new(1.0, 0.0);
    expected.values[3][0] = complex_new(-1.0, 0.0);

    resultado = get_pauliY(2);
    assert(check_matrix(resultado, expected) == 1);
    free_matrix(&resultado);
    free_matrix(&expected);
}

void test_get_rotateZ(void){
    //Caso inválido, n <= 0
    Matrix resultado = get_rotateZ(0, 0.0);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    //Caso theta = 0, matriz identidade
    Matrix expected = get_identity(1);
    resultado = get_rotateZ(1, 0.0);
    assert(check_matrix(resultado, expected) == 1);
    free_matrix(&resultado);
    free_matrix(&expected);

    //Caso theta = pi
    expected = create_matrix(2, 2);
    expected.values[0][0] = complex_new(0.0, -1.0);
    expected.values[1][1] = complex_new(0.0, 1.0);

    resultado = get_rotateZ(1, M_PI);
    assert(check_matrix(resultado, expected) == 1);
    free_matrix(&resultado);
    free_matrix(&expected);
}

void test_get_pauliZ(void){
    //Caso inválido, n <= 0
    Matrix resultado = get_pauliZ(0);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    //Caso base, n = 1
    Matrix expected = create_matrix(2, 2);
    expected.values[0][0] = complex_new(1.0, 0.0);
    expected.values[1][1] = complex_new(-1.0, 0.0);

    resultado = get_pauliZ(1);
    assert(check_matrix(resultado, expected) == 1);
    free_matrix(&resultado);
    free_matrix(&expected);

    //Caso genérico, n = 2
    expected = create_matrix(4, 4);
    expected.values[0][0] = complex_new(1.0, 0.0);
    expected.values[1][1] = complex_new(-1.0, 0.0);
    expected.values[2][2] = complex_new(-1.0, 0.0);
    expected.values[3][3] = complex_new(1.0, 0.0);

    resultado = get_pauliZ(2);
    assert(check_matrix(resultado, expected) == 1);
    free_matrix(&resultado);
    free_matrix(&expected);
}

void test_get_cnot(void){
    //Casos inválidos, n <= 1
    Matrix resultado = get_cnot(1, 0, 1);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    resultado = get_cnot(0, 0, 1);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    //Caso inválido, control == target
    resultado = get_cnot(2, 0, 0);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    //Casos inválidos, índices fora dos limites
    resultado = get_cnot(2, -1, 1);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    resultado = get_cnot(2, 0, 2);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    resultado = get_cnot(2, 2, 0);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    //Caso base, n = 2, control = 0, target = 1
    Matrix expected = create_matrix(4, 4);
    expected.values[0][0] = complex_new(1.0, 0.0);
    expected.values[1][1] = complex_new(1.0, 0.0);
    expected.values[2][3] = complex_new(1.0, 0.0);
    expected.values[3][2] = complex_new(1.0, 0.0);

    resultado = get_cnot(2, 0, 1);
    assert(check_matrix(resultado, expected) == 1);
    free_matrix(&resultado);
    free_matrix(&expected);

    //Caso base, n = 2, control = 1, target = 0
    expected = create_matrix(4, 4);
    expected.values[0][0] = complex_new(1.0, 0.0);
    expected.values[1][3] = complex_new(1.0, 0.0);
    expected.values[2][2] = complex_new(1.0, 0.0);
    expected.values[3][1] = complex_new(1.0, 0.0);

    resultado = get_cnot(2, 1, 0);
    assert(check_matrix(resultado, expected) == 1);
    free_matrix(&resultado);
    free_matrix(&expected);
}

void test_get_cz(void){
    //Casos inválidos, n <= 1
    Matrix resultado = get_cz(1, 0, 1);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    resultado = get_cz(0, 0, 1);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    //Caso inválido, control == target
    resultado = get_cz(2, 0, 0);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    //Casos inválidos, índices fora dos limites
    resultado = get_cz(2, -1, 1);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    resultado = get_cz(2, 0, 2);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    resultado = get_cz(2, 2, 0);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    //Caso base, n = 2, control = 0, target = 1
    Matrix expected = create_matrix(4, 4);
    expected.values[0][0] = complex_new(1.0, 0.0);
    expected.values[1][1] = complex_new(1.0, 0.0);
    expected.values[2][2] = complex_new(1.0, 0.0);
    expected.values[3][3] = complex_new(-1.0, 0.0);

    resultado = get_cz(2, 0, 1);
    assert(check_matrix(resultado, expected) == 1);
    free_matrix(&resultado);
    free_matrix(&expected);

    //Caso base, n = 2, control = 1, target = 0
    expected = create_matrix(4, 4);
    expected.values[0][0] = complex_new(1.0, 0.0);
    expected.values[1][1] = complex_new(1.0, 0.0);
    expected.values[2][2] = complex_new(1.0, 0.0);
    expected.values[3][3] = complex_new(-1.0, 0.0);

    resultado = get_cz(2, 1, 0);
    assert(check_matrix(resultado, expected) == 1);
    free_matrix(&resultado);
    free_matrix(&expected);

    //Caso genérico, n = 3, control = 0, target = 2
    expected = create_matrix(8, 8);
    for (int i = 0; i < 8; i++) {
        expected.values[i][i] = complex_new(1.0, 0.0);
    }
    expected.values[5][5] = complex_new(-1.0, 0.0);
    expected.values[7][7] = complex_new(-1.0, 0.0);

    resultado = get_cz(3, 0, 2);
    assert(check_matrix(resultado, expected) == 1);
    free_matrix(&resultado);
    free_matrix(&expected);
}

void test_get_ccnot(void){
    //Casos inválidos, n <= 2
    Matrix resultado = get_ccnot(2, 0, 1, 2);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    resultado = get_ccnot(0, 0, 1, 2);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    //Casos inválidos, qubits iguais
    resultado = get_ccnot(3, 0, 0, 2);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    resultado = get_ccnot(3, 0, 1, 0);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    resultado = get_ccnot(3, 0, 1, 1);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    //Casos inválidos, índices fora dos limites
    resultado = get_ccnot(3, -1, 1, 2);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    resultado = get_ccnot(3, 0, -1, 2);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    resultado = get_ccnot(3, 0, 1, -1);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    resultado = get_ccnot(3, 3, 1, 2);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    resultado = get_ccnot(3, 0, 3, 2);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    resultado = get_ccnot(3, 0, 1, 3);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    //Caso base, n = 3, control1 = 0, control2 = 1, target = 2
    Matrix expected = create_matrix(8, 8);
    for (int i = 0; i < 6; i++) {
        expected.values[i][i] = complex_new(1.0, 0.0);
    }
    expected.values[6][7] = complex_new(1.0, 0.0);
    expected.values[7][6] = complex_new(1.0, 0.0);

    resultado = get_ccnot(3, 0, 1, 2);
    assert(check_matrix(resultado, expected) == 1);
    free_matrix(&resultado);
    free_matrix(&expected);

    //Caso base, n = 3, control1 = 0, control2 = 2, target = 1
    expected = create_matrix(8, 8);
    for (int i = 0; i < 5; i++) {
        expected.values[i][i] = complex_new(1.0, 0.0);
    }
    expected.values[6][6] = complex_new(1.0, 0.0);
    expected.values[5][7] = complex_new(1.0, 0.0);
    expected.values[7][5] = complex_new(1.0, 0.0);

    resultado = get_ccnot(3, 0, 2, 1);
    assert(check_matrix(resultado, expected) == 1);
    free_matrix(&resultado);
    free_matrix(&expected);
}

void test_get_ccz(void){
    //Casos inválidos, n <= 2
    Matrix resultado = get_ccz(2, 0, 1, 2);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    resultado = get_ccz(0, 0, 1, 2);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    //Casos inválidos, qubits iguais
    resultado = get_ccz(3, 0, 0, 2);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    resultado = get_ccz(3, 0, 1, 0);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    resultado = get_ccz(3, 0, 1, 1);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    //Casos inválidos, índices fora dos limites
    resultado = get_ccz(3, -1, 1, 2);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    resultado = get_ccz(3, 0, -1, 2);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    resultado = get_ccz(3, 0, 1, -1);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    resultado = get_ccz(3, 3, 1, 2);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    resultado = get_ccz(3, 0, 3, 2);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    resultado = get_ccz(3, 0, 1, 3);
    assert(resultado.values == NULL);
    free_matrix(&resultado);

    //Caso base, n = 3, control1 = 0, control2 = 1, target = 2
    Matrix expected = create_matrix(8, 8);
    for (int i = 0; i < 7; i++) {
        expected.values[i][i] = complex_new(1.0, 0.0);
    }
    expected.values[7][7] = complex_new(-1.0, 0.0);

    resultado = get_ccz(3, 0, 1, 2);
    assert(check_matrix(resultado, expected) == 1);
    free_matrix(&resultado);
    free_matrix(&expected);

    //Caso base, n = 3, control1 = 1, control2 = 2, target = 0
    expected = create_matrix(8, 8);
    for (int i = 0; i < 7; i++) {
        expected.values[i][i] = complex_new(1.0, 0.0);
    }
    expected.values[7][7] = complex_new(-1.0, 0.0);

    resultado = get_ccz(3, 1, 2, 0);
    assert(check_matrix(resultado, expected) == 1);
    free_matrix(&resultado);
    free_matrix(&expected);
}

int main(void){
    printf("Iniciando os testes do módulo de portas quânticas\n");

    test_expand_gate();
    printf("função expand_gate OK\n");

    test_get_identity();
    printf("função get_identity OK\n");

    test_get_hadamard();
    printf("função get_hadamard OK\n");

    test_get_rotateX();
    printf("função get_rotateX OK\n");

    test_get_pauliX();
    printf("função get_pauliX OK\n");

    test_get_rotateY();
    printf("função get_rotateY OK\n");

    test_get_pauliY();
    printf("função get_pauliY OK\n");

    test_get_rotateZ();
    printf("função get_rotateZ OK\n");

    test_get_pauliZ();
    printf("função get_pauliZ OK\n");

    test_get_cnot();
    printf("função get_cnot OK\n");

    test_get_cz();
    printf("função get_cz OK\n");

    test_get_ccnot();
    printf("função get_ccnot OK\n");

    test_get_ccz();
    printf("função get_ccz OK\n");

    printf("\nTodos os testes do módulo de portas quânticas passaram\n");
    return 0;
}
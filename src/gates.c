#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/complex.h"
#include "../include/linear_algebra.h"
#include "../include/gates.h"

Matrix expand_gate(int n, Matrix base){
    Matrix vazia = {NULL, 0, 0};
    if (n <= 0){
        free_matrix(&base);
        return vazia;
    }

    //Cresce a matriz para n qubits com tensor product e a base
    Matrix resultado = copy_matrix(base);
    
    for (int i = 0; i < n-1; i++){
        Matrix temp = tensor_product_matrix(resultado, base);
        free_matrix(&resultado); //Evita vazamento de memória
        resultado = temp;
    }

    free_matrix(&base); //Libera a base após a finalização da matriz
    return resultado;
}

Matrix get_identity(int n){
    Matrix vazia = {NULL, 0, 0};
    if (n <= 0) return vazia;

    //Incializa a matriz identidade base
    Matrix base = {NULL, 2, 2};
    base.values = malloc(2 * sizeof *base.values); //Aloca a array externa da matriz

    //Alocas as arrays internas da matriz
    base.values[0] = calloc(2, sizeof *base.values[0]);
    base.values[1] = calloc(2, sizeof *base.values[1]);

    //Define os valores da base 2x2
    base.values[0][0] = complex_new(1.0, 0.0); 
    base.values[1][1] = complex_new(1.0, 0.0);

    //Retorna a matriz expandida para n quibits
    return expand_gate(n, base);
}

Matrix get_hadamard(int n){
    Matrix vazia = {NULL, 0, 0};
    if (n <= 0) return vazia;

    //Incializa a matriz de hadamard base
    Matrix base = {NULL, 2, 2};
    base.values = malloc(2 * sizeof *base.values); //Aloca a array externa da matriz

    //Alocas as arrays internas da matriz
    base.values[0] = calloc(2, sizeof *base.values[0]);
    base.values[1] = calloc(2, sizeof *base.values[1]);

    //Define os valores da matriz base 2x2
    base.values[0][0] = complex_new(1.0 / pow(2, 0.5), 0.0); 
    base.values[0][1] = complex_new(1.0 / pow(2, 0.5), 0.0); 
    base.values[1][0] = complex_new(1.0 / pow(2, 0.5), 0.0); 
    base.values[1][1] = complex_new(-1.0 / pow(2, 0.5), 0.0);

    //Retorna a matriz expandida para n quibits
    return expand_gate(n, base);
}

Matrix get_rotateX(int n, float theta){
    Matrix vazia = {NULL, 0, 0};
    if (n <= 0) return vazia;

    //Incializa a matriz de rotação no eixo X base
    Matrix base = {NULL, 2, 2};
    base.values = malloc(2 * sizeof *base.values); //Aloca a array externa da matriz

    //Alocas as arrays internas da matriz
    base.values[0] = calloc(2, sizeof *base.values[0]);
    base.values[1] = calloc(2, sizeof *base.values[1]);

    //Define os valores da matriz base 2x2
    base.values[0][0] = complex_new(cos(theta/2), 0.0); 
    base.values[0][1] = complex_new(0.0, -sin(theta/2)); 
    base.values[1][0] = complex_new(0.0, -sin(theta/2)); 
    base.values[1][1] = complex_new(cos(theta/2), 0.0);

    //Retorna a matriz expandida para n quibits
    return expand_gate(n, base);
}

Matrix get_pauliX(int n){
    Matrix vazia = {NULL, 0, 0};
    if (n <= 0) return vazia;

    Matrix resultado = get_rotateX(n, M_PI);
    resultado = matrix_scale_mul(resultado, complex_exp(I, n));
    return resultado;
}

Matrix get_rotateY(int n, float theta){
    Matrix vazia = {NULL, 0, 0};
    if (n <= 0) return vazia;

    //Incializa a matriz de rotação no eixo Y base
    Matrix base = {NULL, 2, 2};
    base.values = malloc(2 * sizeof *base.values); //Aloca a array externa da matriz

    //Alocas as arrays internas da matriz
    base.values[0] = calloc(2, sizeof *base.values[0]);
    base.values[1] = calloc(2, sizeof *base.values[1]);

    //Define os valores da matriz base 2x2
    base.values[0][0] = complex_new(cos(theta/2), 0.0); 
    base.values[0][1] = complex_new(-sin(theta/2), 0.0); 
    base.values[1][0] = complex_new(sin(theta/2), 0.0); 
    base.values[1][1] = complex_new(cos(theta/2), 0.0);

    //Retorna a matriz expandida para n quibits
    return expand_gate(n, base);
}

Matrix get_pauliY(int n){
    Matrix vazia = {NULL, 0, 0};
    if (n <= 0) return vazia;

    Matrix resultado = get_rotateY(n, M_PI);
    resultado = matrix_scale_mul(resultado, complex_exp(I, n));
    return resultado;
}

Matrix get_rotateZ(int n, float theta){
    Matrix vazia = {NULL, 0, 0};
    if (n <= 0) return vazia;

    //Incializa a matriz de rotação no eixo Z base
    Matrix base = {NULL, 2, 2};
    base.values = malloc(2 * sizeof *base.values); //Aloca a array externa da matriz

    //Alocas as arrays internas da matriz
    base.values[0] = calloc(2, sizeof *base.values[0]);
    base.values[1] = calloc(2, sizeof *base.values[1]);

    //Define os valores da matriz base 2x2
    base.values[0][0] = complex_new(cos(theta / 2), -sin(theta / 2)); 
    base.values[0][1] = complex_new(0.0, 0.0); 
    base.values[1][0] = complex_new(0.0, 0.0); 
    base.values[1][1] = complex_new(cos(theta / 2), sin(theta / 2));

    //Retorna a matriz expandida para n quibits
    return expand_gate(n, base);
}

Matrix get_pauliZ(int n){
    Matrix vazia = {NULL, 0, 0};
    if (n <= 0) return vazia;

    Matrix resultado = get_rotateZ(n, M_PI);
    resultado = matrix_scale_mul(resultado, complex_exp(I, n));
    return resultado;
}

Matrix get_cnot(int n, int control, int target){
    Matrix vazia = {NULL, 0, 0};
    if (n <= 1) return vazia;
    if (control == target) return vazia;
    if (control >= n || target >= n || target < 0 || control < 0) return vazia;

    //armazena o tamanho da matriz
    int tam = pow(2, n);

    /* Recolhe a matriz identidade para n qubits, pois a porta cnot é
    uma permutação da matriz identidade, sendo mais fácil usar a identidade
    como base */
    Matrix cnot = get_identity(n);
    if (cnot.values == NULL) return vazia;

    //Ajuste dos indices para notação Big-Endian utilizada na matriz
    control = n - 1 - control;
    target  = n - 1 - target;

    for (int i = 0; i < tam; i++){
        //Verifica se o qubit controle está ativado
        if ((i >> control) & 1){
            if ((i >> target) & 1){ //Só realiza a troca quando o qubit alvo for 1, evita dupla troca
                continue;
            }
            //Troca as linhas do qubit alvo
            int indice = i ^ (1 << target);
            Complex *temp = cnot.values[i];
            cnot.values[i] = cnot.values[indice];
            cnot.values[indice] = temp;
        }
    }

    return cnot;
}
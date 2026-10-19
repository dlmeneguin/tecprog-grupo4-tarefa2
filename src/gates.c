#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "complex.h"
#include "linear_algebra.h"
#include "gates.h"

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
    Matrix base = create_matrix(2, 2);
    if (base.values == NULL) return vazia;

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
    Matrix base = create_matrix(2, 2);
    if (base.values == NULL) return vazia;

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
    Matrix base = create_matrix(2, 2);
    if (base.values == NULL) return vazia;

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
    if (resultado.values == NULL) return vazia;

    Matrix temp = matrix_scale_mul(resultado, complex_exp(I, n));
    free_matrix(&resultado); //Evita vazamento de memória
    resultado = temp;

    return resultado;
}

Matrix get_rotateY(int n, float theta){
    Matrix vazia = {NULL, 0, 0};
    if (n <= 0) return vazia;

    //Incializa a matriz de rotação no eixo Y base
    Matrix base = create_matrix(2, 2);
    if (base.values == NULL) return vazia;

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
    if (resultado.values == NULL) return vazia;

    Matrix temp = matrix_scale_mul(resultado, complex_exp(I, n));
    free_matrix(&resultado); //Evita vazamento de memória
    resultado = temp;
    return resultado;
}

Matrix get_rotateZ(int n, float theta){
    Matrix vazia = {NULL, 0, 0};
    if (n <= 0) return vazia;

    //Incializa a matriz de rotação no eixo Z base
    Matrix base = create_matrix(2, 2);
    if (base.values == NULL) return vazia;

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
    if (resultado.values == NULL) return vazia;

    Matrix temp = matrix_scale_mul(resultado, complex_exp(I, n));
    free_matrix(&resultado); //Evita vazamento de memória
    resultado = temp;
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

Matrix get_cz(int n, int control, int target){
    //A construção dessa porta é Ht * cnot * Ht
    Matrix vazia = {NULL, 0, 0};
    if (n <= 1) return vazia;
    if (control == target) return vazia;
    if (control >= n || target >= n || target < 0 || control < 0) return vazia;

    //Obtém a porta CNOT entre controle e alvo
    Matrix cnot = get_cnot(n, control, target);
    if (cnot.values == NULL) return vazia;

    Matrix identidade = get_identity(1);
    if (identidade.values == NULL){
        free_matrix(&cnot); //evita vazamento de memória
        return vazia;
    }

    Matrix hadamard = get_hadamard(1);
    if (hadamard.values == NULL){
        free_matrix(&cnot); //evita vazamento de memória
        free_matrix(&identidade); //evita vazamento de memória
        return vazia;
    }

    //prepara a matriz H aplicada apenas no qubit alvo
    Matrix h_target;
    if (target == 0){
        h_target = copy_matrix(hadamard);
    } else {
        h_target = copy_matrix(identidade);
    }

    for (int i = 1; i < n; i++){
        Matrix prox;
        if (i == target){
            prox = hadamard;
        } else {
            prox = identidade;
        }

        Matrix temp = tensor_product_matrix(h_target, prox);
        free_matrix(&h_target);
        h_target = temp;
    }

    //libera as matrizes 2x2 base
    free_matrix(&identidade);
    free_matrix(&hadamard);

    //Calcula CZ = Ht * CNOT * Ht
    Matrix temp = matrix_mul(cnot, h_target);
    free_matrix(&cnot); //Já utilizamos a cnot, pode jogar fora

    Matrix resultado = matrix_mul(h_target, temp);

    //Libera as últimas matrizes criadas
    free_matrix(&temp);
    free_matrix(&h_target);

    return resultado;
}

//Controlled Controlled Not (ou Toffoli, não é o político), só inverte se os dois controles forem diferentes de 0
Matrix get_ccnot(int n, int control1, int control2, int target){
    Matrix vazia = {NULL, 0, 0};
    //Requer pelo menos 3 qubits
    if (n <= 2) return vazia;
    // Todos os qubits envolvidos devem ser distintos entre si
    if (control1 == control2 || control1 == target || control2 == target) return vazia;
    // Validação dos limites dos índices
    if (control1 >= n || control2 >= n || target >= n) return vazia;
    if (control1 < 0 || control2 < 0 || target < 0) return vazia;

    //Armazena a dimensão 2^n da matriz
    int tam = pow(2, n);

    /* Recolhe a matriz identidade para n qubits, pois a porta Toffoli (CCNOT)
       também é uma permutação das linhas da matriz identidade */
    Matrix ccnot = get_identity(n);
    if (ccnot.values == NULL) return vazia;

    // Ajuste dos índices para notação Big-Endian utilizada na matriz
    control1 = n - 1 - control1;
    control2 = n - 1 - control2;
    target   = n - 1 - target;

    //Mesma coisa da CNOT, mas checa por 2 controles
    for (int i = 0; i < tam; i++){
        // Verifica se ambos os qubits de controle estão ativados (iguais a 1)
        if (((i >> control1) & 1) && ((i >> control2) & 1)){
            // Só realiza a troca quando o qubit alvo for 1 para evitar dupla troca
            if ((i >> target) & 1){
                continue;
            }
            // Troca as linhas correspondentes à inversão do qubit alvo
            int indice = i ^ (1 << target);
            Complex *temp = ccnot.values[i];
            ccnot.values[i] = ccnot.values[indice];
            ccnot.values[indice] = temp;
        }
    }

    return ccnot;
}

//Controlled Controlled Not é literalmente a mesma coisa que a CZ, mas eu uso a CCNOT no meio em vez da CNOT
Matrix get_ccz(int n, int control1, int control2, int target){
    Matrix vazia = {NULL, 0, 0};
    //Requer pelo menos 3 qubits
    if (n <= 2) return vazia;
    // Todos os qubits envolvidos devem ser distintos entre si
    if (control1 == control2 || control1 == target || control2 == target) return vazia;
    // Validação dos limites dos índices
    if (control1 >= n || control2 >= n || target >= n) return vazia;
    if (control1 < 0 || control2 < 0 || target < 0) return vazia;

    //Obtém a porta CCNOT entre controle e alvo
    Matrix ccnot = get_ccnot(n, control1, control2, target);
    if (ccnot.values == NULL) return vazia;

    Matrix identidade = get_identity(1);
    if (identidade.values == NULL){
        free_matrix(&ccnot); //evita vazamento de memória
        return vazia;
    }

    Matrix hadamard = get_hadamard(1);
    if (hadamard.values == NULL){
        free_matrix(&ccnot); //evita vazamento de memória
        free_matrix(&identidade); //evita vazamento de memória
        return vazia;
    }

    //prepara a matriz H aplicada apenas no qubit alvo
    Matrix h_target;
    if (target == 0){
        h_target = copy_matrix(hadamard);
    } else {
        h_target = copy_matrix(identidade);
    }

    for (int i = 1; i < n; i++){
        Matrix prox;
        if (i == target){
            prox = hadamard;
        } else {
            prox = identidade;
        }

        Matrix temp = tensor_product_matrix(h_target, prox);
        free_matrix(&h_target);
        h_target = temp;
    }

    //libera as matrizes 2x2 base
    free_matrix(&identidade);
    free_matrix(&hadamard);

    //Calcula CZ = Ht * CCNOT * Ht
    Matrix temp = matrix_mul(ccnot, h_target);
    free_matrix(&ccnot); //Já utilizamos a ccnot, pode jogar fora

    Matrix resultado = matrix_mul(h_target, temp);

    //Libera as últimas matrizes criadas
    free_matrix(&temp);
    free_matrix(&h_target);

    return resultado;
}

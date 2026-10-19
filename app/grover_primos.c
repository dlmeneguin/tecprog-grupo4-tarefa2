#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "../include/circuits.h"
#include "../include/gates.h"
#include "../include/complex.h"
#include "../include/linear_algebra.h"

// Limitação: a busca funciona de 1 até 4 qubits(0-15), por limitações de memória e tempo

/* Expressão booleana para encontrar primos de 0-15: 
Sejam ABCD os dígitos de um número binário, com A mais significativo e D menos significativo:
f(A,B,C,D) = (~A * ~B * C * ~D) + (D * (B + C)) * ~(A * B * C) -> mintermos
Reorganizando:
f(A,B,C,D) = C XOR (A * C) XOR (B * C) XOR (B * D) XOR (A * B * C) XOR (A * C * D)

Para 1 qubit: f(0,0,0,D) = 0
Para 2 qubits: f(0,0,C,D) = C
Para 3 qubits: f(0,B,C,D) = C XOR (B * C) XOR (B * D)
*/

// Inverte a fase dos bits alvo
// Z * CZ funciona como um x XOR (y * z)
// O mesmo vale para CCZ, mas com 3 termos no and invés de 2
void Oracle(Vector *state, int n){
    // n = 1 tem 0 primos

    // Aplica Z no bit n-1
    if (n >= 2){
        Matrix I = get_identity(1);
        Matrix Z = get_pauliZ(1);
        if (I.values == NULL || Z.values == NULL){
            free_matrix(&I);
            free_matrix(&Z);
            return;
        }
        
        // Z tensor I é garantido
        Matrix Zn = tensor_product_matrix(Z, I);
        if (Zn.values == NULL){
            free_matrix(&I);
            free_matrix(&Z);
            return;
        }

        // Calcula I tensor (Z tensor I) para os n-2 termos sobrando
        for (int i = 2; i < n; i++){
            Matrix temp = tensor_product_matrix(I, Zn);

            free_matrix(&Zn);
            Zn = temp;

            if (Zn.values == NULL){
                free_matrix(&I);
                free_matrix(&Z);
                return;
            }
        }

        free_matrix(&I);
        free_matrix(&Z);
        apply_gate(state, Zn);
    }

    // Aplica CZ no bit n-1 e depois no n usando o bit n-2 como controle
    if (n >= 3){
        // Passamos indexes, portanto subtraimos 1
        apply_gate(state, get_cz(n, n-3, n-2));
        apply_gate(state, get_cz(n, n-3, n-1));
    }

    // Aplica CZ no bit 3 usando 1 como controle. Então, aplica-se CCZ nos bits 3 e 2, usando (1,2) e (1,3) como controles, respectivamente
    if (n >= 4){
        // Passamos indexes, portanto subtraimos 1
        apply_gate(state, get_cz(4, 0, 2));
        apply_gate(state, get_ccz(4, 0, 1, 2));
        apply_gate(state, get_ccz(4, 0, 2, 3));
    }
}

// Opera sobre as amplitudes do vetor de estado, aumentando a do bit marcado
// nova_amplitude = 2*média - amplitude
void Diffuser(Vector *state){
    Complex size = {state->len, 0};

    // Calcula a média dos estados
    Complex average = {0, 0};
    for (int i = 0; i < state->len; i++) average = complex_add(average, state->values[i]);
    average = complex_div(average, size);
    Complex double_average = complex_mul(average, complex_new(2, 0));

    // Muda o vetor de estado para as novas amplitudes
    for (int i = 0; i < state->len; i++){
        state->values[i] = complex_add(double_average, complex_mul(state->values[i], complex_new(-1, 0)));   
    }
}

// Calcula a soma de probabilidades teórica nos bit marcados
double probabilities(const Vector *state){
    int primes[] = {2, 3, 5, 7, 11, 13}; // Primos de 0 a 15
    double probability = 0.0;

    for (int i = 0; i < 6; i++){
        if (primes[i] >= state->len) break; // Para o loop para primos fora do intervalo do número de qubits atual

        probability += complex_mod2(state->values[primes[i]]); // Probabilidade de z é z^2
    }

    return probability;
}

// Printa o vetor de estado num formato mais agradável
void print_state(const Vector *state){
    printf("[%lf + %lfi, ", state->values[0].re, state->values[0].im);
    for (int i = 1; i < state->len - 1; i++) printf("%lf + %lfi, ", state->values[i].re, state->values[i].im);
    printf("%lf + %lfi]\n", state->values[state->len - 1].re, state->values[state->len - 1].im);
}

int main(){
    generate_seed();

    // Recebe a quantidade de qubits e medições que se deseja utilizar
    int qubits, shots;
    scanf("%d %d", &qubits, &shots);
    if (qubits < 1 || qubits > 4){
        printf("ERRO: quantidade de qubits fora do intervalo [1, 4]\n");
        return 1;
    }
    if (shots < 0){
        printf("ERRO: quantidade de shots < 0\n");
        return 1;
    }

    // Gera o vetor de estado 0
    Vector state = init_qubits(qubits);
    if (state.values == NULL) return 1;
    print_state(&state);

    // Aplica Hadamard pra equalizar as probabilidades de estado
    Matrix H = get_hadamard(qubits);
    if (H.values == NULL){
        free_vector(&state);
        return 1;
    }
    Vector equalized = matrix_vec_mul(H, state);
    if (equalized.values == NULL){
        free_vector(&state);
        free_matrix(&H);
        return 1;
    }
    free_matrix(&H);
    free_vector(&state);
    print_state(&equalized);

    int primesperQubit[] = {0, 0, 2, 4, 6};
    int primes[] = {2, 3, 5, 7, 11, 13};

    /* Fórmula para calcular o pico de probabilidade após k iterações de Oráculo + Difusor */
    double theta = asin(sqrt((double) primesperQubit[qubits]/(1<<qubits)));
    double probability = 0;
    double aux = 0;
    int k = 0; // Maior pico encontrado em até 5 iterações
    for (int i = 0; i < 5; i++){
        aux = pow(sin((2*i + 1)*theta), 2);
        if (aux > probability + 1e-12){ // Faz com que a menor quantidade de iterações seja guardada
            probability = aux;
            k = i;
        }
    }
    
    // Aplica o oráculo e o difusor k vezes, garantindo alta probabilidade
    for (int i = 0; i < k; i++){
        Oracle(&equalized, qubits);
        Diffuser(&equalized);
    }
    print_state(&equalized);

    /* Calcula um valor experimental de probabilidade */
    int *experimentalProbabilities = measure_shots(&equalized, shots);
    if (experimentalProbabilities == NULL){
        free_vector(&equalized);
        return 1;
    }
    int measured = 0;
    for (int i = 0; i < 6; i++){
        // Garante que está no intervalo apropriado e adiciona a quantidade de vezes que foi medido um primo
        if (primes[i] < equalized.len) measured += experimentalProbabilities[primes[i]];
    }
    double experimental = (double) measured/shots;
    free(experimentalProbabilities);

    /* Compara o valor experimental com o valor téorico de probabilidade do vetor de estado */
    printf("Valor experimental: %.4lf%%\n", experimental * 100);
    printf("Valor teórico: %.4lf%%\n", probabilities(&equalized) * 100);

    free_vector(&equalized);
    return 0;
}
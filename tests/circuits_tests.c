#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "linear_algebra.h"
#include "gates.h"
#include "circuits.h"

// Verifica se dois complexos são iguais com margem de erro de 1e-6
static int check_complex(Complex a, Complex b) {
    return fabs(a.re - b.re) < 1e-6 && fabs(a.im - b.im) < 1e-6;
}

// Constrói o estado (|00> + |10>)/sqrt(2): Hadamard apenas no primeiro qubit de 2.
// Os índices 1 e 3 têm probabilidade 0 (usado para testar estados impossíveis)
static Vector make_h_first_qubit_state() {
    Vector s = init_qubits(2);
    Matrix h = get_hadamard(1);
    Matrix id = get_identity(1);
    Matrix h_id = tensor_product_matrix(h, id);
    free_matrix(&h);
    free_matrix(&id);

    apply_gate(&s, h_id); //apply_gate já libera a porta h_id
    return s;
}

// Constrói o estado de Bell (|00> + |11>)/sqrt(2): Hadamard no qubit 0 e CNOT(0, 1)
static Vector make_bell_state() {
    Vector s = make_h_first_qubit_state();
    apply_gate(&s, get_cnot(2, 0, 1));
    return s;
}

void test_generate_seed() {
    // Compara se generate_seed() efetivamente altera rand()
    srand(1);
    int sem_seed = rand();

    srand(1);
    generate_seed();
    int com_seed = rand();

    assert(sem_seed != com_seed);
}

void test_init_qubits() {
    // Casos inválidos, sendo n <= 0
    Vector s = init_qubits(0);
    assert(s.values == NULL);
    assert(s.len == 0);

    s = init_qubits(-3);
    assert(s.values == NULL);
    assert(s.len == 0);

    // n = 1: |0> = (1, 0)
    s = init_qubits(1);
    assert(s.values != NULL);
    assert(s.len == 2);
    assert(check_complex(s.values[0], complex_new(1.0, 0.0))); // O 1º elemento do vetor é 1
    assert(check_complex(s.values[1], complex_new(0.0, 0.0))); // O 2º elemento do vetor é 0
    free_vector(&s);

    // Caso genérico, n = 3: 8 amplitudes, sendo somente a primeira 1
    s = init_qubits(3);
    assert(s.values != NULL);
    assert(s.len == 8);
    assert(check_complex(s.values[0], complex_new(1.0, 0.0)));
    for (int i = 1; i < s.len; i++){
        assert(check_complex(s.values[i], complex_new(0.0, 0.0)));
    }
    free_vector(&s);
}

// Testa casos válidos
void test_apply_gate(void) {
    // Pauli X em |0> resulta em |1>
    Vector s = init_qubits(1);
    apply_gate(&s, get_pauliX(1));
    assert(s.len == 2);
    assert(check_complex(s.values[0], complex_new(0.0, 0.0)));
    assert(check_complex(s.values[1], complex_new(1.0, 0.0)));
    free_vector(&s);

    // Hadamard em |0> resulta em (|0> + |1>)/sqrt(2)
    double inv_sqrt2 = 1.0 / sqrt(2.0);
    s = init_qubits(1);
    apply_gate(&s, get_hadamard(1));
    assert(check_complex(s.values[0], complex_new(inv_sqrt2, 0.0)));
    assert(check_complex(s.values[1], complex_new(inv_sqrt2, 0.0)));

    // Aplicar Hadamard duas vezes volta para |0>
    apply_gate(&s, get_hadamard(1));
    assert(check_complex(s.values[0], complex_new(1.0, 0.0)));
    assert(check_complex(s.values[1], complex_new(0.0, 0.0)));
    free_vector(&s);

    //Identidade não altera o estado, n = 2
    s = init_qubits(2);
    apply_gate(&s, get_identity(2));
    assert(s.len == 4);
    assert(check_complex(s.values[0], complex_new(1.0, 0.0)));
    for (int i = 1; i < s.len; i++){
        assert(check_complex(s.values[i], complex_new(0.0, 0.0)));
    }
    free_vector(&s);

    //Estado de Bell: amplitudes 1/sqrt(2) em |00> e |11>
    s = make_bell_state();
    assert(s.len == 4);
    assert(check_complex(s.values[0], complex_new(inv_sqrt2, 0.0)));
    assert(check_complex(s.values[1], complex_new(0.0, 0.0)));
    assert(check_complex(s.values[2], complex_new(0.0, 0.0)));
    assert(check_complex(s.values[3], complex_new(inv_sqrt2, 0.0)));
    free_vector(&s);
}

// Testa casos inválidos, que não alteram o vetor de estados e não quebram a função
void test_apply_gate_invalid(void) {
    // Porta com tamanho incompatível (4x4 em um estado de 1 qubit)
    Vector s = init_qubits(1);
    apply_gate(&s, get_identity(2));
    assert(s.values != NULL);
    assert(s.len == 2);
    assert(check_complex(s.values[0], complex_new(1.0, 0.0)));
    assert(check_complex(s.values[1], complex_new(0.0, 0.0)));

    // Porta vazia
    Matrix gate_vazia = {NULL, 0, 0};
    apply_gate(&s, gate_vazia);
    assert(s.values != NULL);
    assert(s.len == 2);
    assert(check_complex(s.values[0], complex_new(1.0, 0.0)));
    free_vector(&s);

    // Estado vazio
    Vector vazio = {NULL, 0};
    apply_gate(&vazio, get_identity(1));
    assert(vazio.values == NULL);
    assert(vazio.len == 0);

    // Ponteiro NULL para o estado
    apply_gate(NULL, get_identity(1));
}

void test_measure(void) {
    srand(42); // Seed fixa para o teste ser reprodutível

    // Caso inválido, vetor vazio
    Vector vazio = {NULL, 0};
    assert(measure(&vazio) == -1);

    // |0> sempre é medido como 0
    Vector s = init_qubits(2);
    for (int i = 0; i < 100; i++){
        assert(measure(&s) == 0);
    }
    free_vector(&s);

    // PauliX em |0> resulta em |1>, sempre medido como 1
    s = init_qubits(1);
    apply_gate(&s, get_pauliX(1));
    for (int i = 0; i < 100; i++){
        assert(measure(&s) == 1);
    }
    free_vector(&s);

    // Estado com índices de probabilidade 0 (1 e 3): só 0 e 2 podem aparecer, e ambos devem aparecer
    s = make_h_first_qubit_state();
    int viu_0 = 0, viu_2 = 0;
    for (int i = 0; i < 1000; i++){
        int m = measure(&s);
        assert(m == 0 || m == 2);
        if (m == 0) viu_0 = 1;
        if (m == 2) viu_2 = 1;
    }
    assert(viu_0 && viu_2);

    // A medição não colapsa, ou seja, as amplitudes continuam as mesmas
    double inv_sqrt2 = 1.0 / sqrt(2.0);
    assert(check_complex(s.values[0], complex_new(inv_sqrt2, 0.0)));
    assert(check_complex(s.values[1], complex_new(0.0, 0.0)));
    assert(check_complex(s.values[2], complex_new(inv_sqrt2, 0.0)));
    assert(check_complex(s.values[3], complex_new(0.0, 0.0)));
    free_vector(&s);

    // Nesse vetor, a soma das probabilidades de cada estado é 0,25; assim, caso o
    // número sorteado seja maior do que a probabilidade total, retorna-se o último valor não nulo
    s = create_vector(4);
    s.values[1] = complex_new(0.5, 0.0);
    for (int i = 0; i < 100; i++) {
        assert(measure(&s) == 1);
    }
    free_vector(&s);

    // Caso limite: para um vetor preenchido somente com 0, retorna -1
    s = create_vector(4);
    assert(measure(&s) == -1);
    free_vector(&s);
}

// Teste que falhava inicialmente: measure_shots somava o índice do estado medido (result[i] += measured)
// ao invés de incrementar a contagem do estado medido (result[measured]++)

// Com |1> e 2 shots, o resultado esperado é {0, 2}, mas a versão com erro retornava {1, 1}; além disso,
// como i podia ser maior que o comprimento do vetor de estados, poderia haver extrapolamento do limite do vetor
void test_measure_shots_regression() {
    srand(42);

    Vector s = init_qubits(1);
    apply_gate(&s, get_pauliX(1)); //Estado |1>, pois aplicar PauliX em |0> resulta em |1>

    int *contagem = measure_shots(&s, 2);
    assert(contagem != NULL);
    assert(contagem[0] == 0);
    assert(contagem[1] == 2);

    free(contagem);
    free_vector(&s);
}

void test_measure_shots() {
    srand(42);

    // Caso inválido, vetor vazio
    Vector vazio = {NULL, 0};
    assert(measure_shots(&vazio, 10) == NULL);

    // Caso inválido, shots negativo
    Vector s = init_qubits(1);
    assert(measure_shots(&s, -1) == NULL);

    // Caso limite, shots = 0: vetor de contagem todo zerado
    int *contagem = measure_shots(&s, 0);
    assert(contagem != NULL);
    assert(contagem[0] == 0);
    assert(contagem[1] == 0);
    free(contagem);

    // |0> com mais shots do que estados: todas as medições caem no estado 0
    contagem = measure_shots(&s, 100);
    assert(contagem != NULL);
    assert(contagem[0] == 100);
    assert(contagem[1] == 0);
    free(contagem);

    // Hadamard em |0>: ~50% para cada estado (tolerância ampla de 5 desvios padrão)
    apply_gate(&s, get_hadamard(1));
    contagem = measure_shots(&s, 10000);
    assert(contagem != NULL);
    assert(contagem[0] + contagem[1] == 10000);
    // Se contagem[0] está nesse intervalo, então contagem[1] também está
    assert(contagem[0] > 4750 && contagem[0] < 5250);
    free(contagem);
    free_vector(&s);

    // Estado de Bell: só |00> (índice 0) e |11> (índice 3) podem ser medidos
    s = make_bell_state();
    contagem = measure_shots(&s, 1000);
    assert(contagem != NULL);
    assert(contagem[1] == 0);
    assert(contagem[2] == 0);
    assert(contagem[0] + contagem[3] == 1000);
    assert(contagem[0] > 400 && contagem[0] < 600);
    free(contagem);

    // A medição não altera o estado
    assert(check_complex(s.values[0], complex_new(1.0 / sqrt(2.0), 0.0)));
    assert(check_complex(s.values[3], complex_new(1.0 / sqrt(2.0), 0.0)));
    free_vector(&s);
}

int main() {
    printf("Iniciando os testes do módulo de circuitos quânticos\n");

    test_generate_seed();
    printf("função generate_seed OK\n");

    test_init_qubits();
    printf("função init_qubits OK\n");

    test_apply_gate();
    test_apply_gate_invalid();
    printf("função apply_gate OK\n");

    test_measure();
    printf("função measure OK\n");

    test_measure_shots_regression();
    test_measure_shots();
    printf("função measure_shots OK\n");

    printf("\nTodos os testes do módulo de circuitos quânticos passaram!\n");
    return 0;
}

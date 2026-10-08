#ifndef complex_h // Condicional de pré-processamento
#define complex_h // Inicio do pré-processamento

typedef struct {
    double re; // Parte real
    double im; // Parte imaginária
} Complex; // Nome do struct de números complexos

Complex complex_new(double re, double im); // Construtor do número complexo
Complex complex_add(Complex a, Complex b); // Função de adição de números complexos (soma e subtração)
Complex complex_mul(Complex a, Complex b); // Função de multiplicação de números complexos
Complex complex_div(Complex a, Complex b); // Função de divisão de números complexos por números complexos
Complex complex_exp(Complex a, int e); // Função de exponenciação de complexos
double complex_mod2(Complex a); // Função que retorna o quadrado do módulo do número complexo
int complex_equal(Complex a, Complex b); // Verifica igualdade entre a e b (1 == true e 0 == false)

#endif

#ifndef complex_h // Condicional de pré-processamento
#define complex_h // Inicio do pré-processamento

typedef struct {
    double re; // Parte real
    double im; // Parte imaginária
} Complex; // Nome do struct de números complexos

Complex complex_new(double re, double im); // Construtor do número complexo
Complex complex_add(Complex a, Complex b); // Função de adição de números complexos (soma e subtração)
Complex complex_mul(Complex a, Complex b); // Função de multiplicação de números complexos
double complex_mod2(Complex z); // Função que retorna o quadrado do módulo do número complexo

#endif

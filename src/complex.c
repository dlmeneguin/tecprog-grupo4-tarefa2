#include "complex.h"

const Complex I = {0.0, 1.0}; // Constante imaginária i

// Cria um novo número complexo
Complex complex_new(double re, double im){ // Parte real (re) e imaginária (im)
    Complex z = {re, im}; // Declara o número complexo z = re + i*im, representado por dois doubles re e im
    return z; // Retorna o número complexo z criado
}

// Adiciona números complexos a e b (soma e subtração)
Complex complex_add(Complex a, Complex b){ // Números complexos a e b
    double re = a.re + b.re; // Parte real (re) igual a soma das partes reais de a (a.re) e b (b.re)
    double im = a.im + b.im; // Parte imaginária (im) igual a soma das partes imaginárias de a (a.im) e b (b.im)
    return complex_new(re, im); // Retorna um novo complexo resultado da soma, de parte real (re) e parte imaginária (im)
}

// Multiplica números complexos a e b
Complex complex_mul(Complex a, Complex b){ // Números complexos a e b
    double re = (a.re * b.re) - (a.im * b.im); // Parte real (re) resultante do produto a*b = (a.re + a.im * i)*(b.re + b.im * i)
    double im = (a.re * b.im) + (a.im * b.re); // Parte imaginária (im) resultante do produto a*b = (a.re + a.im * i)*(b.re + b.im * i)
    return complex_new(re, im); // Retorna um novo complexo resultado do produto, de parte real (re) e parte imaginária (im)
}

// Divide o número complexo a pelo número complexo b
Complex complex_div(Complex a, Complex b){ // Números complexos a e b
    double den = complex_mod2(b); // Preicsa de den != 0, é igual ao quadrado do módulo de b
    double re = (a.re*b.re + a.im*b.im)/den; // Parte real (re) resultante do produto a*conjugado de b = (a.re + a.im * i)*(b.re - b.im * i)
    double im = (a.im*b.re - a.re*b.im)/den; // Parte imaginária (im) resultante do produto a*conjugado de b = (a.re + a.im * i)*(b.re - b.im * i)
    return complex_new(re, im); // Retorna um novo complexo resultado da divisão, de parte real (re) e parte imaginária (im)
}

// Exponencia o número complexo a por 'e'
Complex complex_exp(Complex a, int e){ // Número complexo 'a' e exponente 'e'
    if (e == 0) return complex_new(1, 0); // Caso base 'e' == 0, retorna 1
    Complex z = complex_new(a.re, a.im); // Inicializa o novo complexo z
    for (int i=1; i<e; i++){ // Multiplica o complexo z atual por a, 'e' vezes
        z = complex_mul(z, a);
    }
    return z; // Retorna o novo complexo z resultado de a^e
}

// Calcula o quadrado do módulo do número complexo a
double complex_mod2(Complex a){ // Número complexo a
    return (a.re*a.re + a.im*a.im); // Retorna o quadrado do módulo de a
}

// Verifica igualdade entre dois complexos a e b
int complex_equal(Complex a, Complex b){ // Números complexos a e b
    if (a.re == b.re && a.im == b.im) return 1; // Retorna 1 se a == b
    else return 0; // Retorna 0 se a != b
}

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "../include/complex.h"

//Função para verificar se os complexos são iguais na medida do possível, i.e.,
// com margem de erro até a ordem de 1e-9. É análogo ao complex_equal em complex.c,
// mas não depende da função sendo testada e permite erros de arredondamento (comparação não exata).
int check_complex(Complex a, Complex b){
    return fabs(a.re - b.re) < 1e-9 && fabs(a.im - b.im) < 1e-9;
}

void test_complex_new(void){
    //Caso genérico
    Complex z = complex_new(3.0, -4.0);
    assert(z.re == 3.0);
    assert(z.im == -4.0);

    //Caso de limite, z == 0
    z = complex_new(0.0, 0.0);
    assert(z.re == 0.0);
    assert(z.im == 0.0);
}

void test_constante_I(void){
    //I = 0 + 1i
    assert(I.re == 0.0);
    assert(I.im == 1.0);

    //Teste de identidade de i: I^2 = -1
    assert(check_complex(complex_mul(I, I), complex_new(-1.0, 0.0)));
}

void test_complex_add(void){
    Complex a = complex_new(1.0, 2.0);
    Complex b = complex_new(3.0, -1.0);

    //Caso genérico
    assert(check_complex(complex_add(a, b), complex_new(4.0, 1.0)));

    //Comutatividade
    assert(check_complex(complex_add(a, b), complex_add(b, a)));

    //Caso limite, somar zero
    assert(check_complex(complex_add(a, complex_new(0.0, 0.0)), a));

    //Caso limite, somar o conjugado
    assert(check_complex(complex_add(a, complex_new(-1.0, -2.0)), complex_new(0.0, 0.0)));
}

void test_complex_mul(void){
    Complex a = complex_new(1.0, 2.0);
    Complex b = complex_new(3.0, -1.0);

    //Caso genérico, (1+2i)(3-i) = 5+5i
    assert(check_complex(complex_mul(a, b), complex_new(5.0, 5.0)));

    //Comutatividade
    assert(check_complex(complex_mul(a, b), complex_mul(b, a)));

    //Caso limite, multiplicar por 1
    assert(check_complex(complex_mul(a, complex_new(1.0, 0.0)), a));

    //Caso limite, multiplicar por 0
    assert(check_complex(complex_mul(a, complex_new(0.0, 0.0)), complex_new(0.0, 0.0)));
}

void test_complex_div(void){
    Complex a = complex_new(1.0, 2.0);
    Complex b = complex_new(3.0, -1.0);

    //Caso genérico, (1+2i)/(3-i) = 0.1+0.7i
    assert(check_complex(complex_div(a, b), complex_new(0.1, 0.7)));

    //Caso genérico, (5+5i)/(3-i) = 1+2i
    assert(check_complex(complex_div(complex_new(5.0, 5.0), b), a));

    //Caso limite, dividir por 1
    assert(check_complex(complex_div(a, complex_new(1.0, 0.0)), a));

    //Caso limite, a/a = 1
    assert(check_complex(complex_div(a, a), complex_new(1.0, 0.0)));

    //Caso inválido, divisão por zero deve resultar em infinito ou NaN
    Complex r = complex_div(a, complex_new(0.0, 0.0));
    assert(!isfinite(r.re) || !isfinite(r.im));
}

void test_complex_exp(void){
    Complex a = complex_new(1.0, 1.0);

    //Caso limite, e = 0 resulta em 1
    assert(check_complex(complex_exp(a, 0), complex_new(1.0, 0.0)));
    assert(check_complex(complex_exp(complex_new(0.0, 0.0), 0), complex_new(1.0, 0.0)));

    //Caso limite, e = 1 resulta na própria base
    assert(check_complex(complex_exp(a, 1), a));

    //Caso genérico, (1+i)^2 = 2i
    assert(check_complex(complex_exp(a, 2), complex_new(0.0, 2.0)));

    //Caso genérico, (1+i)^4 = -4
    assert(check_complex(complex_exp(a, 4), complex_new(-4.0, 0.0)));

    //Potências de I: I^2 = -1 e I^4 = 1
    assert(check_complex(complex_exp(I, 2), complex_new(-1.0, 0.0)));
    assert(check_complex(complex_exp(I, 4), complex_new(1.0, 0.0)));
}

void test_complex_mod2(void){
    //Caso genérico, |3+4i|^2 = 25
    assert(complex_mod2(complex_new(3.0, 4.0)) == 25.0);

    //Sinais não alteram o módulo
    assert(complex_mod2(complex_new(-3.0, -4.0)) == 25.0);

    //Caso limite, zero
    assert(complex_mod2(complex_new(0.0, 0.0)) == 0.0);

    //|I|^2 = 1
    assert(complex_mod2(I) == 1.0);
}

void test_complex_equal(void){
    Complex a = complex_new(1.0, 2.0);

    //Complexos iguais
    assert(complex_equal(a, complex_new(1.0, 2.0)) == 1);
    assert(complex_equal(complex_new(0.0, 0.0), complex_new(0.0, 0.0)) == 1);

    //Parte real diferente
    assert(complex_equal(a, complex_new(2.0, 2.0)) == 0);

    //Parte imaginária diferente
    assert(complex_equal(a, complex_new(1.0, 3.0)) == 0);

    //Partes trocadas
    assert(complex_equal(a, complex_new(2.0, 1.0)) == 0);
}

int main(void){
    printf("Iniciando os testes do módulo de números complexos\n");

    test_constante_I();
    printf("constante I OK\n");

    test_complex_new();
    printf("função complex_new OK\n");

    test_complex_add();
    printf("função complex_add OK\n");

    test_complex_mul();
    printf("função complex_mul OK\n");

    test_complex_div();
    printf("função complex_div OK\n");

    test_complex_exp();
    printf("função complex_exp OK\n");

    test_complex_mod2();
    printf("função complex_mod2 OK\n");

    test_complex_equal();
    printf("função complex_equal OK\n");

    printf("\nTodos os testes do módulo de números complexos passaram\n");
    return 0;
}

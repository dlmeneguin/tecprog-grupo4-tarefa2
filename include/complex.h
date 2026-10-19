/**
 * @file complex.h
 * @brief Interface do módulo de números complexos.
 *
 * Define o tipo ::Complex e as operações de criação, soma, multiplicação,
 * divisão, exponenciação inteira, módulo ao quadrado e comparação.
 */

#ifndef complex_h // Condicional de pré-processamento
#define complex_h // Inicio do pré-processamento

/**
 * @brief Número complexo @f$ z = re + im \cdot i @f$.
 */
typedef struct {
    double re; // Parte real
    double im; // Parte imaginária
} Complex; // Nome do struct de números complexos

/**
 * @brief Unidade imaginária @f$ i = 0 + 1i @f$.
 *
 * Definida em complex.c.
 */
extern const Complex I;

/**
 * @brief Cria um novo número complexo.
 *
 * @param re Parte real.
 * @param im Parte imaginária.
 * @return O número complexo @f$ re + im \cdot i @f$.
 */
Complex complex_new(double re, double im); // Construtor do número complexo

/**
 * @brief Soma dois números complexos.
 *
 * @param a Primeira parcela.
 * @param b Segunda parcela.
 * @return O número complexo @f$ a + b @f$.
 */
Complex complex_add(Complex a, Complex b); // Função de adição de números complexos

/**
 * @brief Multiplica dois números complexos.
 *
 * @param a Primeiro fator.
 * @param b Segundo fator.
 * @return O número complexo @f$ a \cdot b @f$.
 */
Complex complex_mul(Complex a, Complex b); // Função de multiplicação de números complexos

/**
 * @brief Divide o número complexo @p a pelo número complexo @p b.
 *
 * @pre @p b não pode ser zero, i.e. @c complex_mod2(b) @c != @c 0.
 *      A função não verifica essa condição.
 *
 * @warning Se @p b for zero, o resultado contém @c inf ou @c NaN.
 *
 * @param a Dividendo.
 * @param b Divisor.
 * @return O número complexo @f$ a / b @f$.
 *
 * @see complex_mod2
 */
Complex complex_div(Complex a, Complex b); // Função de divisão de números complexos por números complexos

/**
 * @brief Eleva o número complexo @p a ao expoente inteiro @p e.
 *
 * @pre @p e >= 0. Para @p e negativo a função retorna @p a, que não é
 *      o valor matemático de @f$ a^e @f$.
 *
 * @param a Base.
 * @param e Expoente inteiro não negativo.
 * @return O número complexo @f$ a^e @f$ (ou @c 1+0i se @p e == 0).
 */
Complex complex_exp(Complex a, int e); // Função de exponenciação de complexos

/**
 * @brief Calcula o quadrado do módulo de um número complexo.
 *
 * @param a Número complexo.
 * @return O valor de @f$ |a|^2 = a_r^2 + a_i^2 @f$, sem usar raiz quadrada.
 */
double complex_mod2(Complex a); // Função que retorna o quadrado do módulo do número complexo

/**
 * @brief Verifica se dois números complexos são iguais.
 *
 * A comparação é exata (@c ==) nas duas partes, então resultados de
 * contas com @c double podem diferir por erro de arredondamento.
 *
 * @param a Primeiro número complexo.
 * @param b Segundo número complexo.
 * @retval 1 Se @c a.re == b.re e @c a.im == b.im.
 * @retval 0 Caso contrário.
 */
int complex_equal(Complex a, Complex b); // Verifica igualdade entre a e b (1 == true e 0 == false)

#endif

/**
* @brief Expande a matriz fornecida para a utilização em n qubits com tensor product
* @param n Número de qubits que aquela porta será aplicada, expoente do 2 no tamanho da matriz (2^n x 2^n)
* @return Objeto Matrix da matriz expandida
* @pre n maior que 0
* @note a função não libera a memória da matriz retornada, função principalmente de uso interno
*/
Matrix expand_gate(int n, Matrix base);

/**
* @brief Retorna a matriz identidade de tamanho (2^n x 2^n).
* @param n número de qubits que essa porta será aplicada
* @return Objeto Matrix da matriz gerada
* @pre n maior que 0
* @note a função não libera a memória da matriz
*/
Matrix get_identity(int n);

/**
* @brief Retorna a porta de hadamard de tamanho (2^n x 2^n).
* @param n número de qubits que essa porta será aplicada
* @return Objeto Matrix da matriz gerada
* @pre n maior que 0
* @note a função não libera a memória da matriz
*/
Matrix get_hadamard(int n);

/**
* @brief Retorna a matriz de rotação no eixo X de tamanho (2^n x 2^n).
* @param n número de qubits que essa matriz será aplicada
* @param theta ângulo que deve ser utilizado na rotação
* @return objeto Matrix da matriz gerada
* @pre n maior que 0
* @note a função não libera a memória da matriz
*/
Matrix get_rotateX(int n, float theta);

/**
* @brief Retorna a porta de Pauli X de tamanho (2^n x 2^n).
* @param n número de qubits que essa porta será aplicada.
* @return Objeto Matrix da matriz gerada
* @pre n maior que 0
* @note a função não libera a memória da matriz
*/
Matrix get_pauliX(int n);

/**
* @brief Retorna a matriz de rotação no eixo Y de tamanho (2^n x 2^n).
* @param n número de qubits que essa matriz será aplicada
* @param theta ângulo que deve ser utilizado na rotação
* @return objeto Matrix da matriz gerada
* @pre n maior que 0
* @note a função não libera a memória da matriz
*/
Matrix get_rotateY(int n, float theta);

/**
* @brief Retorna a porta de Pauli Y de tamanho (2^n x 2^n).
* @param n número de qubits que essa porta será aplicada.
* @return Objeto Matrix da matriz gerada
* @pre n maior que 0
* @note a função não libera a memória da matriz
*/
Matrix get_pauliY(int n);

/**
* @brief Retorna a matriz de rotação no eixo Z de tamanho (2^n x 2^n).
* @param n número de qubits que essa matriz será aplicada
* @param theta ângulo que deve ser utilizado na rotação
* @return objeto Matrix da matriz gerada
* @pre n maior que 0
* @note a função não libera a memória da matriz
*/
Matrix get_rotateZ(int n, float theta);

/**
* @brief Retorna a porta de Pauli Z de tamanho (2^n x 2^n).
* @param n número de qubits que essa porta será aplicada.
* @return Objeto Matrix da matriz gerada
* @pre n maior que 0
* @note a função não libera a memória da matriz
*/
Matrix get_pauliZ(int n);

/**
* @brief Retorna a matriz Controlled Not de tamanho (2^n x 2^n).
* @param n número de qubits que essa porta será aplicada
* @param control variável que define o indice do qubit de controle da porta
* @param target variável que define o indice qubit alvo da porta
* @return objeto Matrix da matriz gerada
* @pre n maior ou igual a 2
* @pre control diferente de target
* @pre 0 <= control < n e 0 <= target < n
* @note a função não libera a memória da matriz
*/
Matrix get_cnot(int n, int control, int target);
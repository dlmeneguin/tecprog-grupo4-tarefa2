# Simulador de Computação Quântica em C

Biblioteca modular em linguagem C desenvolvida para modelagem, simulação e execução de circuitos quânticos e operações do modelo de circuito quântico.

O projeto disponibiliza uma interface pública bem definida, com suporte a números complexos, operações de álgebra linear, portas quânticas de múltiplos qubits e simulação de circuitos com medições probabilísticas.

O repositório também inclui uma aplicação cliente demonstrativa baseada no Algoritmo de Busca de Grover para identificação de números primos, compilável com ligação estática e dinâmica.

---

## Créditos

- Daniel Lima Meneguin, 17977130: Implementação do módulo de portas quânticas, gerenciamento do git, correções e padronizações gerais, escrita do README e escrita geral do relatório.
- João Taveira Hugenin, 17972294: Implementação do módulo de números complexos, implementação completa do Make com makefile, confecção integral do vídeo de demonstração e escrita parcial do relatório.
- Matheus Sousa Roseira, 16910742: Implementação base do módulo de circuitos quânticos, os testes de seu módulo cobrem também os testes do projeto, criação dos slides.
- Tomás Pontes Monteiro de Carvalho, 17926053: Implementação do módulo de álgebra linear, implementação parcial do módulo de circuitos quânticos, implementação da aplicação cliente e escrita parcial do relatório.

---

## Pré-requisitos

Para compilar, testar e executar o projeto, são necessárias as seguintes ferramentas em ambiente Linux (ou WSL no Windows):

- `GCC` (GNU Compiler Collection, com suporte a C99 ou superior)
- `GNU Make` (versão 4.0 ou superior)
- `Git` (para controle de versão)
- `ar` (utilitário do GNU binutils para criação de bibliotecas estáticas)
- Biblioteca matemática padrão do C (`libm`)

---

## Estrutura do Projeto

O repositório é organizado de forma modular, mantendo separação estrita entre interface pública, implementação interna, aplicação cliente e suíte de testes:

```text
tecprog-grupo4-tarefa2/
├── include/                 # Interface pública da biblioteca (arquivos .h)
│   ├── circuits.h           # API de circuitos e medição quântica
│   ├── complex.h            # API de números complexos
│   ├── gates.h              # API de portas quânticas
│   └── linear_algebra.h     # API de vetores, matrizes e produtos tensoriais
├── src/                     # Implementação interna da biblioteca (arquivos .c)
│   ├── circuits.c           # Implementação das rotinas de circuitos
│   ├── complex.c            # Implementação de números complexos
│   ├── gates.c              # Implementação das matrizes de portas quânticas
│   └── linear_algebra.c     # Implementação das operações de álgebra linear
├── app/                     # Aplicação cliente independente
│   └── main.c               # Ponto de entrada padrão da aplicação cliente
├── tests/                   # Bateria de testes automatizados
│   ├── circuits_tests.c     # Testes do módulo de circuitos
│   ├── complex_tests.c      # Testes do módulo de números complexos
│   ├── linear_algebra_tests.c # Testes do módulo de álgebra linear
│   └── quantum_gates_tests.c  # Testes do módulo de portas quânticas
├── lib/                     # Bibliotecas geradas (.a e .so)
├── bin/                     # Executáveis gerados da aplicação
├── build/                   # Objetos intermediários (.o) e dependências (.d)
├── Makefile                 # Automação de compilação, testes e instalação
├── CONTRIBUTING.md          # Diretrizes de contribuição e fluxo Git
└── README.md                # Documentação principal do projeto
```

---

## Módulos da Biblioteca

A biblioteca é dividida em 4 módulos funcionalmente distintos:

### 1. Números Complexos (`include/complex.h` e `src/complex.c`)
Responsável pela estrutura de números complexos (`Complex`) e operações aritméticas:
- `complex_new`: inicialização com parte real e imaginária
- `complex_add`: soma de dois números complexos
- `complex_mul`: multiplicação de números complexos
- `complex_div`: divisão com tratamento preventivo
- `complex_exp`: exponenciação inteira não negativa
- `complex_mod2`: cálculo do quadrado do módulo (|z|²)
- `complex_equal`: verificação de igualdade

### 2. Álgebra Linear (`include/linear_algebra.h` e `src/linear_algebra.c`)
Responsável pela manipulação de estados (`Vector`) e operadores (`Matrix`):
- `create_vector` e `free_vector`: alocação e desalocação de vetores dinâmicos
- `create_matrix`, `free_matrix` e `copy_matrix`: gerenciamento de matrizes
- `tensor_product_vec`: produto tensorial de Kronecker entre vetores
- `tensor_product_matrix`: produto tensorial de Kronecker entre matrizes
- `matrix_scale_mul`: multiplicação de matriz por escalar complexo
- `matrix_mul`: produto matricial
- `matrix_vec_mul`: produto matriz-vetor

### 3. Portas Quânticas (`include/gates.h` e `src/gates.c`)
Responsável pela geração e expansão de portas unitárias para sistemas de `n` qubits:
- Portas de 1 qubit: Identidade (`get_identity`), Hadamard (`get_hadamard`), Pauli X (`get_pauliX`), Pauli Y (`get_pauliY`), Pauli Z (`get_pauliZ`)
- Portas parametrizadas de rotação: `get_rotateX`, `get_rotateY`, `get_rotateZ`
- Portas controladas de 2 qubits: CNOT (`get_cnot`), CZ (`get_cz`)
- Portas controladas de 3 qubits: Toffoli / CCNOT (`get_ccnot`), CCZ (`get_ccz`)
- Expansão tensorial genérica: `expand_gate`

### 4. Circuitos Quânticos e Medição (`include/circuits.h` e `src/circuits.c`)
Responsável pela montagem e execução do circuito quântico:
- `generate_seed`: inicialização do gerador pseudoaleatório
- `init_qubits`: inicialização do registrador no estado base |0...0>
- `apply_gate`: aplicação de operador unitário ao vetor de estado
- `measure`: medição probabilística sem colapso (regra de Born)
- `measure_shots`: múltiplas medições estatísticas com contagem de ocorrências

---

## Compilação

O projeto conta com automação completa via `Makefile`, com suporte a:
- Compilação da biblioteca estática: `lib/libqsim.a`
- Compilação da biblioteca dinâmica: `lib/libqsim.so`
- Compilação da aplicação com ligação estática: `bin/app_static`
- Compilação da aplicação com ligação dinâmica: `bin/app_shared`
- Compilação incremental: flags `-MMD -MP` mapeiam as dependências dos arquivos de cabeçalho (`.h`), recompilando estritamente os módulos que sofreram alteração.
- Configuração de `rpath`: o executável dinâmico utiliza `-Wl,-rpath,'$ORIGIN/../lib'` para localizar a biblioteca compartilhada de forma relativa ao binário, dispensando configuração manual de `LD_LIBRARY_PATH`.

Para compilar todo o projeto, execute:

```bash
make
```

ou

```bash
make all
```

---

## Alvos do Makefile

O `Makefile` disponibiliza os seguintes alvos:

- `make all` (ou apenas `make`):
  Compila os módulos da biblioteca, empacota a biblioteca estática (`libqsim.a`), gera a biblioteca compartilhada (`libqsim.so`) e compila as duas versões do executável da aplicação cliente (`bin/app_static` e `bin/app_shared`).

- `make clean`:
  Remove todos os artefatos intermediários de compilação das pastas `build/`, `lib/` e `bin/`.

- `make run`:
  Compila o projeto (caso necessário) e executa sequencialmente a aplicação cliente com ligação estática (`./bin/app_static`) e com ligação dinâmica (`./bin/app_shared`).

- `make test`:
  Compila e executa de forma automatizada todas as suítes de testes em `tests/`. Interrompe a execução caso qualquer teste falhe, exibindo o status de aprovação de cada módulo.

- `make install`:
  Instala os artefatos compilados da biblioteca (`libqsim.a` e `libqsim.so`) e os cabeçalhos públicos da API (`include/*.h`) no prefixo especificado. O prefixo padrão é `/usr/local`.

---

## Execução da Aplicação Cliente

A aplicação cliente demonstra a aplicação prática da biblioteca implementando o Algoritmo de Busca Quântica de Grover para encontrar números primos em um registrador de 1 a 4 qubits (intervalo de inteiros de 0 a 15).

A aplicação lê dois inteiros da entrada padrão:
1. `qubits`: quantidade de qubits (intervalo entre 1 e 4).
2. `shots`: número de amostragens/medições a serem realizadas.

### Executando a versão estática

```bash
./bin/app_static
```

### Executando a versão dinâmica

```bash
./bin/app_shared
```

### Exemplo de Uso

Ao executar `./bin/app_static` e fornecer a entrada `3 1000` (3 qubits, analisando números de 0 a 7, com 1000 medições):

Entrada:
```text
3 1000
```

Saída produzida:
```text
[1.000000 + 0.000000i, 0.000000 + 0.000000i, 0.000000 + 0.000000i, 0.000000 + 0.000000i, 0.000000 + 0.000000i, 0.000000 + 0.000000i, 0.000000 + 0.000000i, 0.000000 + 0.000000i]
[0.353553 + 0.000000i, 0.353553 + 0.000000i, 0.353553 + 0.000000i, 0.353553 + 0.000000i, 0.353553 + 0.000000i, 0.353553 + 0.000000i, 0.353553 + 0.000000i, 0.353553 + 0.000000i]
[0.353553 + 0.000000i, 0.353553 + 0.000000i, 0.353553 + 0.000000i, 0.353553 + 0.000000i, 0.353553 + 0.000000i, 0.353553 + 0.000000i, 0.353553 + 0.000000i, 0.353553 + 0.000000i]
Valor experimental: 51.2000%
Valor teórico: 50.0000%
```

A saída exibe:
- Vetor inicial no estado |000>
- Superposição uniforme após aplicação de Hadamard
- Vetor de amplitudes amplificado após o oráculo booleano e difusor de Grover
- Comparação de probabilidade teórica contra a medição experimental

---

## Testes Automatizados

O projeto contém testes automatizados dedicados para todas as funções públicas da biblioteca, cobrindo casos normais, casos de limite e entradas inválidas (ex: dimensões incompatíveis, alocações inválidas, argumentos nulos).

Para rodar todos os testes:

```bash
make test
```

Saída esperada:

```text
Iniciando os testes do módulo de circuitos quânticos
...
Todos os testes do módulo de circuitos quânticos passaram!
PASS build/tests/circuits_tests

Iniciando os testes do módulo de números complexos
...
Todos os testes do módulo de números complexos passaram
PASS build/tests/complex_tests

Iniciando os testes do módulo de álgebra linear
...
Todos os testes do módulo de álgebra linear passaram
PASS build/tests/linear_algebra_tests

Iniciando os testes do módulo de portas quânticas
...
Todos os testes do módulo de portas quânticas passaram
PASS build/tests/quantum_gates_tests
```

---

## Instalação

A biblioteca pode ser instalada no sistema ou em um diretório personalizado de usuário.

### Instalação no sistema (padrão `/usr/local`)

Requer privilégios administrativos:

```bash
sudo make install
```

### Instalação em diretório personalizado

Para instalar sem privilégios de administrador (por exemplo, em `$HOME/.local`):

```bash
make install PREFIX=$HOME/.local
```

### Arquivos instalados

O comando copia:
- Bibliotecas para `$(PREFIX)/lib/`:
  - `libqsim.a`
  - `libqsim.so`
- Arquivos de cabeçalho para `$(PREFIX)/include/qsim/`:
  - `circuits.h`
  - `complex.h`
  - `gates.h`
  - `linear_algebra.h`

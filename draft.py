from math import pi, e, cos, sin
from random import random

#Isso é só para reduzir a precisão do python e fazer com que valores 10^-16 já virem 0, em c n vai ter problema
def limpar_matriz(precisao=15):
    def decorator(func):
        def wrapper(*args, **kwargs):
            resultado = func(*args, **kwargs)
            return _arredondar_recursivo(resultado, precisao)
        return wrapper
    return decorator

def _arredondar_recursivo(val, prec):
    if isinstance(val, (int, float)):
        v = round(val, prec)
        return 0.0 if v == 0.0 else v
    elif isinstance(val, list):
        return [_arredondar_recursivo(x, prec) for x in val]
    return val

#==============================================
#módulo complexos
#==============================================
def complex_add(a, b):
    a[0] += b[0]
    a[1] += b[1]
    return a

def complex_mul(a, b):
    temp = [a[0], a[1]]
    temp[0] = a[0]*b[0]-a[1]*b[1]
    temp[1] = a[0]*b[1]+a[1]*b[0]
    return temp

def complex_abs_square(a):
    return a[0]**2 + a[1]**2


#==============================================
#módulo algelin
#==============================================
def create_vector(n):
    return [[0, 0] for _ in range(2**n)]

def tensor_product_vec(a, b):
    resultado = [0 for _ in range(len(a)*len(b))]
    for i in range(len(a)):
        for j in range(len(b)):
            resultado[i*len(b)+j] = complex_mul(a[i], b[j])
    return resultado

def tensor_product_matrix(a, b):
    m, n = len(a), len(a[0])
    p, q = len(b), len(b[0])

    resultado = [[0 for _ in range(n*q)] for _ in range(m*p)]

    for ia in range(m):
        for ja in range(n):
            for ib in range(p):
                for jb in range(q):
                    resultado[ia * p + ib][ja * q + jb] = complex_mul(a[ia][ja], b[ib][jb])
    return resultado

def matrix_mul(a, b):
    m, n = len(a), len(a[0])
    p, q = len(b), len(b[0])

    resultado = [[[0, 0] for _ in range(q)] for _ in range(m)]

    for ia in range(m):
        for jb in range(q):
            for ja in range(n):
                resultado[ia][jb] = complex_add(resultado[ia][jb], complex_mul(a[ia][ja], b[ja][jb]))
    return resultado

def matrix_vec_mul(matriz, vetor):
    m, n = len(matriz), len(matriz[0])

    resultado = [[0, 0] for _ in range(m)]
    for i in range(m):
        for j in range(n):
            resultado[i] = complex_add(resultado[i] , complex_mul(matriz[i][j], vetor[j]))

    return resultado

#==============================================
#módulo portas quânticas
#==============================================
def get_identity(n):
    identidade_pura=[[[1, 0], [0, 0]],
                     [[0, 0], [1, 0]]]
    identidade = identidade_pura
    for _ in range(n-1):
        identidade = tensor_product_matrix(identidade, identidade_pura)
    return identidade

def get_hadamard(n):
    hadamard_pura=[[[1/(2**0.5), 0], [1/(2**0.5), 0]],
                   [[1/(2**0.5), 0], [-1/(2**0.5), 0]]]
    hadamard = hadamard_pura
    for _ in range(n-1):
        hadamard = tensor_product_matrix(hadamard, hadamard_pura)
    return hadamard

@limpar_matriz(15)
def get_rotateX(n, theta):
    rx_pura = [[[cos(theta / 2), 0], [0, -sin(theta / 2)]],
          [[0, -sin(theta / 2)], [cos(theta / 2), 0]]]
    rx = rx_pura
    for _ in range(n-1):
        rx = tensor_product_matrix(rx, rx_pura)
    return rx

@limpar_matriz(15)
def get_rotateY(n, theta):
    ry_pura = [[[cos(theta / 2), 0], [-sin(theta / 2), 0]],
               [[sin(theta / 2), 0], [cos(theta / 2), 0]]]
    ry = ry_pura
    for _ in range(n-1):
        ry = tensor_product_matrix(ry, ry_pura)
    return ry

@limpar_matriz(15)
def get_rotateZ(n, theta):
    rz_pura = [[[cos(theta / 2), -sin(theta / 2)], [0, 0]],
               [[0, 0], [cos(theta / 2), sin(theta / 2)]]]
    rz = rz_pura
    for _ in range(n-1):
        rz = tensor_product_matrix(rz_pura, rz)
    return rz


#Função adaptada, créditos Instituto Eldorado e Cesar School
def get_cnot(n_qubits, control, target):
    tamanho = 2**n_qubits
    #convenção big-endian, necessário ajustar os indices
    control = n_qubits - 1 - control
    target  = n_qubits - 1 - target

    #Matriz tamanho x tamanho preenchida com [0, 0]
    U = [[[0, 0] for _ in range(tamanho)] for _ in range(tamanho)]

    #se o bit de controle for 1, inverte o bit alvo
    for col in range(tamanho):
        if (col >> control) & 1:
            row = col ^ (1 << target)
        else:
            row = col
        # |row><col| = 1 + 0i  ->  par [1, 0]
        U[row][col] = [1, 0]

    return U

#==============================================
#módulo circuitos quânticas
#==============================================
def init_qubits(n):
    qubit = [[1, 0], [0, 0]]
    for _ in range(n-1):
        qubit = tensor_product_vec(qubit, [[1, 0], [0, 0]])
    return qubit

def apply_gate(state, matriz):
    return matrix_vec_mul(matriz, state)

def measure(state):
    r = random()
    probabilidades = [complex_abs_square(abs) for abs in state]
    aux = 0
    for i in range(len(probabilidades)):
        aux += probabilidades[i]
        if probabilidades[i] > 0 and r < aux:
            return i
    return i

def measure_shots(state, shots):
    resultado = [0 for _ in range(len(state))]
    for _ in range(shots):
        r = random()
        probabilidades = [complex_abs_square(abs) for abs in state]
        aux = 0
        for i in range(len(probabilidades)):
            aux += probabilidades[i]
            if probabilidades[i] > 0 and r < aux:
                break
        resultado[i] += 1
    return resultado

#Teste, não é o cliente
qubits = init_qubits(2)
print(qubits)
qubits = apply_gate(qubits, tensor_product_matrix(get_hadamard(1), get_identity(1)))
print(qubits)
qubits = apply_gate(qubits, get_cnot(2, 0, 1))
print(qubits)
print(measure_shots(qubits, 1000))

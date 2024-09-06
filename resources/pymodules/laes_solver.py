r"""
author: Konak Vladimir Evgen'evich

last edited: 05.13.2024

organisation: Dnevnie mechaniki
"""

from math import log, ceil

def swap_lines(matrix, index1, index2, n):
    r"""
    Perform line swap to keep roots in ascending order during calculations.

    Utility method.
    :param matrix: matrix representation of the given LAES.
    :param index1: index of which is supposed to be swapped.
    :param index2: index of column where zero value is located.
    :param n: total number of lines in matrix
    """

    matrix = matrix
    for i in range(n):
        if i == index1:
            continue
        if matrix[i][index2] != 0:
            tempo = matrix[index1]
            matrix[index1] = matrix[i]
            matrix[i] = tempo


def split_matrix(matrix):
    r"""
    Split LAES matrix into two: 1st containing equations coefficients and 2nd containing constant values.

    Utility method.
    :param matrix: matrix representation of the given LAES.
    :return: tuple of 2  matrices: matrix A containing coefficients, matrix B containing constants.
    """

    A = []
    B = []
    for line in matrix:
        A.append(line[:-1])
        B.append(line[-1])
    return A, B


def validate(eq_system):
    r"""
    Checks that all equation contained within matrix are legit.

    Utility method.
    :param eq_system: matrix representation of the given LAES.
    :return: Bool value signalling if system is legit.
    """

    for line in eq_system:
        counter = 0
        for element in line:
            if element == 0:
                counter += 1
        if counter >= len(line) - 1:
            return False
    return True


def gauss(eq_system, precision=4):
    r"""
    Find `all` roots for MxN system of linear algebraic equations using Gaussian elimination method.

    :param eq_system: matrix representation of the LAES to be solved.
    :param precision: integer, represents number of signs after decimal point.
    :return: list of roots. Empty list is returned if no roots are found.
    If system has Infinitely many solutions only one of them is returned.
    """

    if not validate(eq_system):
        return list()
    n = len(eq_system[0]) - 1
    x = [0.0 for _ in range(n)]
    column = 0
    for k in range(0, n):
        if eq_system[k][k] == 0:
            swap_lines(eq_system, k, k, n)
        divider = eq_system[k][k]
        eq_system[k] = [element / divider for element in eq_system[k]]
        for j in range(k + 1, n + 1):
            eq_system[k][j] = eq_system[k][j] / eq_system[k][k]
            for i in range(k + 1, n):
                eq_system[i][j] = eq_system[i][j] - eq_system[i][k] * eq_system[k][j]
        for i in range(k + 1, n):
            eq_system[i][column] = 0
        column += 1
    if not validate(eq_system):
        if eq_system[0][-1] != 0.0:
            return list()
    for i in range(n - 1, -1, -1):
        s = 0
        for k in range(i + 1, n):
            s += eq_system[i][k] * x[k]
        x[i] = 0.0 if eq_system[i][n] - s == 0 else round(eq_system[i][n] - s, precision)
    return x


def simple_iterations(eq_system, precision=4):
    r"""
    Find `all` roots for MxN system of linear algebraic equations using Simple Iterations method.

    If LAES is solvable with Simple Iterations method then arbitrary precise answer is returned depending on
    :variable:`precision`.

    :param eq_system: matrix representation of the LAES to be solved.
    :param precision: integer, represents number of signs after decimal point.
    :return: tuple consisting of list of roots and `m` - number of iterations.
    if LAES has only one variable then `only` root is returned.
    If LAES can not be solved using this method then empty list is returned.
    """

    if not validate(eq_system):
        return list()
    n = len(eq_system)
    epsilon = 1 / (10 ** precision)
    x_n1 = [0 for _ in range(len(eq_system[0]) - 1)]

    for i in range(n):
        if eq_system[i][i] == 0:
            swap_lines(eq_system, i, i, n)
        div = eq_system[i][i]
        for j in range(len(eq_system[0])):
            eq_system[i][j] = eq_system[i][j] / div * -1
        eq_system[i][-1] = eq_system[i][-1] * -1
        eq_system[i][i] = 0

    q = 0
    alpha, beta = split_matrix(eq_system)
    for line in alpha:
        t_max = sum(abs(element) for element in line)
        q = t_max if t_max > q else q
    x_n = beta

    if q >= 1:
        return list()
    elif q == 0:
        return [round(element, precision) for element in x_n]

    index = 0
    for line in alpha:
        x_n1[index] = sum([line[i] * x_n[i] for i in range(n)]) + beta[index]
        index += 1
    x1_x0 = sum([abs(x_n1[i] - x_n[i]) for i in range(len(x_n))])
    m = log(epsilon * (1 - q) / x1_x0) / log(q)

    for i in range(ceil(m)):
        index = 0
        for line in alpha:
            x_n1[index] = sum([line[i] * x_n[i] for i in range(len(line))]) + beta[index]
            index += 1
        x_n = x_n1
    x_n = [round(element, precision) for element in x_n]
    return x_n, m

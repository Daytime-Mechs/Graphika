import sympy as sp
import numpy as np

def solve_nonlinear_system(equations_str):
    equations = []
    variables = []
    for eq in equations_str.split(";"):
        eq = eq.strip()
        parts = eq.split("=")
        lhs = sp.sympify(parts[0].strip())
        rhs = sp.sympify(parts[1].strip())
        equations.append(lhs - rhs)
        variables.extend(lhs.free_symbols)

    variables = list(set(variables))

    x0 = np.zeros(len(variables))
    solution = newton_nonlinear_systems(equations, variables, x0)

    return solution

def newton_nonlinear_systems(equations, variables, x0, tol=1e-6, max_iter=100):
    x = x0.copy()
    
    for _ in range(max_iter):
        F = [eq.subs([(v, x_i) for v, x_i in zip(variables, x)]) for eq in equations]
        if np.max(np.abs(F)) < tol:
            return x
        
        J = np.array([[sp.diff(eq, v).subs([(v_i, x_i) for v_i, x_i in zip(variables, x)]) for v in variables] for eq in equations])
        x = x - np.linalg.solve(J, F)
    
    raise RuntimeError("Метод Ньютона не сходится")

#equations_str = 'x^2 + y = 0; x - y = 0'
#solution = solve_nonlinear_system(equations_str)
#print(solution)

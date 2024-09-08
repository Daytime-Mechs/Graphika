#include "sender.h"
#include "settings.h"

Sender::Sender() {}

void Sender::setMacro( pymodules::Methods method, pymodules::Modules module ) {

    switch (method) {
    case pymodules::Methods::NEWTON:
        functionName = "newton_polynomial";
        break;
    case pymodules::Methods::LAGRANGE:
        functionName = "lagrange_polynomial";
        break;
    case pymodules::Methods::BEIRUT:
        functionName = "beirut_functions";
        break;
    case pymodules::Methods::NIL:
        functionName = nullptr;
        break;
    case pymodules::Methods::DIFF_2P:
        functionName = "differential_2p_for_plot";
        break;
    case pymodules::Methods::DIFF_3P:
        functionName = "differential_3p_for_plot";
        break;
    case pymodules::Methods::DIFF_5P:
        functionName = "differential_5p_for_plot";
        break;
    case pymodules::Methods::INTEG_LINEAR:
        functionName = "integral_linear";
        break;
    case pymodules::Methods::INTEG_TRAP:
        functionName = "integral_trapezoid";
        break;
    case pymodules::Methods::INTEG_PARAB:
        functionName = "integral_parabolic";
        break;
    case pymodules::Methods::GAUSS:
        functionName = "gauss";
        break;
    case pymodules::Methods::SIMPLE_ITER:
        functionName = "simple_iterations";
        break;
    case pymodules::Methods::NON_LINEAR_SYS:
        functionName = "solve_nonlinear_system";
        break;
    default:
        break;
    }

    switch (module) {
    case pymodules::Modules::POLYNOMIALS:
        moduleName = ":/pyFiles/resources/pymodules/polynomials.py";
        break;
    case pymodules::Modules::NIL:
        moduleName = nullptr;
        break;
    case pymodules::Modules::DIFFERENTIATION:
        moduleName = ":/pyFiles/resources/pymodules/differentiation.py";
        break;
    case pymodules::Modules::INTEGRATION:
        moduleName = ":/pyFiles/resources/pymodules/integration.py";
        break;
    case pymodules::Modules::EQUATIONS:
        moduleName = ":/pyFiles/resources/pymodules/laes_solver.py";
        break;
    case pymodules::Modules::NON_LINEAR_EQUATIONS:
        moduleName = ":/pyFiles/resources/pymodules/nonlinear_laes_solver.py";
        break;
    default:
        break;
    }
}

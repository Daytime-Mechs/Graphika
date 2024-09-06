/*!
 * \attention In development.
 *
 * \author Malaniya Mark Timurovich, Dnevnie Mechaniki.
 *
 * \date last update: 18.05.2024.
 */

#ifndef SETTINGS_H
#define SETTINGS_H

namespace limits
{
const int NODES_LIMIT = 10; ///< limit on the number of interpolation nodes.
const int SIZE_LIMIT = 1002; ///< limit of nodes for plotting.
} // limits

namespace dimensional
{
enum PlotDimensional
{
    _2D, _3D
};
} // dimensional

namespace pymodules
{
/*!
 * \class Methods
 *
 * \brief The Methods enum
 */
enum class Methods
{
    NEWTON, ///< Newton method of interpolation.
    LAGRANGE, ///< Lagrange method of interpolation.
    BEIRUT, ///< Beirut method of interpolation.
    DIFF_2P, ///< two-point differentiation.
    DIFF_3P, ///< three-point differentiation.
    DIFF_5P, ///< five-point differentiation.
    INTEG_LINEAR, ///< Linear method of integration.
    INTEG_TRAP, ///< Trapezoid method of integration.
    INTEG_PARAB, ///< Parabolic method of integration.
    GAUSS, ///< solving a system of equations using the Gaussian method.
    SIMPLE_ITER, ///< solving a system of equations using the simple iterations method.
    NON_LINEAR_SYS,
    NIL ///< no methods given.
};

/*!
 * \class Modules
 *
 * \brief The Python Modules enum
 */
enum class Modules
{
    POLYNOMIALS, ///< polynomials.py file to invoke.
    DIFFERENTIATION, ///< differentiation.py file to invoke.
    INTEGRATION, ///< integration.py file to invoke.
    EQUATIONS, ///< equations.py file to invoke.
    NON_LINEAR_EQUATIONS,
    NIL ///< no methods given.
};

} // pymodules

#endif // SETTINGS_H

/*!
 * \attention In development.
 *
 * \author Malaniya Mark Timurovich, Dnevnie Mechaniki.
 *
 * \date last update: 21.05.2024.
 */

#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <QWidget>
#include <QTableWidget>
#include <vector>
#include <cmath>

/*!
 * \class MathUtils
 *
 * \brief The MathUtils class that contains some handy utils for work.
 */
class MathUtils
{
public:
    /*!
     * \brief MathUtils: standard constructor.
     */
    MathUtils( void );

    /*!
     * \brief multiplyPoints: multiply points from a to b with some step.
     *
     * \param a: x-min.
     * \param b: x-max.
     *
     * \return vector of real x numbers.
     */
    static std::vector<double> multiplyPoints( const double a, const double b ) noexcept;

    /*!
     * \brief setupNodes: calculates the step and creates points for the x array.
     *
     * \param X: x data array for data save.
     * \param nodes: number of nodes in the interim.
     * \param min: x-min.
     * \param max: x-max.
     */
    static void setupNodes( std::vector<double>& data, const double nodes, const double min, const double max ) noexcept;

    /*!
     * \brief calculateAverageError: calculates error between two arrays of real numbers data.
     *
     * \param X1: first array.
     * \param X2: second array.
     *
     * \return average error.
     */
    static double calculateAverageError( const QVector<double>& X1, const QVector<double>& X2 ) noexcept;

    /*!
     * \brief formTheSystemOfEquations: forms the data structure from given data table.
     *
     * \param table: table of unknown and free terms of the equation system.
     *
     * \return storage format more convenient for programming.
     */
    static QVector<QVector<double>> formTheSystemOfEquations( const QTableWidget& table );
};

#endif // MATHUTILS_H

/*!
 * \attention In development.
 *
 * \author Korshunov Ilya Dmitrievich, Dnevnie Mechaniki.
 *
 * \date last update: 15.09.2024.
 */

#ifndef PROGRAMMERSETTINGS_H
#define PROGRAMMERSETTINGS_H
#include <QDebug>

/*!
 * \struct ProgrammerSettings
 * \brief The ProgrammerSettings struct holds configuration settings for a programmable environment, including ranges, steps, and nodes.
 *
 * This struct is used to define various parameters such as minimum and maximum ranges for both axes, step sizes, and the number of nodes.
 */
struct ProgrammerSettings {
    double min; ///< Minimum X-axis range.
    double yMin; ///< Minimum Y-axis range.
    double max; ///< Maximum X-axis range.
    double yMax; ///< Maximum Y-axis range.
    double minStep; ///< Minimum step size.
    double maxStep;  ///< Maximum step size.
    double minNodes; ///< Minimum number of nodes.
    double maxNodes; ///< Maximum number of nodes.
    int decimals; ///< Number of decimal places.

    /*!
     * \brief ProgrammerSettings: Default constructor that initializes all fields to their default values.
     *
     * Initializes:
     * - min = -100.0
     * - yMin = -100.0
     * - max = 100.0
     * - yMax = 100.0
     * - minStep = 0.01
     * - maxStep = 1.0
     * - minNodes = 2.0
     * - maxNodes = 100.0
     * - decimals = 2
     */
    ProgrammerSettings()
        : min(-100.0), yMin(-100.0), max(100.0), yMax(100.0),
        minStep(0.01), maxStep(1.0), minNodes(2.0), maxNodes(100.0),
        decimals(2) {}
};

#endif // PROGRAMMERSETTINGS_H

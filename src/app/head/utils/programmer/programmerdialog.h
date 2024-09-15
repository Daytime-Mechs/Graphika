/*!
 * \attention In development.
 *
 * \author Korshunov Ilya Dmitrievich, Dnevnie Mechaniki.
 *
 * \date last update: 15.09.2024.
 */

#ifndef PROGRAMMERDIALOG_H
#define PROGRAMMERDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>
#include "programmerSettings.h"

/*!
 * \class ProgrammerDialog
 * \brief The ProgrammerDialog class provides a dialog for configuring settings like range, steps, and nodes for a programming environment.
 *
 * The dialog allows the user to input various numerical ranges and parameters, and then apply or reset these settings.
 */

class ProgrammerDialog : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief ProgrammerDialog: Constructor to initialize the programmer settings dialog.
     *
     * \param parent: Pointer to the parent widget (optional).
     */
    explicit ProgrammerDialog(QWidget *parent = nullptr);

    /*!
     * \brief getMinRange: Returns the minimum range value.
     * \return The minimum range value as a double.
     */
    double getMinRange();
    /*!
     * \brief getYMinRange: Returns the minimum Y-axis range value.
     * \return The minimum Y-axis range value as a double.
     */
    double getYMinRange();
    /*!
     * \brief getMaxRange: Returns the maximum range value.
     * \return The maximum range value as a double.
     */
    double getMaxRange();
    /*!
     * \brief getYMaxRange: Returns the maximum Y-axis range value.
     * \return The maximum Y-axis range value as a double.
     */
    double getYMaxRange();

    /*!
     * \brief getMinStepRange: Returns the minimum step range value.
     * \return The minimum step range value as a double.
     */
    double getMinStepRange();
    /*!
     * \brief getMaxStepRange: Returns the maximum step range value.
     * \return The maximum step range value as a double.
     */
    double getMaxStepRange();

    /*!
     * \brief getMinNodesRange: Returns the minimum nodes range value.
     * \return The minimum nodes range value as a double.
     */
    double getMinNodesRange();
    /*!
     * \brief getMaxNodesRange: Returns the maximum nodes range value.
     * \return The maximum nodes range value as a double.
     */
    double getMaxNodesRange();

    /*!
     * \brief getDecimals: Returns the number of decimal places for a given value.
     *
     * \param value: The value for which the decimal precision is calculated.
     * \return The number of decimal places as an integer.
     */
    int getDecimals(double value);

signals:
    /*!
     * \brief settingsApplied: Signal emitted when settings are applied.
     *
     * \param min: Minimum X-axis range.
     * \param Ymin: Minimum Y-axis range.
     * \param max: Maximum X-axis range.
     * \param Ymax: Maximum Y-axis range.
     * \param minStep: Minimum step value.
     * \param maxStep: Maximum step value.
     * \param minNodes: Minimum number of nodes.
     * \param maxNodes: Maximum number of nodes.
     * \param decimels: Number of decimal places.
     */
    void settingsApplied(double min, double Ymin, double max, double Ymax, double minStep, double maxStep, double minNodes, double maxNodes, int decimels);

private slots:
    /*!
     * \brief applySettings: Slot to handle the "Apply" button click event.
     *
     * This method applies the current settings and emits the settingsApplied signal.
     */
    void applySettings();
    /*!
     * \brief resetSettings: Slot to handle the "Reset" button click event.
     *
     * This method resets all the values in the dialog to their default states.
     */
    void resetSettings();

private:
    QDoubleSpinBox* minSpinBox; ///< Spinbox for setting the minimum X-axis range.
    QDoubleSpinBox* yMinSpinBox;  ///< Spinbox for setting the minimum Y-axis range.
    QDoubleSpinBox* maxSpinBox; ///< Spinbox for setting the maximum X-axis range.
    QDoubleSpinBox* yMaxSpinBox; ///< Spinbox for setting the maximum Y-axis range.
    QDoubleSpinBox* minStepSpinBox; ///< Spinbox for setting the minimum step value.
    QDoubleSpinBox* maxStepSpinBox;///< Spinbox for setting the maximum step value.
    QDoubleSpinBox* minNodesSpinBox; ///< Spinbox for setting the minimum number of nodes.
    QDoubleSpinBox* maxNodesSpinBox;  ///< Spinbox for setting the maximum number of nodes.
    QPushButton *applyButton; ///< Button to apply the settings.
    QPushButton *resetButton; ///< Button to reset the settings.
    QFormLayout *formLayout; ///< Button to reset the settings.
    QVBoxLayout *mainLayout; ///< Main layout for organizing the form and buttons.
};

#endif // PROGRAMMERDIALOG_H

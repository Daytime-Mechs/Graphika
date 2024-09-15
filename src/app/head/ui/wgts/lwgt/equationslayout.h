/*!
 * \attention In development.
 *
 * \author Malaniya Mark Timurovich, Dnevnie Mechaniki.
 *
 * \date last update: 15.09.2024.
 */

#ifndef EQUATIONSLAYOUT_H
#define EQUATIONSLAYOUT_H


#include "layoutInitializer.h"
#include <QRadioButton>

/*!
 * \class EquationsLayout
 *
 * \brief The EquationsLayout class, that heir a super class "LayoutInitializer" which contains widget controls.
 */
class EquationsLayout : public LayoutInitializer
{
    Q_OBJECT
private:
    QGridLayout* layout; ///< a grid layout for widgets.
    bool nonLinearFlag = false; ///< Flag indicating if non-linear equations are active.

public:
    /*!
     * standard class-destructor.
     */
    ~EquationsLayout( void ) { delete layout; }

    /*!
     * \brief generateWidgets: method adopting class, additional list of widgets.
     *
     * \param widgets: special class for storing all widgets on the left side of the window control.
     */
    void generateWidgets( Widgets& widgets ) override;

    /*!
     * \brief get: standard layout getter.
     *
     * \return private class member layout.
     */
    QGridLayout* get( void ) override;

public slots:
    /*!
     * \brief updateEquationsButtonsPosition: Adjusts the position of equation control buttons.
     */
    void updateEquationsButtonsPosition( void );
    /*!
     * \brief onAddEquationRowButtonClicked: Adds a new row for equation input.
     */
    void onAddEquationRowButtonClicked( void );
    /*!
     * \brief onRemoveEquationRowButtonClicked: Removes the selected row from the equation input table.
     */
    void onRemoveEquationRowButtonClicked( void );
    /*!
     * \brief hideEquationsButtonsWidget: Hides the buttons used for controlling the equations table.
     */
    void hideEquationsButtonsWidget( void );
    /*!
     * \brief setNonLinearFlag: Sets the flag for non-linear equation mode.
     *
     * \param flag: Boolean value to indicate if non-linear mode is enabled.
     */
    void setNonLinearFlag( bool flag );
    /*!
     * \brief updateNonLinearSpinBoxes: Updates the spin boxes related to non-linear equations.
     */
    void updateNonLinearSpinBoxes( void );

signals:
    /*!
     * \brief equationsTableEdited: Signal emitted when the equations table is edited.
     */

    void equationsTableEdited( void );

};

#endif

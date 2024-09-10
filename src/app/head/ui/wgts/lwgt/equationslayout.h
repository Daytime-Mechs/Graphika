/*!
 * \attention In development.
 *
 * \author Malaniya Mark Timurovich, Dnevnie Mechaniki.
 *
 * \date last update: 18.05.2024.
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
    void updateEquationsButtonsPosition();
    void onAddEquationRowButtonClicked();
    void onRemoveEquationRowButtonClicked();
    void hideEquationsButtonsWidget();

signals:
    void equationsTableEdited();

};

#endif

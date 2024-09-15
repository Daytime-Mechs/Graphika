/*!
 * \attention In development.
 *
 * \author Malaniya Mark Timurovich, Dnevnie Mechaniki.
 *
 * \date last update: 18.05.2024.
 */

#ifndef MENUBAR_H
#define MENUBAR_H

#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QMenu>
#include <QMouseEvent>
#include "sender.h"
#include "qapplication.h"

/*!
 * \class MenuBar
 *
 * \brief The MenuBar class that contains every menu module.
 */
class MenuBar : public QToolBar
{
    Q_OBJECT
public:
    /*!
     * \brief MenuBar: constructor with standard heir argument.
     *
     * \param parent: pointer to the parent object.
     */
    explicit MenuBar( QWidget *parent = nullptr );

    dimensional::PlotDimensional getSelectedFuncMethod( void ) const;

    /*!
     * \brief getSelectedDiffMethod: standard getter.
     * \return pymodules::Methods
     */
    pymodules::Methods getSelectedDiffMethod( void ) const;

    /*!
     * \brief getSelectedDiffMethod: standard getter.
     * \return pymodules::Methods
     */
    pymodules::Methods getSelectedIntegralMethod( void ) const;

    /*!
     * \brief getSelectedDiffMethod: standard getter.
     * \return pymodules::Methods
     */
    pymodules::Methods getSelectedSysMethod( void ) const;

    /*!
     * \brief unsetChecked: makes every checkable action unchecked.
     */
    void unsetChecked( void );

protected:
    /*!
     * \brief mousePressEvent: calls a method when the right mouse button is clicked.
     * \param event: right button click.
     */
    void mousePressEvent( QMouseEvent *event ) override;

private:
    QAction* plotFunctionAction; ///< QAction for plotting functions.

    QAction* lagrangeAction; ///< QAction for Lagrange polynomial method.
    QAction* newthonAction; ///< QAction for Newton polynomial method.
    QAction* beirutAction; ///< QAction for Beirut polynomial method.

    QAction* diffAction; ///< derivation action menu.
    QAction* methodTwoDots; ///< specific derivation method.
    QAction* methodThreeDots; ///< specific derivation method.
    QAction* methodFiveDots; ///< specific derivation method.

    QAction* integralAction; ///< integration action menu.
    QAction* linearMethod; ///< specific integration method.
    QAction* trapezoidMethod; ///< specific integration method.
    QAction* parabolicMethod; ///< specific integration method.

    QAction* sysAction; ///< equations action menu.
    QAction* gaussMethod; ///< specific equations method.
    QAction* nonlinearNewthonMethod; ///< specific equations method.
    QAction* simpleIterMethod; ///< specific equations method.

    QMenu* diffMenu; ///< derivation menu.
    QMenu* integralMenu; ///< integration menu.
    QMenu* sysMenu; ///< equations menu.

private:
    void initFuncMenu( void );

    /*!
     * \brief initDiffMenu: setup menu of derivation actions.
     */
    void initDiffMenu( void );

    /*!
     * \brief initIntegralMenu: setup menu of integration actions.
     */
    void initIntegralMenu( void );

    /*!
     * \brief initSysMenu: setup menu of equation actions.
     */
    void initSysMenu( void );

    /*!
     * \brief initMenu: invokes specific menu method.
     *
     * \param menu: type of menu.
     * \param items: list of options.
     */
    void initMenu( QMenu*& menu, const QStringList& items );

    void updateFuncCheckState( QAction* checkedAction );

    void initPolynomeMenu( void );

    /*!
     * \brief updateDiffCheckState: updates derivation checkbox object.
     *
     * \param checkedAction: specific action.
     */
    void updateDiffCheckState( QAction* checkedAction );

    /*!
     * \brief updateIntegralCheckState: updates integration checkbox object.
     *
     * \param checkedAction: specific action.
     */
    void updateIntegralCheckState( QAction* checkedAction );

    /*!
     * \brief updateSysCheckState: updates equations checkbox object.
     *
     * \param checkedAction: specific action.
     */
    void updateSysCheckState( QAction* checkedAction );

    /*!
     * \brief updateDiffCheckState: updates checkbox object.
     *
     * \param checkedAction: specific action.
     * \param actions: list of actions.
     */
    void updateCheckState( QAction* checkedAction, QList<QAction*> actions );

    /*!
     * \brief setCheckable: set all actions checkable.
     */
    void setCheckable( void );
    /*!
     * \brief isPersistentAction: checks if the action is persistent (i.e., stays active).
     * \param action: the action to check.
     * \return true if the action is persistent, otherwise false.
     */
    bool isPersistentAction(QAction* action);
    /*!
     * \brief connectActions: connects signals and slots for actions.
     */
    void connectActions();

signals:
    /*!
     * \brief currentMethodChanged: signal emitted when the method of solving or calculating changes.
     *
     * \param methodName: name of the selected method.
     */
    void currentMethodChanged( const QString& methodName );
    /*!
     * \brief containsNonLinearData: signal emitted if non-linear data is detected.
     *
     * \param nonLinear: true if non-linear data is present, otherwise false.
     */
    void containsNonLinearData( const bool& nonLinear );

private slots:
    /*!
     * \brief handleDiffActionTriggered: handles the event when a differentiation action is triggered.
     */
    void handleDiffActionTriggered( void );
    /*!
     * \brief handleIntegralActionTriggered: handles the event when an integration action is triggered.
     */
    void handleIntegralActionTriggered( void );
};

#endif // MENUBAR_H

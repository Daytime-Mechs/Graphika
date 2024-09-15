/*!
 * \attention In development.
 *
 * \author Malaniya Mark Timurovich, Dnevnie Mechaniki.
 *
 * \date last update: 21.05.2024.
 */

#ifndef LEFTWIDGET_H
#define LEFTWIDGET_H

#include <QWidget>
#include "widgets.h"
#include "functionlayout.h"
#include "derivationlayout.h"
#include "integrationlayout.h"
#include "equationslayout.h"
#include "polynomialslayout.h"
#include "rightwidget.h"
#include "validatestring.h"
#include "stringparser.h"
#include "datamodel.h"
#include "buffer.h"
#include "mathutils.h"
#include "settings.h"
#include <QDoubleSpinBox>
#include <QDebug>

#include "programmerdialog.h"
#include "programmerSettings.h"

/*!
 * \class LeftWidget
 *
 * \brief The LeftWidget class, the main link for managing the state of widgets on the left side of the window.
 */
class LeftWidget : public QWidget
{
    Q_OBJECT
private:
    FunctionLayout* functionLayout; ///< contains all widgets for function module need.
    DerivationLayout* derivationLayout; ///< contains all widgets for derivation module need.
    IntegrationLayout* integrationLayout; ///< contains all widgets for integration module need.
    EquationsLayout* equationsLayout; ///< contains all widgets for equations module need.
    PolynomialsLayout* polynomialsLayout; ///< contains all widgets for polynomials module need.

    Widgets* widgets; ///< a class that stores all widgets for editing members of the LeftWidget class, descendants of LayoutInitializer.
    QGridLayout* layout; ///< main left-widget layout.
    ProgrammerSettings programmerSetting;

public:
    LayoutInitializer* currentLayout; ///< layout that contains up-to-date information about the state of widgets.

private:
    /*!
     * \brief hideAllWidgets: goes through the list of widgets and makes them invisible.
     *
     * \param layout: object with widgets container.
     */
    void hideAllWidgets( QLayout* layout );

    /*!
     * \brief connectLabels: method to connect all slots and signals of members of this class.
     *
     * \param buffer: x & y data buffer to transfer.
     */
    void connectLabels( SpecialBuffer& buffer );
    /*!
     * \brief applyStoredSettings: applies the previously saved programmer settings to the widget.
     */
    void applyStoredSettings();
    /*!
     * \brief hideButtons: hides all buttons in the widget.
     */
    void hideButtons();

public:
    /*!
     * \brief LeftWidget:base constructor.
     *
     * \param buffer: x & y data buffer to transfer.
     * \param parent: pointer to the parent object.
     */
    explicit LeftWidget( QWidget* parent = nullptr );

public slots:
    /*!
     * \brief initLayout: method for initializing and saving the state of a layout to a class named layout.
     *
     * \param buffer: x & y data buffer to transfer.
     * \param module: specific module for layout generation.
     */
    void initLayout( SpecialBuffer& buffer, pymodules::Modules module );

    /*!
     * \brief applyProgrammerSettings: save new settings statement
     *
     * \param min
     * \param Ymin
     * \param max
     * \param Ymax
     * \param minStep
     * \param maxStep
     * \param minNodes
     * \param maxNodes
     * \param decimals
     */
    void applyProgrammerSettings( double min, double Ymin, double max, double Ymax, double minStep, double maxStep, double minNodes, double maxNodes, int decimals );
    /*!
     * \brief onTableEdited: triggered when table data is edited.
     */
    void onTableEdited();
    /*!
     * \brief onEquationsTableEdited: triggered when the equations table data is edited.
     */
    void onEquationsTableEdited();
    /*!
     * \brief setNonLinearFlag: sets the flag indicating whether the system of equations is nonlinear.
     *
     * \param flag: boolean value indicating whether the system is nonlinear.
     */
    void setNonLinearFlag( bool flag );
    /*!
     * \brief updateNonLinearSpinBoxes: updates spin boxes for nonlinear system settings.
     */
    void updateNonLinearSpinBoxes();
    /*!
     * \brief acceptXYData: accepts and stores X and Y data vectors.
     *
     * \param x: vector of X data.
     * \param y: vector of Y data.
     */
    void acceptXYData( const QVector<double>& x, const QVector<double>& y );

signals:
    /*!
     * \brief switchToGL3DGraphBuilder: signal to switch to 3D graph builder mode.
     */
    void switchToGL3DGraphBuilder( void );
    /*!
     * \brief switchToGraphBuilder: signal to switch to 2D graph builder mode.
     */
    void switchToGraphBuilder( void );
    /*!
     * \brief functionTextChanged: signal emitted when the function text is changed.
     *
     * \param functionText: the new function text.
     */
    void functionTextChanged( const QString& functionText );
    /*!
     * \brief sendNonLinearSys: signal to send a nonlinear system text.
     *
     * \param sysText: the nonlinear system text.
     */
    void sendNonLinearSys( const QString& sysText );
    /*!
     * \brief buildFuncGraph: signal to build a function graph.
     *
     * \param x: vector of X data.
     * \param y: vector of Y data.
     */
    void buildFuncGraph( const QVector<double> &x, const QVector<double> &y );
};

#endif // LEFTWIDGET_H

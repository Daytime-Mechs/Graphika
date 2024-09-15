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
    void applyStoredSettings();
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

    void onTableEdited();
    void onEquationsTableEdited();
    void setNonLinearFlag( bool flag );
    void updateNonLinearSpinBoxes();
    void acceptXYData( const QVector<double>& x, const QVector<double>& y );

signals:
    void switchToGL3DGraphBuilder( void );
    void switchToGraphBuilder( void );
    void functionTextChanged( const QString& functionText );
    void sendNonLinearSys( const QString& sysText );
    void buildFuncGraph( const QVector<double> &x, const QVector<double> &y );
};

#endif // LEFTWIDGET_H

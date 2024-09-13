/*!
 * \attention In development.
 *
 * \author Malaniya Mark Timurovich, Dnevnie Mechaniki.
 *
 * \date last update: 20.05.2024.
 */


#ifndef LAYOUTINITIALIZER_H
#define LAYOUTINITIALIZER_H

#include <QGridLayout>
#include <QTextEdit>
#include <QTextDocument>
#include <buffer.h>
#include <QString>
#include "widgets.h"
#include "mathutils.h"

struct Equation {
    std::vector<std::string> variables;
    double constant;
    double rhs;
};

class MathWidget;

/*!
 * \class LayoutInitializer
 *
 * \brief The LayoutInitializer class. Contains methods for managing widgets and their initialization.
 */
class LayoutInitializer : public QWidget
{
    Q_OBJECT
protected:
    bool couldBuildTable{ true }; ///< depends of expression correctness.
    bool manualInput{ false }; ///< is data was entered by manual input.
    bool derivativeLabel{ false }; ///< is derivative label active.

public:
    Widgets* widgets; ///< list of all widgets.

public:
    /*!
     * \brief ~LayoutInitializer: standard virtual destructor.
     */
    virtual ~LayoutInitializer( void ) {}

    /*!
     * \brief generateWidgets: initialize layout with given widgets object.
     *
     * \param widgets: list of all needed objects.
     */
    virtual void generateWidgets( Widgets& widgets ) = 0;

    /*!
     * \brief get: standard getter.
     *
     * \return layout.
     */
    virtual QGridLayout* get( void ) = 0;

    /*!
     * \brief hideWidgets: makes every given widget invisible.
     * \param w: list of widgets.
     */
    template<typename... widgets>
    void hideWidgets( widgets*... w )
    {
        ( ... , ( std::is_base_of_v<QWidget, std::decay_t<Widgets>> ? static_cast<void>( w->hide() ) : void() ) );
    }

    /*!
     * \brief onInputTextChanged: invokes validator for updated string expression.
     *
     * \param text: math expressiom.
     */
    void onInputTextChanged( const QString& text );

    /*!
     * \brief onValidateDataValid: makes it possible to solve the equation.
     */
    void onValidateDataValid( void );

    /*!
     * \brief onValidateDataInvalid: prohibits solving the equation.
     */
    void onValidateDataInvalid( void );

    /*!
     * \brief editTable: rebuilds data table to default statement.
     */
    void editTable( void );

    /*!
     * \brief clearDataTable: clears the table.
     */
    void clearDataTable( void );

    /*!
     * \brief changeLayer: changes the fields for entering limits on x depending on the type of division into nodes.
     *
     * \param index: number of layer.
     */
    void changeLayer( int index );

    /*!
     * \brief hideFirstLayer: limits with step.
     */
    void hideFirstLayer( void );

    /*!
     * \brief hideSecondLayer: limits with nodes.
     */
    void hideSecondLayer( void );

    /*!
     * \brief onSolveButtonClicked: collects data, parses values ​​for y, builds a table.
     *
     * \param buffer: setting x & y data from table for right widget transfer.
     */
    void onSolveButtonClicked( SpecialBuffer& buffer );

    /*!
     * \brief updateDataFromTable: updates data to the current state and stores it in a buffer.
     *
     * \param buffer: setting x & y data from table for right widget transfer.
     */
    void updateDataFromTable( SpecialBuffer& buffer );

    /*!
     * \brief setupNodes: invokes math utils method for nodes propagation.
     *
     * \param node: number of nodes.
     */
    void setupNodes( const double node );

    /*!
     * \brief fillDataFromTable: collects all data from table columnt to vector.
     *
     * \param column: number of column.
     *
     * \return vector of column data.
     */
    std::vector<double> fillDataFromTable( int column );

    /*!
     * \brief showTable: generates and displays a table.
     *
     * \param x: x variables data.
     * \param y: y variables data.
     * \param dY: derivated y variables data.
     */
    void showTable( const std::vector<double> x, const std::vector<double> y, const std::vector<double> z, const std::vector<double> dY );

public slots:
    /*!
     * \brief handleParserError: prevents from building a table due to an error.
     *
     * \param err: a given error message.
     */
    void handleParserError( const QString &err );

    /*!
     * \brief acceptData: parses a given mathematical function within specified limits with a specially configured step.
     *
     * \param mode: math expression.
     * \param a: x-min.
     * \param b: x-max.
     */
    void acceptData( const QString& model, const double a, const double b );

    /*!
     * \brief onSolveEquationsButtonClicked: prepares data from a table for solving a system of equations.
     */
    void onSolveEquationsButtonClicked( void );

    /*!
     * \brief setEquationsResult: setting a result list of founded variables.
     *
     * \param result: a given list of founded variables result.
     */
    void setEquationsResult( const QString& result );

    /*!
     * \brief acceptArea: setting a result of founded area of curve.
     *
     * \param area: given real number of area.
     */
    void acceptArea( const std::string& area );

    void onAddRowButtonClicked();
    void onRemoveRowButtonClicked();
    void updateButtonsPosition();

    void clearTableButtons();
    void hideButtonsWidget();

    void containsNonLinearData( const bool& nl );

    void calculateDiffError( const QVector<double>& y1, const QVector<double>& y2 );

signals:
    /*!
     * \brief validExpression: is given math expression was typed currectly.
     */
    void validExpression( void );
    void tableEdited();

    /*!
     * \brief invalidExpression: is given math expression was typed  incorrect.
     */
    void invalidExpression( void );

    /*!
     * \brief readyToDraw: emits when the program ready to display given data to plot.
     *
     * \param x: given x data variables.
     * \param y: given y data variables.
     */
    void readyToDraw( std::vector<double>& x, std::vector<double>& y );

    /*!
     * \brief readyToSendEquationsData: emits when called when the program is ready to transfer conveniently formed data from the table.
     *
     * \param data: given formed data from table to vevctor of real number vectors.
     */
    void readyToSendEquationsData( QVector<QVector<double>>& data );

    void readyToSendNonLinearSys( const QString& sys );
};

#endif

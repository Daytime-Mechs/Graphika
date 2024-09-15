/*!
 * \attention In development.
 *
 * \author Malaniya Mark Timurovich, Dnevnie Mechaniki.
 *
 * \date last update: 15.09.2024.
 */

#ifndef RIGHTWIDGET_H
#define RIGHTWIDGET_H

#include <QWidget>
#include <QVector>
#include <graphbar.h>
#include "graphbuilder.h"
#include "buffer.h"
#include "pythonconveyor.h"
#include "sender.h"
#include "loglist.h"
#include "settings.h"
#include "mathutils.h"

/*!
 * \class RightWidget
 *
 * \brief The RightWidget class, the main link for managing the state of widgets on the right side of the window.
 */
class RightWidget : public QWidget
{
    Q_OBJECT
private:
    std::string resultModel; ///< model of interpolation.
    std::string area; ///< area of ​​the trapezoid under the curve.
    QString currentLegend;  ///< currently displayed legend text on the graph.
    QString functionText; ///< mathematical function in string form.
    QString eqSysText; ///< system of equations in string form.
    GraphBar* graphBar; ///< instance of GraphBar for handling graph settings and options.

    int graphBarWidth, graph2dHeight, graph3dHeight;

    bool unpinned{ false }; ///< flag indicating if the graph is unpinned.

public:
    QGridLayout* rightLayout; ///< main right-widget layout.

    PythonConveyor* conveyor; ///< data linker between C++ code and python modules.
    GraphBuilder* graphBuilder; ///< main plot builder class.

    QVector<double> x; ///< x variables.
    QVector<double> y; ///< y variables.
    QVector<double> z; ///< z variables.

private:
    /*!
     * \brief buildWidgetForDerivativeOperations: change y axis to y'.
     */
    void buildWidgetForDerivativeOperations( void );

public:
    /*!
     * \brief RightWidget: constructor with standard heir argument.
     *
     * \param paren: pointer to the parent object.
     */
    explicit RightWidget( QWidget *parent = nullptr );

    std::string getResultModel( void ) { return resultModel; }
    /*!
     * \brief getArea: returns the area under the curve.
     * \return calculated area as a string.
     */
    std::string getArea( void ) { return area; }

    /*!
     * \brief printGraph: method for plot generation.
     *
     * \param buffer: x & y data variables.
     * \param sender: module to invoke & legend name.
     * \param stack: stack for data log system (in development).
     */
    void printGraph( SpecialBuffer& buffer, Sender& sender, LogList* logList );

    /*!
     * \brief printGraph: method for derivation-plot generation.
     *
     * \param buffer: x & y data variables.
     * \param sender: module to invoke & legend name.
     * \param stack: stack for data log system (in development).
     */
    void printDiffGraph( SpecialBuffer& buffer, Sender& sender, LogList* logList );

    /*!
     * \brief calculateIntegral: method for area calculating algorithm.
     *
     * \param buffer: x & y data variables.
     * \param sender: integration module with specific method.
     * \param stack: stack for data log system (in development).
     */
    void calculateIntegral( SpecialBuffer& buffer, Sender& sender, LogList* logList );

    /*!
     * \brief buildPolynome:  method for polynome-plot generation.
     *
     * \param buffer: x & y data variables.
     * \param sender: integration method.
     * \param stack: stack for data log system (in development).
     */
    void buildPolynome( SpecialBuffer& buffer, Sender& sender, LogList* logList );

    /*!
     * \brief interpolationSolve: method for python polynomials.py module invokation.
     *
     * \param x: x data variables.
     * \param y: y data variables.
     * \param sender: polynomials module with specific method.
     */
    void interpolationSolve( const std::vector<double>& x, const std::vector<double>& y, Sender& sender );

    /*!
     * \brief differentiationSolve: method for python differentiation.py module invokation.
     *
     * \param x: x data variables.
     * \param y: y data variables.
     * \param sender: differentiation module with specific method.
     */
    void differentiationSolve( SpecialBuffer& buffer, const QVector<double>& x, const QVector<double>& y, Sender& sender );

    /*!
     * \brief integrationSolve: method for python integration.py module invokation.
     *
     * \param x: x data variables.
     * \param y: y data variables.
     * \param sender: integration module with specific method.
     */
    void integrationSolve( const QVector<double>& x, const QVector<double>& y, Sender& sender );

    /*!
     * \brief sysSolve: method for python laes_solver.py module invokation.
     *
     * \param data: map of equations arguments.
     * \param sender: laes_solver module with specific method.
     */
    void sysSolve( const QVector<QVector<double>>& data, const Sender& sender );

    void checkoutAxeses( void );

    /*!
     * \brief clearGraph: clear plot content.
     */
    void clearGraph( void );

    /*!
     * \brief moveLegend: move legend clockwise.
     */
    void moveLegend( void );

    /*!
     * \brief showLegend: set plot visible.
     */
    void showLegend( void );

    /*!
     * \brief hideLegend: set plot invisible.
     */
    void hideLegend( void );

    /*!
     * \brief stepBack: move backward through the history of data saving on the chart.
     */
    void stepBack( void );

    /*!
     * \brief stepForward: move forward through the history of data saving on the chart.
     */
    void stepForward( void );

    /*!
     * \brief zoomIn: increase zoom on plot.
     */
    void zoomIn();

    /*!
     * \brief zoomOut: decrease zoom on plot.
     */
    void zoomOut();
    /*!
    * \brief resetZoom: resets the zoom level of the graph to its default state.
    */
    void resetZoom();

    /*!
    * \brief couldSavePlotAsImage: enables or disables the ability to save the plot as an image.
    *
    * \param couldSave: boolean flag indicating whether the plot can be saved.
    */
    void couldSavePlotAsImage( bool couldSave );

    /*!
    * \brief unpinGraph: unpins the graph, allowing it to be freely moved or resized.
    */
    void unpinGraph();

    /*!
     * \brief savePlotAsImage: open a window to save the graph as an image.
     */
    void savePlotAsImage();

public slots:

    void printFuncGraph(const QVector<double> &x, const QVector<double> &y);

    /*!
     * \brief drawInterpolationGraph: draw points of interpolation from X|Y table and calculate curve by interpolation model.
     *
     * \param x: x data variables.
     * \param y: y data variables.
     */
    void drawInterpolationGraph( const std::vector<double> x, const std::vector<double> y );

    /*!
     * \brief rebuildWidgets: change interaction window.
     *
     * \param modules: window corresponding to this module.
     */
    void rebuildWidgets( pymodules::Modules modules );

    /*!
     * \brief printDerivationGraph: method for drawing a derivative graph
     *
     * \param x: x data variables.
     * \param y: y data variables.
     * \param sender: differentiation module with specific method.
     * \param stack for data log system (in development).
     */
    void printDerivationGraph( const QVector<double>& x, const QVector<double>& y, Sender& sender, LogList* stack );

    /*!
     * \brief updateLegend: updates the graph legend with the provided text.
     *
     * \param legendText: text to display in the legend.
     */
    void updateLegend( const QString& legendText );
    /*!
     * \brief setFunctionText: sets the function text displayed on the graph.
     *
     * \param functionText: mathematical function in string form.
     */
    void setFunctionText( const QString& functionText );

    /*!
     * \brief hideBarButtons: hides or shows the bar buttons on the graph.
     *
     * \param hide: true to hide the buttons, false to show them.
     */
    void hideBarButtons( const bool& hide );

    void acceptNonLinearSys( const QString& sys );

signals:
    /*!
     * \brief errorOccured: error mesage sender signal for leftWidget object.
     *
     * \param err: specific error message string.
     */
    void errorOccured( const QString& err );

    /*!
     * \brief readyToSendData: signal for leftWidget object for calculation of a function in the given ranges.
     *
     * \param expr: string typed math expression.
     * \param a: x-from.
     * \param b: x-to.
     */
    void readyToSendData( const QString& expr, const double a, const double b );

    /*!
     * \brief sendData: signal for leftWidget object for log data system.
     *
     * \param data: any object that can be figured in session mode.
     * \param toRemove: remove or save object.
     */
    void sendData( QVariant& data, bool toRemove );

    /*!
     * \brief readyToSendArea: signal for leftWidget object for area line-edit setup.
     * \param area: real number of calculated area.
     */
    void readyToSendArea( std::string& area );

    /*!
     * \brief readyToSendSysResult: signal for leftWidget object for equations line-edit setup.
     * \param result: intersection points.
     */
    void readyToSendSysResult( const QString &result );
    /*!
    * \brief readyToSendNonLinearSysResult: signal emitted when the solution of a non-linear system is ready.
    *
    * \param result: vector of results representing the solution of the non-linear system.
    */
    void readyToSendNonLinearSysResult( const QVector<double> &result );

    /*!
    * \brief calculateError: signal emitted to calculate the error between two sets of data.
    *
    * \param y1: first set of y data values.
    * \param y2: second set of y data values to compare.
    */
    void calculateError( const QVector<double>& y1, const QVector<double>& y2 );
};

#endif // RIGHTWIDGET_H

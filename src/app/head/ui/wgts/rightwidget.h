/*!
 * \attention In development.
 *
 * \author Malaniya Mark Timurovich, Dnevnie Mechaniki.
 *
 * \date last update: 18.05.2024.
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
    QString currentLegend;
    QString functionText;
    QString eqSysText;
    GraphBar* graphBar;

    int graphBarWidth, graph2dHeight, graph3dHeight;;

    bool unpinned{ false };

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
    void differentiationSolve( const QVector<double>& x, const QVector<double>& y, Sender& sender );

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

    void resetZoom();

    void couldSavePlotAsImage( bool couldSave );

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
    void updateLegend( const QString& legendText );
    void setFunctionText( const QString& functionText );

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

    void readyToSendNonLinearSysResult( const QVector<double> &result );

    void calculateError( const QVector<double>& y1, const QVector<double>& y2 );
};

#endif // RIGHTWIDGET_H

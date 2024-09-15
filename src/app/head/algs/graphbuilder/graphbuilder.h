/*!
 * \brief The GraphBuilder class for painting graphics
 *
 * \company Dnevnie Mechaniki
 *
 * \author Vilenskiy Kirill Romanovich
 *
 * \date last update: 12.07.2024
 */

#ifndef GRAPHBUILDER_H
#define GRAPHBUILDER_H

#include <qcustomplot/qcustomplot.h>
#include <QDebug>
#include <string>
#include <QString>
#include <QVector>
#include <QWidget>
#include <QList>
#include <QVBoxLayout>
#include "graphInfo.h"
#include <optional>
#include <QMatrix4x4>
#include "gl3dgraphbuilder.h"

struct GraphState
{
    QVector<GraphInfo> graphInfoList; ///< List for saving the graph info.

    QString name; ///< Name of the graph.

    QVector<double> xAxis; ///< Array of X coordinates of the graph.
    QVector<double> yAxis; ///< Array of Y coordinates of the graph.

    bool graphOn; ///< A variable responsible for drawing or not drawing a graph line.
    bool scatterOn; ///< A variable responsible for drawing or not drawing a scatter.
    const std::optional<bool> fillingOn;
};

/*!
 * \class GraphBuilder.
 *
 * \brief GraphBuilder: A class that builds and edits graphs.
 */
class GraphBuilder : public QWidget, protected QOpenGLFunctions
{
    Q_OBJECT
private:

    enum WidgetType { ThreeDWidget, CustomPlotWidget };

    QCPItemTracer* tracer; ///< Tracer linked to data points.
    QVBoxLayout* layout; ///< The layout to which all other objects are written.
    QCPItemText* textItem; ///< Item that displays the coordinates of the point where the user is pointing.

    QList<QCustomPlot*>* plots; ///< List that contains all states plot.

    int i = 0; ///< Graph count variable.
    int l = 0; ///< Legend position number.
    int currentindex; ///< Current index.

    double xmax = 2.0; ///< Maximum X coordinate space.
    double xmin = -2.0; ///< Minimum X coordinate space.
    double ymax = 2.0; ///< Maximum Y coordinate space.
    double ymin = -2.0; ///< Minimum Y coordinate space.

    float scale = 1.0f;
    float xRot, yRot, zRot;
    QPoint lastPos;
    float x[50], y[50], z[50];


    QVector<GraphInfo> graphInfoList; ///< List for saving the graph info.

    std::vector<GraphState> graphStates; ///< Graphic state vector.
    std::vector<GraphState>::iterator currentState; ///< Iterator of the current state graph.

    bool unpinned{ false }; ///< The variable indicates whether the graph window is detached from the general program.

public:
    QCustomPlot* graph2d; ///< The object to which all graphs are recorded.
    GL3DGraphBuilder* graph3d;
    QColor color;

public:
    /*!
     * \brief GraphBuilder: Creates a space where everything happens.
     *
     * \param parent - Pointer to the parent widget.
     */
    explicit GraphBuilder( QWidget* parent = nullptr );

private:
    /*!
     * \brief updateGraphState: Builds python list from std::vector<double>.
     *
     * \param state - Graph state variable.
     *
     * \return Graph state.
     */
    void updateGraphState( const GraphState& state );

public slots:
    /*!
     * \brief PaintG: Create and paint graphs.
     *
     * \param x - An array of vector x coordinates.
     * \param y - An array of vector y coordinates.
     * \param name - Name of the graph.
     * \param graphOn - A variable responsible for drawing or not drawing a graph line.
     * \param scatterOn - A variable responsible for drawing or not drawing a scatter.
     *
     * \return Graph.
     */
    std::optional<QCustomPlot*> PaintG( const QVector<double>& x, const QVector<double>& y, const QString& name, bool graphOn, bool scatterOn, bool fillingOn , const std::optional<QVector<double>> z = std::nullopt );

    /*!
     * \brief onClearButtonClicked: Erases all graphics.
     */
    void onClearButtonClicked( void );

    /*!
     * \brief resetZoom: Returns the graph window to its original position.
     */
    void resetZoom();

    /*!
     * \brief onMousMove: When you move the cursor, it shows the coordinates of the point. which the user points to.
     *
     * \param event - Event that tracks mouse movement.
     *
     * \return Mouse position.
     */
    void onMousMove(QMouseEvent* event);

    /*!
     * \brief textVisible: Prevents coordinates from being displayed above the graph.
     *
     * \param event - Event that tracks mouse movement.
     *
     * \return Mouse position.
     */
    void textVisible(QMouseEvent* event);

    /*!
     * \brief moveLegend: Moves the graph legend.
     */
    void moveLegend();

    /*!
     * \brief showLegend: Enable graphics display.
     */
    void showLegend();

    /*!
     * \brief hideLegend: Turns off graphics display.
     */
    void hideLegend();

    /*!
     * \brief GoBack: Rolls back the number of charts by one step.
     */
    void GoBack();

    /*!
     * \brief GoFront: Returns the number of graphs one step ahead.
     */
    void GoFront();

    /*!
     * \brief zoomIn: The schedule is approaching.
     */
    void zoomIn();

    /*!
     * \brief zoomOut: Zooms out the chart.
     */
    void zoomOut();

    /*!
     * \brief savePlotAsImage: Saves graphs.
     */
    void savePlotAsImage( void );


    void setupThreeCView( void );

    void mousePressEventd( QMouseEvent* event );

    void mouseMoveEventd( QMouseEvent* event );

    void wheelEventd( QWheelEvent* event );

    void switchToGL3DGraphBuilder( void );

    void switchToGraphBuilder( void );

signals:
    /*!
     * \brief couldSavePlotAsImage: Не дает сохранения пустого пространства.
     *
     * \param couldSave - Allow or disable saving the scene.
     */
    void couldSavePlotAsImage( const bool& couldSave );

    void hideBarButtons( const bool& hide );
};

#endif // GRAPHBUILDER_H

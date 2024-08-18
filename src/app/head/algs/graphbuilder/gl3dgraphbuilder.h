#ifndef GL3DGRAPBUILDER_H
#define GL3DGRAPBUILDER_H

#include "qevent.h"
#include <QFileDialog>
#include <QDebug>
#include <QInputDialog>
#include <QImage>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLFramebufferObject>
#include <GL/gl.h>

class GL3DGraphBuilder : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
private:
    QMatrix4x4 projectionMatrix;
    QMatrix4x4 modelView;
    QVector<QVector3D> points;

    QMatrix4x4 cameraTransform;
    QMatrix4x4 rotation;
    QPoint lastMousePos;

    struct GraphData
    {
        QVector<QVector3D> vertices;
        QColor color;
    };

    QVector<GraphData> figures;
    QVector<GraphData> buffer;

    QList<QVector<QVector3D>> graphStates;
    int currentState = 0;

    float axisLineWidth;
    float gridLineWidth;
    float pointSize;

public:
    GL3DGraphBuilder( QWidget* parent = nullptr );

    void buildGraph( const QVector<QVector3D>& points )
    {
        GraphData graphData;
        graphData.vertices = points;
        graphData.color = getRandomColor();
        figures.push_back(graphData);
        update();
    }

    void onClearButtonClicked( void );
    void resetZoom( void );
    void zoomIn( void );
    void zoomOut( void );
    void stepBack( void );
    void stepForward( void );
    void savePlotAsImage( void );

protected:

    void initializeGL() override;
    void resizeGL( int width, int height ) override;
    void paintGL() override;

    void mousePressEvent( QMouseEvent* event ) override;
    void mouseMoveEvent( QMouseEvent* event ) override;
    void wheelEvent( QWheelEvent* event ) override;

private:
    void drawAxes( void );
    void drawGrid( void );
    QColor getRandomColor( void )
    {
        return QColor::fromRgbF( static_cast<float>(rand())/RAND_MAX,
                                static_cast<float>(rand())/RAND_MAX,
                                static_cast<float>(rand())/RAND_MAX );
    }

    void render( QOpenGLFramebufferObject *fbo );

signals:
    void dataUpdated();
};

#endif //GL3DGRAPBUILDER_H

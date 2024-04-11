#ifndef GRAPHBUILDER_H
#define GRAPHBUILDER_H

#include <QMainWindow>
#include <qcustomplot/qcustomplot.h>
#include <QDebug>
#include <string>
#include <QString>
#include <QWidget>

class GraphBuilder : public QWidget
{
    Q_OBJECT

private:
    QCPItemTracer* tracer;
    QGridLayout* layout;

    int i = 0;

    double xmax = 2.0;
    double xmin = -2.0;
    double ymax = 2.0;
    double ymin = -2.0;

public:
    QCustomPlot* wGraphic;

public:
    explicit GraphBuilder( QWidget *parent = nullptr );

public slots:
    void PaintG( QVector<double>& x, QVector<double>& y, const QString& name, bool graphOn, bool scatterOn);
    void on_clearButton_clicked( void );
    void ZoomB();

};
#endif // GRAPHBUILDER_H

#include "graphbuilder.h"
#include <QApplication>
#include <QGridLayout>
#include <qcustomplot/qcustomplot.h>
#include <QtPrintSupport/QPrinter>
#include <algorithm>
#include <vector>
#include <string>
#include <QString>
#include <QRandomGenerator>
#include <QColor>

GraphBuilder::GraphBuilder( QWidget* parent )
    : QWidget( parent )
{
    layout = new QGridLayout( this );
    wGraphic = new QCustomPlot( this );

    wGraphic->setMinimumSize( 450, 380 );

    tracer = new QCPItemTracer( wGraphic );
    tracer->setGraph( wGraphic->graph( 0 ) );

    wGraphic->xAxis->setLabel( "x" );
    wGraphic->yAxis->setLabel( "y" );

    wGraphic->xAxis->setRange( xmin, xmax );
    wGraphic->yAxis->setRange( ymin, ymax );

    wGraphic->replot();

    layout->addWidget( wGraphic );
}

void GraphBuilder::PaintG( QVector<double>& xAxis, QVector<double>& yAxis, const QString& name )
{
    auto maxXElement = std::max_element( xAxis.begin(), xAxis.end() );
    auto minXElement = std::min_element( xAxis.begin(), xAxis.end() );
    auto maxYElement = std::max_element( yAxis.begin(), yAxis.end() );
    auto minYElement = std::min_element( yAxis.begin(), yAxis.end() );
    if ( xmax < *maxXElement )
    {
        if( xmin > *minXElement )
        {
            xmax = *maxXElement;
            xmin = *minXElement;
            wGraphic->xAxis->setRange( xmin, xmax );
            wGraphic->replot();
            wGraphic->update();
        }
        else
        {
            xmax = *maxXElement;
            wGraphic->xAxis->setRange( xmin, xmax );
            wGraphic->replot();
            wGraphic->update();
        }
    }
    else if( xmin > *minXElement )
    {
        xmin = *minXElement;
        wGraphic->xAxis->setRange( xmin, xmax );
        wGraphic->replot();
        wGraphic->update();

    }


    if ( ymax < *maxYElement )
    {
        if( ymin > *minYElement )
        {
            ymax = *maxYElement;
            ymin = *minYElement;
            wGraphic->yAxis->setRange( ymin, ymax );
            wGraphic->replot();
            wGraphic->update();
        }
        else
        {
            ymax = *maxYElement;
            wGraphic->yAxis->setRange( ymin, ymax );
            wGraphic->replot();
            wGraphic->update();
        }
    }
    else if( ymin > *minYElement )
    {
        ymin = *minYElement;
        wGraphic->yAxis->setRange( ymin, ymax );
        wGraphic->replot();
        wGraphic->update();
    }

    wGraphic->addGraph( wGraphic->xAxis, wGraphic->yAxis );
    wGraphic->graph( i )->setData( xAxis, yAxis );
    //wGraphic->graph(i)->setInterpolation(trou);
    QColor color = QColor::fromRgb( QRandomGenerator::global()->bounded( 255 ),
                                    QRandomGenerator::global()->bounded( 255 ),
                                    QRandomGenerator::global()->bounded( 255 ) );
    QPen pin( color );
    wGraphic->graph( i )->setPen( pin );
    wGraphic->graph( i )->setScatterStyle( QCPScatterStyle::ssCircle );
    wGraphic->graph( i )->setName( name );
    //wGraphic->legend->setVisible( true );
    QPen pen = wGraphic->graph( i )->pen();
    pen.setWidth( 4 );
    wGraphic->graph( i )->setPen( pen );
    i++;
    wGraphic->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom |QCP::iSelectPlottables);
    wGraphic->axisRect()->setRangeZoomFactor(Qt::Horizontal,0.85);
    wGraphic->axisRect()->setRangeZoomFactor(Qt::Vertical,0.85);
}


void GraphBuilder::on_clearButton_clicked()
{
    for( int c{}; c < i; ++c )
    {
        wGraphic->graph( c )->data()->clear();
    }
    wGraphic->legend->clear();
    wGraphic->legend->setVisible( false );
    wGraphic->replot();
    wGraphic->update();
}
void GraphBuilder::ZoomB(){
    wGraphic->xAxis->setRange( xmin, xmax );
    wGraphic->yAxis->setRange( ymin, ymax );
    wGraphic->replot();
    wGraphic->update();
}


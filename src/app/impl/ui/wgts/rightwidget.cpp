#include "rightwidget.h"
#include <qstring.h>
#include <filesystem>
#include <QString>
#include <QVariant>

void RightWidget::buildWidgetForDerivativeOperations( void )
{
    graphBuilder->onClearButtonClicked();
    graphBuilder->graph2d->yAxis->setLabel( "y'" );
}

RightWidget::RightWidget( QWidget *parent )
    : QWidget{ parent }
{
    graphBar = new GraphBar( this );
    graphBuilder = new GraphBuilder( this );
    conveyor = new PythonConveyor();
    rightLayout = new QGridLayout( this );

    connect( graphBar->actions().at( 0 ), &QAction::triggered, this, &RightWidget::clearGraph            );
    connect( graphBar->actions().at( 1 ), &QAction::triggered, this, &RightWidget::resetZoom             );
    connect( graphBar->actions().at( 2 ), &QAction::triggered, this, &RightWidget::moveLegend            );
    connect( graphBar->actions().at( 3 ), &QAction::triggered, this, &RightWidget::showLegend            );
    connect( graphBar->actions().at( 4 ), &QAction::triggered, this, &RightWidget::stepBack              );
    connect( graphBar->actions().at( 5 ), &QAction::triggered, this, &RightWidget::stepForward           );
    connect( graphBar->actions().at( 6 ), &QAction::triggered, this, &RightWidget::zoomOut               );
    connect( graphBar->actions().at( 7 ), &QAction::triggered, this, &RightWidget::zoomIn                );
    connect( graphBar->actions().at( 8 ), &QAction::triggered, this, &RightWidget::unpinGraph            );
    connect( graphBar->actions().at( 9 ), &QAction::triggered, this, &RightWidget::savePlotAsImage       );
    connect( graphBuilder, &GraphBuilder::couldSavePlotAsImage, this, &RightWidget::couldSavePlotAsImage );

    connect( graphBuilder, &GraphBuilder::hideBarButtons, this, &RightWidget::hideBarButtons );

    rightLayout->addWidget( graphBar );
    rightLayout->addWidget( graphBuilder );
    currentLegend = "График заданной функции";
    functionText = "График заданной функции";
}

void RightWidget::updateLegend( const QString& legendText )
{
    currentLegend = legendText;
    qDebug() << "Легенда обновлена на:" << currentLegend;
}

void RightWidget::setFunctionText( const QString& text )
{
    functionText = text;
}

void RightWidget::hideBarButtons( const bool& hide )
{
    graphBar->actions().at( 1 )->setDisabled( hide );
    graphBar->actions().at( 2 )->setDisabled( hide );
    graphBar->actions().at( 3 )->setDisabled( hide );
    graphBar->actions().at( 6 )->setDisabled( hide );
    graphBar->actions().at( 7 )->setDisabled( hide );
}

void RightWidget::acceptNonLinearSys( const QString& sys )
{
    qDebug() << "acceptNonLinearSys\n";
    eqSysText = sys;
    auto xy = new QVector<QVector<double>>();
    auto send = new Sender();
    sysSolve( *xy, *send );
}

void RightWidget::printGraph( SpecialBuffer& buffer, Sender& sender, LogList* logList )
{
    x = buffer.x;
    y = buffer.y;
    z = buffer.z;

    checkoutAxeses();

    graphBuilder->graph2d->replot();
    std::optional<QCustomPlot*> graph = graphBuilder->PaintG( x, y, functionText, true, false, false, z );

    if (logList && graph.has_value())
    {
        LogList::Item item;
        item.item = LogList::ItemType::QString;
        item.str = new QString(QString(
                                   "При заданном числовом диапазоне x[ %1, %2 ], функции %3, получен график функции:\n"
                                   ).arg(x.takeFirst())
                                   .arg(x.takeLast())
                                   .arg(functionText));
        logList->addItem(item);

        QCustomPlot* customPlot = new QCustomPlot();
        auto gb = new GraphBuilder( customPlot );

        gb->graph2d->addGraph();
        gb->graph2d->graph(0)->setData(x, y);
        gb->graph2d->graph(0)->setPen(graphBuilder->color);
        QPen pen = graphBuilder->graph2d->graph()->pen();
        pen.setWidth( 4 );
        gb->graph2d->graph()->setPen( pen );

        gb->graph2d->xAxis->setRange(x.takeFirst(), x.takeLast());
        gb->graph2d->yAxis->setRange(*std::min_element(y.begin(), y.end()), *std::max_element(y.begin(), y.end()));
        gb->graph2d->xAxis->setLabel("x");
        gb->graph2d->yAxis->setLabel("f(x)");

        gb->graph2d->rescaleAxes();
        gb->graph2d->replot();

        item.item = LogList::ItemType::QCustomPlot;
        item.gbd = gb;
        logList->addItem(item);
    }
}

void RightWidget::printFuncGraph( const QVector<double>& x, const QVector<double>& y)
{
    checkoutAxeses();

    graphBuilder->graph2d->replot();
    std::optional<QCustomPlot*> graph = graphBuilder->PaintG( x, y, functionText, true, false, false, z );

}

void RightWidget::printDerivationGraph( const QVector<double>& x, const QVector<double>& y, Sender& sender, LogList* logList )
{
    graphBuilder->graph2d->replot();
    auto graph = graphBuilder->PaintG( x, y, "График производной функции " + functionText + " (" + currentLegend + ")", true, false, false );

    std::vector<double> _x(std::begin(x), std::end(x));
    std::vector<double> _y(std::begin(y), std::end(y));

    if( logList && graph.has_value() )
    {
        LogList::Item item;
        item.item = LogList::ItemType::QString;
        item.str = new QString(QString(
                                   "С данным числовым диапозоном x[ %1, %2 ], функции %3, получен график производной от данной функции:\n"
                                   ).arg(_x.at(0))
                                   .arg(_x.at(_x.size()))
                                   .arg(functionText));
        logList->addItem(item);

        QCustomPlot* customPlot = new QCustomPlot();
        auto gb = new GraphBuilder( customPlot );
        gb->graph2d->addGraph();
        gb->graph2d->graph(0)->setData(x, y);
        gb->graph2d->graph(0)->setPen(graphBuilder->color);
        QPen pen = graphBuilder->graph2d->graph()->pen();
        pen.setWidth( 4 );
        gb->graph2d->graph()->setPen( pen );
        gb->graph2d->xAxis->setRange(_x.at(0), _x.at(_x.size()));
        gb->graph2d->yAxis->setRange(*std::min_element(y.begin(), y.end()), *std::max_element(y.begin(), y.end()));
        gb->graph2d->xAxis->setLabel("x");
        gb->graph2d->yAxis->setLabel("f'(x)");

        gb->graph2d->rescaleAxes();
        gb->graph2d->replot();
        item.item = LogList::ItemType::QCustomPlot;
        item.gbd = gb;
        logList->addItem(item);
    }
}

void RightWidget::printDiffGraph( SpecialBuffer &buffer, Sender &sender, LogList* logList )
{
    x = buffer.x;
    y = buffer.y;

    buildWidgetForDerivativeOperations();
    graphBuilder->graph2d->replot();

    differentiationSolve( buffer, x, y, sender );

    if( logList )
    {
        LogList::Item item;
        item.item = LogList::ItemType::QString;
        item.str = new QString(QString(
                                   "С данным числовым диапозоном x[ %1, %2 ], функции %3, получен график разности между исходной функцией и полученной моделью:\n"
                                   ).arg(x.takeFirst())
                                   .arg(x.takeLast())
                                   .arg(functionText));
        logList->addItem(item);

        QCustomPlot* customPlot = new QCustomPlot();
        item.item = LogList::ItemType::QCustomPlot;
        auto gb = new GraphBuilder( customPlot );

        gb->graph2d->addGraph();
        gb->graph2d->graph(0)->setData(x, y);
        gb->graph2d->graph(0)->setPen(graphBuilder->color);
        QPen pen = graphBuilder->graph2d->graph()->pen();
        pen.setWidth( 4 );
        gb->graph2d->graph()->setPen( pen );
        gb->graph2d->xAxis->setRange(x.takeFirst(), x.takeLast());
        gb->graph2d->yAxis->setRange(*std::min_element(y.begin(), y.end()), *std::max_element(y.begin(), y.end()));
        gb->graph2d->xAxis->setLabel("x");
        gb->graph2d->yAxis->setLabel("f(x) - P(x)");

        gb->graph2d->rescaleAxes();

        gb->graph2d->replot();

        item.gbd = gb;
        logList->addItem(item);
    }
}

void RightWidget::calculateIntegral( SpecialBuffer& buffer, Sender& sender, LogList* logList )
{
    x = buffer.x;
    y = buffer.y;

    checkoutAxeses();

    integrationSolve( x, y, sender );
    graphBuilder->graph2d->replot();
    auto graph = graphBuilder->PaintG( x, y, "Площадь функции " + functionText  + " (" + currentLegend + ")", true, false, true );

    if( logList )
    {
        LogList::Item item;
        item.item = LogList::ItemType::QString;
        item.str = new QString(QString(
                                   "С числовым диапозоном x[ %1, %2 ], функции %3, был получен график криволинейной трапеции, площадь которой равна S = %4:\n"
                                   ).arg(x.takeFirst())
                                   .arg(x.takeLast())
                                   .arg(functionText)
                                   .arg(area.c_str()));
        logList->addItem(item);

        QCustomPlot* customPlot = new QCustomPlot();
        item.item = LogList::ItemType::QCustomPlot;
        auto gb = new GraphBuilder( customPlot );
        gb->graph2d->addGraph();
        gb->graph2d->graph(0)->setData(x, y);
        gb->graph2d->graph(0)->setPen(graphBuilder->color);
        QPen pen = graphBuilder->graph2d->graph()->pen();
        pen.setWidth( 4 );
        gb->graph2d->graph()->setPen( pen );
        gb->graph2d->xAxis->setRange(x.takeFirst(), x.takeLast());
        gb->graph2d->yAxis->setRange(*std::min_element(y.begin(), y.end()), *std::max_element(y.begin(), y.end()));
        gb->graph2d->xAxis->setLabel("x");
        gb->graph2d->yAxis->setLabel("∫f(x)dx");

        gb->graph2d->rescaleAxes();
        gb->graph2d->replot();
        item.gbd = gb;
        logList->addItem(item);
    }
}

void RightWidget::buildPolynome( SpecialBuffer &buffer, Sender &sender, LogList* logList )
{
    x = buffer.x;
    y = buffer.y;

    if( x.size() > limits::NODES_LIMIT )
    {
        emit errorOccured( "Не больше 10 узлов" );
        return;
    }
    if( x.back() < x.first() )
    {
        emit errorOccured( "Введите значения x по возрастанию" );
        return;
    }

    checkoutAxeses();

    graphBuilder->graph2d->replot();
    graphBuilder->PaintG( x, y, "Точки интерполяции", false, true, false );
    std::vector<double> _x(std::begin(x), std::end(x));
    std::vector<double> _y(std::begin(y), std::end(y));
    interpolationSolve( _x, _y, sender );
  
    QString str = QString::fromUtf8( resultModel.c_str() );
    emit readyToSendData( str, x[0], x.back() );

    if (logList) {
        LogList::Item item;
        item.item = LogList::ItemType::QString;
        item.str = new QString(QString(
                                   "С заданным числовым диапозоном x[ %1, %2 ] получена модель интерполяции: P(x) = %3\n"
                                   ).arg(x.takeFirst())
                                   .arg(x.takeLast())
                                   .arg(str));


        logList->addItem(item);

        QCustomPlot* customPlot = new QCustomPlot();
        auto gb = new GraphBuilder(customPlot);
        gb->graph2d->addGraph();
        gb->graph2d->graph(0)->setData(x, y);
        gb->graph2d->graph(0)->setPen(graphBuilder->color);
        QPen pen = graphBuilder->graph2d->graph()->pen();
        pen.setWidth(4);
        gb->graph2d->graph()->setPen(pen);

        gb->graph2d->xAxis->setRange(x.takeFirst(), x.takeLast());
        gb->graph2d->yAxis->setRange(*std::min_element(y.begin(), y.end()), *std::max_element(y.begin(), y.end()));
        gb->graph2d->xAxis->setLabel("x");
        gb->graph2d->yAxis->setLabel("P(x)");

        gb->graph2d->rescaleAxes();
        gb->graph2d->replot();

        item.item = LogList::ItemType::QCustomPlot;
        item.gbd = gb;
        logList->addItem(item);
        x.clear();
        y.clear();
    }
}

void RightWidget::interpolationSolve( const std::vector<double> &x, const std::vector<double> &y, Sender& sender )
{
    QString str = conveyor->sendArraysToPythonFunction( sender.moduleName, sender.functionName, x, y );
    resultModel = str.toStdString();
}

void RightWidget::integrationSolve( const QVector<double>& x, const QVector<double>& y, Sender &sender )
{
    QString resultStr = conveyor->sendDataToIntegration( sender.moduleName, sender.functionName, x, y );
    area = resultStr.toStdString();
    emit readyToSendArea( area );
}

void RightWidget::differentiationSolve( SpecialBuffer& buffer, const QVector<double>& x, const QVector<double>& y, Sender& sender )
{
    std::pair< QVector<double>, QVector<double> > diffResult = conveyor->sendDataToDifferentiation( sender.moduleName, sender.functionName, x, y );
    QVector<double> resultX = diffResult.first;
    QVector<double> resultY = diffResult.second;

    std::vector<double> first;
    first.assign(resultY.begin(), resultY.end());
    std::vector<double> second;
    second.assign(buffer.dy.begin(), buffer.dy.end());

    auto dy1 = QVector<double>(first.begin(), first.end());
    auto dy2 = QVector<double>(second.begin(), second.end());

    emit calculateError( dy1, dy2 );

    printDerivationGraph( resultX, resultY, sender, nullptr );
}

void RightWidget::sysSolve( const QVector<QVector<double>>& data, const Sender& sender )
{
    if( eqSysText.size() > 2 )
    {
        QVector<double> resultSysVector = conveyor->sendDataToSolveNonLinearSys(":/pyFiles/resources/pymodules/nonlinear_laes_solver.py", "solve_nonlinear_system", eqSysText );
        qDebug() << "getting vector: ";
        for( const auto& e : resultSysVector )
        {
            qDebug() << e << "\n";
        }
        eqSysText.clear();
        return;
    }

    QString resultSysStr = conveyor->sendDataToSolveSys( sender.moduleName, sender.functionName, data );
    emit readyToSendSysResult( resultSysStr );
    qDebug() << resultSysStr;
}

void RightWidget::checkoutAxeses( void )
{
    if( graphBuilder->graph2d->yAxis->label() == "y'" )
    {
        graphBuilder->onClearButtonClicked();
        graphBuilder->graph2d->yAxis->setLabel( "y" );
    }
}

void RightWidget::clearGraph( void )
{
    if( graphBuilder->graph2d->isVisible() )
    {
        graphBuilder->onClearButtonClicked();
        return;
    }
    graphBuilder->graph3d->onClearButtonClicked();
}

void RightWidget::drawInterpolationGraph( const std::vector<double> x, const std::vector<double> y )
{
    QVector<double> X = QVector<double>( x.begin(), x.end() );
    QVector<double> Y = QVector<double>( y.begin(), y.end() );
    this->x = X;
    this->y = Y;
    graphBuilder->PaintG( X, Y, "График интерполяции (" + currentLegend + ")", true, false, false );
}

void RightWidget::moveLegend( void )
{
    graphBuilder->moveLegend();
}

void RightWidget::showLegend( void )
{
    graphBuilder->showLegend();
    if( !graphBar->legendEnabled )
    {
        graphBar->actions().at( 3 )->setIcon( QIcon( ":/toolbaricons/resources/hideLegend.PNG" ) );
        graphBuilder->showLegend();
        graphBar->legendEnabled = true;
        return;
    }
    graphBar->actions().at( 3 )->setIcon( QIcon( ":/toolbaricons/resources/showLegend.PNG" ) );
    graphBuilder->hideLegend();
    graphBar->legendEnabled = false;
}

void RightWidget::hideLegend( void )
{
    graphBuilder->hideLegend();
}

void RightWidget::stepBack( void )
{
    if( graphBuilder->graph2d->isVisible() )
    {
        graphBuilder->GoBack();
        return;
    }
    graphBuilder->graph3d->stepBack();
}

void RightWidget::stepForward( void )
{
    if( graphBuilder->graph2d->isVisible() )
    {
        graphBuilder->GoFront();
        return;
    }
    graphBuilder->graph3d->stepForward();
}

void RightWidget::zoomIn( void )
{
    if( graphBuilder->graph2d->isVisible() ) [[likely]]
    {
        graphBuilder->zoomIn();
        return;
    }
    graphBuilder->graph3d->zoomIn();
}

void RightWidget::zoomOut( void )
{
    if( graphBuilder->graph2d->isVisible() ) [[likely]]
    {
        graphBuilder->zoomOut();
        return;
    }
    graphBuilder->graph3d->zoomOut();
}

void RightWidget::resetZoom( void )
{
    if( graphBuilder->graph2d->isVisible() ) [[likely]]
    {
        graphBuilder->resetZoom();
        return;
    }
    //graphBuilder->graph3d->resetZoom();
}


void RightWidget::couldSavePlotAsImage( bool couldSave )
{
    graphBar->actions().at( 9 )->setEnabled( couldSave );
}

void RightWidget::unpinGraph( void )
{
    //graphBar->unsetChecked();
    graphBar->actions().at( 8 )->setChecked( true );

    if( unpinned )
    {
        graphBar->actions().at( 8 )->setChecked( false );
        return;
    }
    graphBar->actions().at( 8 )->setEnabled( false );

    graphBarWidth = graphBar->width();
    graph2dHeight = graphBuilder->graph2d->height();
    graph3dHeight = graphBuilder->graph3d->height();

    QDialog* dialog = new QDialog;
    dialog->setWindowFlags( dialog->windowFlags() & ~Qt::WindowSystemMenuHint );
    dialog->setWindowTitle( "Unpinned plot" );
    QVBoxLayout* layout = new QVBoxLayout( dialog );

    layout->addWidget( this->graphBar );
    layout->addWidget( graphBuilder->graph2d );
    layout->addWidget( graphBuilder->graph3d );
    dialog->setLayout( layout );

    connect( dialog, &QDialog::finished, this, [=]( int result )
        {
            Q_UNUSED( result );
            rightLayout->addWidget( graphBar );
            rightLayout->addWidget( graphBuilder->graph2d );
            rightLayout->addWidget( graphBuilder->graph3d );
            graphBar->setFixedWidth( graphBarWidth );
            graphBuilder->graph2d->setFixedHeight( graph2dHeight );
            graphBuilder->graph3d->setFixedHeight( graph3dHeight );
            graphBar->actions().at( 8 )->setEnabled( true );
            dialog->deleteLater();
            unpinned = false;
        }
    );

    dialog->show();
    unpinned = true;
    graphBar->actions().at( 8 )->setChecked( false );
}

void RightWidget::savePlotAsImage()
{
    if( graphBuilder->graph2d->isVisible() )
    {
        graphBuilder->savePlotAsImage();
        return;
    }
    graphBuilder->graph3d->savePlotAsImage();
}

void RightWidget::rebuildWidgets( pymodules::Modules modules )
{
    switch( modules )
    {
    case pymodules::Modules::DIFFERENTIATION:
        buildWidgetForDerivativeOperations();
        break;
    };
}


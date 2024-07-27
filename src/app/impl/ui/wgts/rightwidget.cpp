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
    graphBuilder = new GraphBuilder( this );
    conveyor = new PythonConveyor();
    rightLayout = new QGridLayout( this );
    modelLabel = new QLabel( "Полученная модель: ", this );
    model = new QLineEdit( this );
    rightLayout->addWidget( modelLabel );
    rightLayout->addWidget( model );
    rightLayout->addWidget( graphBuilder );
    currentLegend = "График заданной функции";
    functionText = "График заданной функции";
}

void RightWidget::updateLegend( const QString& legendText )
{
    currentLegend = legendText;
    qDebug() << "Легенда обновлена на:" << currentLegend;
}

void RightWidget::setFunctionText(const QString& text) {
    functionText = text;
    qDebug() << "Функция обновлена на:" << functionText;
}

void RightWidget::printGraph( SpecialBuffer& buffer, Sender& sender, const CompositeStateStack* stack )
{
    x = buffer.x;
    y = buffer.y;
    z = buffer.z;

    if( graphBuilder->graph2d->yAxis->label() == "y'" )
    {
        graphBuilder->onClearButtonClicked();
        graphBuilder->graph2d->yAxis->setLabel( "y" );
    }
    graphBuilder->graph2d->replot();
    // TODO: исправить заглушку
    graphBuilder->PaintG( x, y,  functionText , true, false, false, z );

    if( stack ) [[unlikely]]
    {
//        emit sendData( model, false );
//        emit sendData( *graphBuilder->graph2d, false );
    }
}

void RightWidget::printDerivationGraph( const QVector<double>& x, const QVector<double>& y, Sender& sender, const CompositeStateStack* stack  )
{
    graphBuilder->graph2d->replot();
    // TODO: исправить заглушку
    graphBuilder->PaintG( x, y, "График производной функции " + functionText + " (" + currentLegend + ")", true, false, false );

    if( stack ) [[unlikely]]
    {
//        emit sendData( *graphBuilder->graph2d, false );
    }
}


void RightWidget::printDiffGraph( SpecialBuffer &buffer, Sender &sender, const CompositeStateStack* stack )
{
    x = buffer.x;
    y = buffer.y;

    buildWidgetForDerivativeOperations();
    graphBuilder->graph2d->replot();

    differentiationSolve( x, y, sender );

    if( stack ) [[unlikely]]
    {
//        emit sendData( *graphBuilder->graph2d, false );
    }
}

void RightWidget::calculateIntegral( SpecialBuffer& buffer, Sender& sender, const CompositeStateStack* stack )
{
    x = buffer.x;
    y = buffer.y;

    integrationSolve( x, y, sender );
    graphBuilder->graph2d->replot();
    graphBuilder->PaintG( x, y, "Площадь функции " + functionText  + " (" + currentLegend + ")", true, false, true );

    if( stack ) [[unlikely]]
    {
//        emit sendData( *new QLineEdit( area.c_str() ), false );
    }
}

void RightWidget::buildPolynome( SpecialBuffer &buffer, Sender &sender, const CompositeStateStack* stack  )
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
    // TODO: В отдельный метод
    if( graphBuilder->graph2d->yAxis->label() == "y'" )
    {
        graphBuilder->onClearButtonClicked();
        graphBuilder->graph2d->yAxis->setLabel( "y" );
    }

    graphBuilder->graph2d->replot();
    graphBuilder->PaintG( x, y, "Точки интерполяции", false, true, false );
    std::vector<double> _x(std::begin(x), std::end(x));
    std::vector<double> _y(std::begin(y), std::end(y));
    interpolationSolve( _x, _y, sender );
  
    QString str = QString::fromUtf8( resultModel.c_str() );
    model->setText( str );
    emit readyToSendData( model->text(), x[0], x.back() );

    if( stack ) [[unlikely]]
    {
//        emit sendData( *model, false );
//        emit sendData( *graphBuilder->graph2d, false );
    }
}

void RightWidget::interpolationSolve( const std::vector<double> &x, const std::vector<double> &y, Sender& sender )
{
    conveyor->setData( &PythonConveyor::functionName, sender.functionName );
    conveyor->setData( &PythonConveyor::pythonFilePath, sender.moduleName );
    conveyor->setData( &PythonConveyor::xVector, x );
    conveyor->setData( &PythonConveyor::yVector, y );


    conveyor->sendArraysToPythonFunction();
    QString str = conveyor->getData( &PythonConveyor::resultString );
    resultModel = str.toStdString();
}

void RightWidget::integrationSolve( const QVector<double>& x, const QVector<double>& y, Sender &sender )
{
    conveyor->setData( &PythonConveyor::functionName, sender.functionName );
    conveyor->setData( &PythonConveyor::pythonFilePath, sender.moduleName );

    std::vector<double> _x(std::begin(x), std::end(x));
    std::vector<double> _y(std::begin(y), std::end(y));
    conveyor->setData( &PythonConveyor::xVector, _x );
    conveyor->setData( &PythonConveyor::yVector, _y );

    conveyor->sendDataToIntegration();
    QString resultStr = conveyor->getData( &PythonConveyor::resultString );
    area = resultStr.toStdString();
    emit readyToSendArea( area );
}

void RightWidget::differentiationSolve( const QVector<double>& x, const QVector<double>& y, Sender& sender )
{
    conveyor->setData( &PythonConveyor::functionName, sender.functionName );
    conveyor->setData( &PythonConveyor::pythonFilePath, sender.moduleName );

    std::vector<double> _x(std::begin(x), std::end(x));
    std::vector<double> _y(std::begin(y), std::end(y));

    conveyor->setData( &PythonConveyor::xVector, _x );
    conveyor->setData( &PythonConveyor::yVector, _y );

    conveyor->sendDataToDifferentiation();
    QVector<double> resultX = conveyor->getData( &PythonConveyor::resultDiff_XVector );
    QVector<double> resultY = conveyor->getData( &PythonConveyor::resultDiff_YVector );
    printDerivationGraph( resultX, resultY, sender, nullptr );
}

void RightWidget::sysSolve( QVector<QVector<double>>& data, Sender &sender )
{
    conveyor->setData( &PythonConveyor::functionName, sender.functionName );
    conveyor->setData( &PythonConveyor::pythonFilePath, sender.moduleName );

    conveyor->setData( &PythonConveyor::sys, data );

    conveyor->sendDataToSolveSys();
    QVector<double> resultSysVector = conveyor->getData( &PythonConveyor::resultSys_Vector );
    QString resultSysStr = conveyor->getData( &PythonConveyor::resultString );
    emit readyToSendSysResult( resultSysStr );
    qDebug() << resultSysStr;
}

void RightWidget::clearGraph( void )
{
    graphBuilder->onClearButtonClicked();
}

void RightWidget::drawInterpolationGraph( const std::vector<double> x, const std::vector<double> y )
{
    QVector<double> X = QVector<double>( x.begin(), x.end() );
    QVector<double> Y = QVector<double>( y.begin(), y.end() );
    graphBuilder->PaintG( X, Y, "График интерполяции (" + currentLegend + ")", true, false, false );
}

void RightWidget::moveLegend(void)
{
    graphBuilder->moveLegend();
}

void RightWidget::showLegend()
{
    graphBuilder->showLegend();
}

void RightWidget::hideLegend()
{
    graphBuilder->hideLegend();
}

void RightWidget::stepBack()
{
    graphBuilder->GoBack();
}

void RightWidget::stepForward()
{
    graphBuilder->GoFront();
}

void RightWidget::zoomIn()
{
    graphBuilder->zoomIn();
}

void RightWidget::zoomOut()
{
    graphBuilder->zoomOut();
}

void RightWidget::savePlotAsImage()
{
    graphBuilder->savePlotAsImage();
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

#include "rightwidget.h"
#include <qstring.h>
#include <filesystem>

RightWidget::RightWidget( QWidget *parent )
    : QWidget{ parent }
{
    graphBuilder = new GraphBuilder( this );
    conveyor = new PythonConveyor();
    rightLayout = new QGridLayout( this );
    label = new QLabel( "Полученная модель: ", this );
    model = new QLineEdit( this );
    rightLayout->addWidget( label );
    rightLayout->addWidget( model );
    rightLayout->addWidget( graphBuilder );
}

void RightWidget::printGraph( SpecialBuffer& buffer, Sender& sender, const CompositeStateStack* stack )
{
    x = buffer.x;
    y = buffer.y;

    graphBuilder->wGraphic->replot();
    // TODO: исправить заглушку
    graphBuilder->PaintG( x, y, sender.functionName == nullptr ? "" : sender.functionName, true, false );

    QString str = QString::fromUtf8( resultModel.c_str() );
    model->setText( str );

    if( stack )
    {
        emit sendData( *model, false );
        emit sendData( *graphBuilder->wGraphic, false );
    }
}

void RightWidget::printGraph( QVector<double>& x, QVector<double>& y, Sender& sender, const CompositeStateStack* stack  )
{
    graphBuilder->wGraphic->replot();
    // TODO: исправить заглушку
    graphBuilder->PaintG( x, y, "График производной функции", true, false );

    if( stack )
    {
        emit sendData( *graphBuilder->wGraphic, false );
    }
}


void RightWidget::printDiffGraph( SpecialBuffer &buffer, Sender &sender, const QString& expression, const CompositeStateStack* stack )
{
    x = buffer.x;

    graphBuilder->wGraphic->replot();

    differentiationSolve( expression, x, sender );

    if( stack )
    {
        emit sendData( *graphBuilder->wGraphic, false );
    }
}

void RightWidget::buildPolynome( SpecialBuffer &buffer, Sender &sender, const CompositeStateStack* stack  )
{
    x = buffer.x;
    y = buffer.y;
    if( x.size() > pymodules::NODES_LIMIT )
    {
        emit errorOccured( "Не больше 10 узлов" );
        return;
    }
    graphBuilder->wGraphic->replot();
    graphBuilder->PaintG( x, y, "Точки интерполяции", false, true );
    interpolationSolve( x.toStdVector(), y.toStdVector(), sender );
    QString str = QString::fromUtf8( resultModel.c_str() );
    model->setText( str );
    emit readyToSendData( model->text(), x[0], x.back() );

    if( stack )
    {
        emit sendData( *model, false );
        emit sendData( *graphBuilder->wGraphic, false );
    }
}

void RightWidget::interpolationSolve( const std::vector<double> &x, const std::vector<double> &y, Sender& sender )
{
    conveyor->setFunctionName(sender.functionName);
    conveyor->setPythonFilePath(sender.moduleName);
    conveyor->setDataX( x );
    conveyor->setDataY( y );

    conveyor->sendArraysToPythonFunction();
    resultModel = conveyor->getResult().toStdString();
}

void RightWidget::integrationSolve( const QString &expression, const double &a, const double &b, Sender &sender )
{
    conveyor->setFunctionName(sender.functionName);
    conveyor->setPythonFilePath(sender.moduleName);

    conveyor->setFunctionToIntegration(expression);
    conveyor->setStartNumToIntegration(a);
    conveyor->setEndNumToIntegration(b);

    conveyor->sendDataToIntegration();
    resultModel = conveyor->getResult().toStdString();
}

void RightWidget::differentiationSolve( const QString& expression, const QVector<double>& x, Sender& sender )
{
    conveyor->setFunctionName(sender.functionName);
    conveyor->setPythonFilePath(sender.moduleName);

    conveyor->setFunctionToDiff(expression);
    conveyor->setDataNums(x.toStdVector());

    conveyor->sendDataToDifferentiation();
    QVector<double> resultX = QVector<double>::fromStdVector(conveyor->get_Nums_Vector());
    QVector<double> resultY = conveyor->getResultVector();
    printGraph( resultX, resultY, sender, nullptr );
}

void RightWidget::clearGraph( void )
{
    graphBuilder->on_clearButton_clicked();
}

void RightWidget::drawGraph( const std::vector<double> x, const std::vector<double> y )
{
    QVector<double> X = QVector<double>::fromStdVector( x );
    QVector<double> Y = QVector<double>::fromStdVector( y );
    graphBuilder->PaintG( X, Y, "График интерполяции", true, false );
}

#include "layoutInitializer.h"
#include <QDebug>

void LayoutInitializer::onInputTextChanged( const QString& text )
{
    widgets->validator->validateExpression( text );
}

void LayoutInitializer::onValidateDataValid( void )
{
    widgets->solve->setEnabled( true );
    widgets->solve->setStyleSheet( "background-color: lightgreen;" );
}

void LayoutInitializer::onValidateDataInvalid( void )
{
    couldBuildTable = false;
    widgets->solve->setEnabled( false );
    widgets->solve->setStyleSheet( "background-color: tomato;" );
}

void LayoutInitializer::acceptArea( const std::string& area )
{
    widgets->area->setText( QString::fromStdString( area ) );
}

void LayoutInitializer::editTable( void )
{
    clearDataTable();
    if( widgets->tableWidget->columnCount() == 3 )
    {
        widgets->tableWidget->setColumnCount( 3 );
    }
    else
    {
        widgets->tableWidget->setColumnCount( 2 );
    }
    widgets->tableWidget->setRowCount( 10 );
    for( int row{}; row < widgets->tableWidget->rowCount(); ++row )
    {
        QRadioButton* button = new QRadioButton;
        widgets->tableWidget->setCellWidget( row, widgets->tableWidget->columnCount(), button );
    }
    widgets->solve->setEnabled( false );
    widgets->solve->setStyleSheet( "background-color: tomato;" );
    widgets->buildGraph->setEnabled( true );
}

void LayoutInitializer::clearDataTable( void )
{
    widgets->tableWidget->clearContents();
    widgets->tableWidget->setRowCount( 0 );
    widgets->equationsTableWidget->clearContents();
    widgets->eqResult->clear();
    widgets->errLabel->clear();
    widgets->X.clear();
    widgets->Y.clear();
    widgets->dY.clear();
    widgets->buildGraph->setEnabled( false );
}

void LayoutInitializer::changeLayer( int index )
{
    if( index == 0 )
    {
        hideFirstLayer();
    }
    else if( index == 1 )
    {
        hideSecondLayer();
    }
}

void LayoutInitializer::hideFirstLayer( void )
{
    widgets->X.clear();

    widgets->nodes->hide();
    widgets->nodesLabel->hide();
    widgets->error->hide();
    widgets->calculatedArea->hide();
    widgets->area->hide();

    widgets->minLabel->show();
    widgets->min->show();
    widgets->maxLabel->show();
    widgets->max->show();
    widgets->stepLabel->show();
    widgets->step->show();
}

void LayoutInitializer::hideSecondLayer( void )
{
    widgets->X.clear();

    widgets->stepLabel->hide();
    widgets->step->hide();
    widgets->error->hide();
    widgets->calculatedArea->hide();
    widgets->area->hide();

    widgets->minLabel->show();
    widgets->min->show();
    widgets->maxLabel->show();
    widgets->max->show();
    widgets->nodesLabel->show();
    widgets->nodes->show();
}

void LayoutInitializer::setupNodes( const double node )
{
    widgets->X.clear();
    if( widgets->tableWidget->columnCount() == 3 )
    {
        widgets->Y.clear();
        MathUtils::setupNodes( widgets->Y, node, widgets->yMin->value(), widgets->yMax->value() );
        MathUtils::setupNodes( widgets->X, node, widgets->min->value(), widgets->max->value() );
        return;
    }
    MathUtils::setupNodes( widgets->X, node, widgets->min->value(), widgets->max->value() );
}

std::vector<double> LayoutInitializer::fillDataFromTable( int column )
{
    std::vector<double> data;
    for( int row{}; row < widgets->tableWidget->rowCount(); ++row )
    {
        QTableWidgetItem* item = widgets->tableWidget->item( row, column );
        if( item != nullptr )
        {
            bool ok;
            double value = item->text().toDouble( &ok );
            if( ok )
            {
                data.push_back( value );
            }
            else
            {
                emit handleParserError( QString::asprintf( "Некорректное значение в таблице. Строка: %d", row+1 ) );
            }
        }
        else
        {
            emit handleParserError( QString::asprintf( "Пустая ячейка в строке: %d", row+1 ) );
        }
    }
    return data;
}

void LayoutInitializer::onSolveButtonClicked( SpecialBuffer& buffer )
{
    if( derivativeLabel && ( widgets->derivativeExpressionInput->text().isEmpty() ||
        widgets->expressionInput->text().isEmpty() ) )
    {
        emit handleParserError( "Заполните все поля" );
        return;
    }

    clearDataTable();

    auto expression = widgets->expressionInput->text();
    auto min        = widgets->min->value();
    auto max        = widgets->max->value();
    auto yMin       = widgets->yMin->value();
    auto yMax       = widgets->yMax->value();
    auto step       = widgets->step->value();

    if( !widgets->nodes->isVisible() && manualInput == false )
    {
        if( widgets->tableWidget->columnCount() == 3 )
        {
            for( double i = yMin; i <= yMax; i += step )
            {
                widgets->Y.push_back( i );
            }
        }
        for( double i = min; i <= max; i += step )
        {
            widgets->X.push_back( i );
        }
        widgets->parser->setDataX( widgets->X );

        if( widgets->tableWidget->columnCount() == 3 )
        {
            widgets->parser->setDataY( widgets->Y );
            widgets->Z = widgets->parser->parseExpression( expression.toStdString().c_str(), 3 );
        }
        else widgets->Y = widgets->parser->parseExpression( expression.toStdString().c_str(), 2 );
    }
    else if( widgets->nodes->isVisible() && manualInput == false )
    {
        setupNodes( widgets->nodes->value() );
        widgets->parser->setDataX( widgets->X );
        if( widgets->tableWidget->columnCount() == 3 )
        {
            widgets->parser->setDataY( widgets->Y );
            widgets->Z = widgets->parser->parseExpression( expression.toStdString().c_str(), 3 );
        }
        else widgets->Y = widgets->parser->parseExpression( expression.toStdString().c_str(), 2 );
    }
    else if( manualInput )
    {
        widgets->X = fillDataFromTable( 0 );
        widgets->Y = fillDataFromTable( 1 );
        widgets->Z = fillDataFromTable( 2 );
    }


    if( widgets->X.size() > limits::SIZE_LIMIT )
    {
        emit handleParserError( "Предел: 1000 узлов" );
        return;
    }

    buffer.x = QVector<double>( widgets->X.begin(), widgets->X.end() );
    buffer.y = QVector<double>( widgets->Y.begin(), widgets->Y.end() );
    buffer.z = QVector<double>( widgets->Z.begin(), widgets->Z.end() );

    if( !widgets->derivativeExpressionInput->text().isEmpty() )
    {
        widgets->dY = widgets->parser->parseExpression( widgets->derivativeExpressionInput->text().toStdString().c_str(), 2 );
        buffer.dy = QVector<double>( widgets->dY.begin(), widgets->dY.end() );
    }

    if( couldBuildTable )
    {
        showTable( widgets->X, widgets->Y, widgets->Z, widgets->dY );
    }
    couldBuildTable = true;
    widgets->X.clear();
    widgets->Y.clear();
    widgets->Z.clear();
    widgets->dY.clear();
}

void LayoutInitializer::updateDataFromTable( SpecialBuffer& buffer )
{
    widgets->X.clear();
    widgets->Y.clear();
    widgets->Z.clear();
    widgets->dY.clear();
    for( int row{}; row < widgets->tableWidget->rowCount(); ++row )
    {
        QTableWidgetItem* itemX = widgets->tableWidget->item( row, 0 );
        QTableWidgetItem* itemY = widgets->tableWidget->item( row, 1 );
        QTableWidgetItem* itemZ = widgets->tableWidget->item( row, 2 );

        if( itemX && itemY )
        {
            widgets->X.push_back( itemX->text().toDouble() );
            widgets->Y.push_back( itemY->text().toDouble() );
        }
        if( itemZ )
        {
            widgets->Z.push_back( itemZ->text().toDouble() );
        }
    }
    buffer.x = QVector<double>( widgets->X.begin(), widgets->X.end() );
    buffer.y = QVector<double>( widgets->Y.begin(), widgets->Y.end() );
    buffer.z = QVector<double>( widgets->Z.begin(), widgets->Z.end() );
}

void LayoutInitializer::showTable( const std::vector<double> x, const std::vector<double> y, const std::vector<double> z, const std::vector<double> dY )
{
    widgets->tableWidget->clear();
    widgets->tableWidget->setRowCount( x.size() );
    QStringList labels;
    labels << "X" << "Y";
    if( !dY.empty() )
    {
        labels << "Y'";
    }
    if( !z.empty() )
    {
        labels << "Z";
    }

    widgets->tableWidget->setHorizontalHeaderLabels( labels );

    for( std::size_t i{}; i < x.size(); ++i )
    {
        QTableWidgetItem* itemX = new QTableWidgetItem( QString::number( x[i] ) );
        QTableWidgetItem* itemY = new QTableWidgetItem( QString::number( y[i] ) );
        widgets->tableWidget->setItem( i, 0, itemX );
        widgets->tableWidget->setItem( i, 1, itemY );

        if( !dY.empty() )
        {
            QTableWidgetItem* itemDY = new QTableWidgetItem( QString::number( dY[i] ) );
            widgets->tableWidget->setItem( i, 2, itemDY );
        }
        if( !z.empty() )
        {
            QTableWidgetItem* itemZ = new QTableWidgetItem( QString::number( z[i] ) );
            widgets->tableWidget->setItem( i, 2, itemZ );
        }
    }
    widgets->buildGraph->setEnabled( true );
}

void LayoutInitializer::handleParserError( const QString& err )
{
    couldBuildTable = false;
    widgets->errLabel->setStyleSheet( "QLabel { color : red; }" );
    widgets->errLabel->setText( err + "!" );
}

void LayoutInitializer::acceptData( const QString& model, const double a, const double b )
{
    auto x = MathUtils::multiplyPoints( a, b );
    widgets->parser->setDataX( x );
    std::vector<double> y = widgets->parser->parseExpression( model.toStdString().c_str(), 2 );
    emit readyToDraw( x, y );
}

void LayoutInitializer::onSolveEquationsButtonClicked( void )
{
    for( int row{}; row < widgets->equationsTableWidget->rowCount(); ++row )
    {
        QTableWidgetItem* firstColumnItem = widgets->equationsTableWidget->item( row, 0 );
        QTableWidgetItem* secondColumnItem = widgets->equationsTableWidget->item( row, 1 );

        if( firstColumnItem == nullptr || secondColumnItem == nullptr )
        {
            qDebug() << "There is some blank labels\n";
            continue;
        }

        QString firstCol = firstColumnItem->text();
        QString secondCol = secondColumnItem->text();

        if( !ExpressionValidator::validateTableRow( firstCol, secondCol ) )
        {
            qDebug() << "ERROR OF TYPING\n";
            return;
        }
    }
    auto data = MathUtils::formTheSystemOfEquations( *widgets->equationsTableWidget );
    emit readyToSendEquationsData( data );
}

void LayoutInitializer::setEquationsResult(const QString &result)
{
    widgets->eqResult->setText( result );
}

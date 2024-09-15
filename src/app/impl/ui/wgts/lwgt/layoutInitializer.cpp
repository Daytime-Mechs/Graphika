#include "layoutInitializer.h"

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
    int columnCount = (widgets->tableWidget->columnCount() == 3) ? 3 : 2;
    int initialRowCount = 10; // начальное количество строк
    int totalRowCount = initialRowCount ; // +1 для строки с кнопками

    widgets->tableWidget->setColumnCount(columnCount);
    widgets->tableWidget->setRowCount(totalRowCount);

    for( int row{}; row < initialRowCount; ++row )
    {
        QRadioButton* button = new QRadioButton;
        widgets->tableWidget->setCellWidget( row, columnCount, button );
    }


    widgets->solve->setEnabled(false);
    widgets->solve->setStyleSheet("background-color: tomato;");
    widgets->buildGraph->setEnabled(true);
    emit tableEdited();
}

void LayoutInitializer::onAddRowButtonClicked()
{
    int rowCount = widgets->tableWidget->rowCount();
    widgets->tableWidget->insertRow(rowCount);
    updateButtonsPosition();
}

void LayoutInitializer::onRemoveRowButtonClicked()
{
    int rowCount = widgets->tableWidget->rowCount();
    if (rowCount > 1) // Не удалять последнюю строку с кнопками
    {
        widgets->tableWidget->removeRow(rowCount - 2);

    }
    updateButtonsPosition();
}

void LayoutInitializer::updateButtonsPosition()
{
    if ( widgets->tableWidget->rowCount() == 0)
    {
        return;
    }

    // Проверяем, существует ли уже buttonsWidget
    if (widgets->buttonsWidget != nullptr)
    {
        widgets->buttonsWidget->hide();
        delete widgets->buttonsWidget;
    }

    widgets->buttonsWidget = new QWidget(widgets->tableWidget->parentWidget());
    QVBoxLayout* buttonsLayout = new QVBoxLayout(widgets->buttonsWidget);

    QPushButton* addButton = new QPushButton();
    addButton->setIcon( QIcon( ":/leftwidget/resources/plus-icon.png" ) );
    addButton->setIconSize( QSize( 30, 30 ) );
    addButton->setFixedSize( 30, 30 );
    addButton->setStyleSheet(
        "QPushButton { border: none; }"
        "QPushButton:pressed { icon: url(:/leftwidget/resources/plus-icon-pressed.png); }"
        );

    QPushButton* removeButton = new QPushButton();
    removeButton->setIcon( QIcon( ":/leftwidget/resources/minus-icon.png" ) );
    removeButton->setIconSize( QSize( 30, 30 ) );
    removeButton->setFixedSize( 30, 30 );
    removeButton->setStyleSheet(
        "QPushButton { border: none; }"
        "QPushButton:pressed { icon: url(:/leftwidget/resources/minus-icon-pressed.png); }"
        );

    buttonsLayout->addWidget(addButton);
    buttonsLayout->addWidget(removeButton);
    buttonsLayout->addStretch();

    widgets->buttonsWidget->setLayout(buttonsLayout);

    QRect tableGeometry = widgets->tableWidget->geometry();
    int columnCount = widgets->tableWidget->columnCount();
    int lastColumnWidth = widgets->tableWidget->columnWidth( columnCount - 1 );

    int headerWidth = widgets->tableWidget->horizontalHeader()->sectionSize( columnCount - 1 );

    int buttonWidgetX = tableGeometry.left() + widgets->tableWidget->verticalHeader()->width()
                        + widgets->tableWidget->columnViewportPosition (columnCount - 1 )
                        + headerWidth + 5;

    // Получаем положение последней строки
    int rowCount = widgets->tableWidget->rowCount();
    int lastRowY = widgets->tableWidget->rowViewportPosition( rowCount - 1 );
    int lastRowHeight = widgets->tableWidget->rowHeight( rowCount - 1 );

    int buttonWidgetY = tableGeometry.top() + lastRowY + ( lastRowHeight / 2 ) - ( widgets->buttonsWidget->height() / 2 );

    widgets->buttonsWidget->setGeometry( buttonWidgetX, buttonWidgetY, widgets->buttonsWidget->sizeHint().width(), widgets->buttonsWidget->sizeHint().height() );

    connect( addButton, &QPushButton::clicked, this, &LayoutInitializer::onAddRowButtonClicked );
    connect( removeButton, &QPushButton::clicked, this, &LayoutInitializer::onRemoveRowButtonClicked );

    widgets->buttonsWidget->show();
}

void LayoutInitializer::clearDataTable( void )
{
    widgets->tableWidget->clearContents();
    widgets->tableWidget->setRowCount( 0 );
    widgets->equationsTableWidget->clearContents();
    widgets->eqResult->clear();
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
    if ( derivativeLabel && ( widgets->derivativeExpressionInput->text().isEmpty() || widgets->expressionInput->text().isEmpty() ) )
    {
        emit handleParserError( "Заполните все поля" );
        return;
    }

    clearDataTable();

    auto expression = widgets->expressionInput->text();
    auto min = widgets->min->value();
    auto max = widgets->max->value();
    auto yMin = widgets->yMin->value();
    auto yMax = widgets->yMax->value();
    auto step = widgets->step->value();

    bool is3D = widgets->tableWidget->columnCount() > 2 && widgets->tableWidget->horizontalHeaderItem( 2 ) && widgets->tableWidget->horizontalHeaderItem( 2 )->text() == "Z";
    bool isDifferentiation = widgets->tableWidget->columnCount() > 2 && widgets->tableWidget->horizontalHeaderItem( 2 ) && widgets->tableWidget->horizontalHeaderItem( 2 )->text() == "Y'";

    if ( !widgets->nodes->isVisible() && manualInput == false )
    {
        for ( double i = min; i <= max; i += step )
        {
            widgets->X.push_back( i );
        }
        widgets->parser->setDataX( widgets->X );

        if ( is3D )
        {
            for ( double i = yMin; i <= yMax; i += step )
            {
                widgets->Y.push_back( i );
            }
            widgets->parser->setDataY( widgets->Y );
            widgets->Z = widgets->parser->parseExpression( expression.toStdString().c_str(), 3 );
        }
        else
        {
            widgets->Y = widgets->parser->parseExpression( expression.toStdString().c_str(), 2 );
        }
    }
    else if ( widgets->nodes->isVisible() && manualInput == false )
    {
        setupNodes( widgets->nodes->value() );
        widgets->parser->setDataX( widgets->X );

        if ( is3D )
        {
            widgets->parser->setDataY( widgets->Y );
            widgets->Z = widgets->parser->parseExpression( expression.toStdString().c_str(), 3 );
        }
        else
        {
            widgets->Y = widgets->parser->parseExpression( expression.toStdString().c_str(), 2 );
        }
    }
    else if ( manualInput )
    {
        widgets->X = fillDataFromTable( 0 );
        widgets->Y = fillDataFromTable( 1 );
        if ( is3D )
        {
            widgets->Z = fillDataFromTable( 2 );
        }
        else if ( isDifferentiation )
        {
            widgets->dY = fillDataFromTable( 2 );
        }
    }

    if ( widgets->X.size() > limits::SIZE_LIMIT )
    {
        emit handleParserError( "Предел: 1000 узлов" );
        return;
    }

    buffer.x = QVector<double>( widgets->X.begin(), widgets->X.end() );
    buffer.y = QVector<double>( widgets->Y.begin(), widgets->Y.end() );

    if ( is3D )
    {
        buffer.z = QVector<double>( widgets->Z.begin(), widgets->Z.end() );
    }
    else if ( isDifferentiation )
    {
        buffer.dy = QVector<double>( widgets->dY.begin(), widgets->dY.end() );
    }

    if ( !widgets->derivativeExpressionInput->text().isEmpty() )
    {
        widgets->dY = widgets->parser->parseExpression( widgets->derivativeExpressionInput->text().toStdString().c_str(), 2 );
        buffer.dy = QVector<double>( widgets->dY.begin(), widgets->dY.end() );
    }

    if ( couldBuildTable )
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
    widgets->errLabel->setPopupText( err + "!" );
    widgets->errLabel->show();
}

void LayoutInitializer::acceptData( const QString& model, const double a, const double b )
{
    auto x = MathUtils::multiplyPoints( a, b );
    widgets->parser->setDataX( x );
    std::vector<double> y = widgets->parser->parseExpression( model.toStdString().c_str(), 2 );
    widgets->model->setText( model );
    emit readyToDraw( x, y );
}

std::string solveEquation(const std::string& equation) {
    size_t equalPos = equation.find('=');

    std::string expression = equation.substr(0, equalPos);
    std::string constStr = equation.substr(equalPos + 1);
    char* endPtr;
    double constant = std::strtod(constStr.c_str(), &endPtr);
    if (endPtr == constStr.c_str()) {
        return equation;
    }
    char sign = constant >= 0 ? '+' : '-';
    sign = sign == '+' ? '-' : '+';
    std::string result = expression + sign + " " + std::to_string(std::abs(constant));

    return result;
}

template<typename T>
std::size_t LayoutInitializer::findElementIndex(const std::vector<T>& vec, const T& element)
{
    auto it = std::find(vec.begin(), vec.end(), element);
    if (it == vec.end()) {
        return std::numeric_limits<std::size_t>::max();
    }
    return static_cast<std::size_t>(std::distance(vec.begin(), it));
}

bool LayoutInitializer::containsCommonElements(const std::vector<std::vector<double>>& data, const std::vector<double>& X, std::vector<double>& res)
{
    if (data.empty() || data[0].empty() || X.empty())
    {
        return false;
    }

    std::unordered_map<double, std::vector<std::pair<std::size_t, std::vector<std::size_t>>>> indexMap;

    for (std::size_t i = 0; i < X.size(); ++i)
    {
        for (std::size_t j = 0; j < data.size(); ++j)
        {
            std::size_t dataIndex = findElementIndex(data[j], X[i]);
            if (dataIndex != std::numeric_limits<std::size_t>::max())
            {
                indexMap[X[i]].emplace_back(i, std::vector<std::size_t>{dataIndex});
            }
        }
    }

    for (const auto& pair : indexMap)
    {
        if (pair.second.size() == data.size())
        {
            bool allEqual = true;
            for (std::size_t i = 1; i < pair.second.size(); ++i)
            {
                if (pair.second[i].second != pair.second[0].second)
                {
                    allEqual = false;
                    break;
                }
            }
            if (allEqual)
            {
                res.push_back(pair.first);
            }
        }
    }

    return !res.empty();
}



void LayoutInitializer::onSolveEquationsButtonClicked( void )
{
    int rowCount = widgets->equationsTableWidget->rowCount();
    if( !widgets->nonLinear )
    {
        for( int row{}; row < rowCount; ++row )
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
    else
    {
        std::vector<QString> equations;
        for( int row = 0; row < rowCount; ++row )
        {
            if( !widgets->tableWidget->rowAt( row ) ) break;
            QTableWidgetItem* leftItem = widgets->equationsTableWidget->item( row, 0 );
            QTableWidgetItem* rightItem = widgets->equationsTableWidget->item( row, 1 );

            if( leftItem && rightItem )
            {
                QString leftPart = leftItem->text();
                QString rightPart = rightItem->text();

                equations.push_back( (solveEquation(leftPart.toStdString() + " = " + rightPart.toStdString())).c_str() );
            }
        }

        StringParser parser;
        std::vector<std::vector<double>> roots;
        std::vector<std::vector<double>> allRoots;
        std::vector<double> xArr;

        for (double i = widgets->nonLinearXMin->value(); i <= widgets->nonLinearXMax->value(); i += widgets->nonLinearStep->value())
            xArr.emplace_back(i);


        for(const auto& equation : equations)
        {
            QString eq(solveEquation(equation.toStdString()).c_str());

            parser.setDataX(xArr);
            std::vector<double> yArr = parser.parseExpression(equation, 2);
            QVector<double> _x = QVector<double>(xArr.begin(), xArr.end());
            QVector<double> _y = QVector<double>(yArr.begin(), yArr.end());
            emit readyToDrawGraphsFromSys(_x, _y);
            allRoots.push_back(yArr);
        }

        std::vector<double> res;
        if(allRoots.size() > 1 && containsCommonElements(allRoots, xArr, res))
        {
            std::stringstream ss;
            for (size_t i = 0; i < res.size(); ++i) {
                if (i > 0) {
                    ss << ", ";
                }
                ss << res[i];
            }
            setEquationsResult(QString::fromStdString(ss.str()));
        }
        else
        {
            setEquationsResult("Решение не найдено.");
        }
    }
}

void LayoutInitializer::setEquationsResult( const QString &result )
{
    widgets->eqResult->setText( result );
}

void LayoutInitializer::clearTableButtons()
{
    if (widgets->buttonsWidget != nullptr)
    {
        widgets->buttonsWidget->hide();
        delete widgets->buttonsWidget;
        widgets->buttonsWidget = nullptr;
    }
}

void LayoutInitializer::hideButtonsWidget()
{
    if (widgets->buttonsWidget != nullptr)
    {
        widgets->buttonsWidget->hide();
    }
}

void LayoutInitializer::containsNonLinearData( const bool& nl )
{
    widgets->nonLinear = nl;
    if(widgets->nonLinear)
    {
        widgets->oddsInputLabel->setText( QString::asprintf( "Вы находитесь в режиме поиска корней в системе нелинейных уравнений,\nгде слева: f(x), а справа константа.\nВ левую часть таблицы вводите мат. выражения вида f(x), а слева - целочисленную постоянную.\nПарсер математического выражения преобразует входные\nданные следующим образом: f(x) +/- C = 0 и найдет корни.\nПРИМЕЧАНИЕ: Для корректного вычисления корней - выбирайте четный шаг!" ) );
    }
    else widgets->oddsInputLabel->setText( QString::asprintf( "Введите через пробел коэффициенты \nлинейных уравнений и свободный член" ) );
}

void LayoutInitializer::calculateDiffError(const QVector<double> &y1, const QVector<double> &y2)
{
    widgets->error->clear();
    widgets->error->insert( QString::asprintf( "%lf", MathUtils::calculateAverageError( y1, y2 ) ) );
}

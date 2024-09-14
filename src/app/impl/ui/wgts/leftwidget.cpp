#include "leftwidget.h"
#include <QLineEdit>
#include <QString>
#include <iostream>

LeftWidget::LeftWidget( QWidget *parent ) : QWidget( parent )
{
    layout = new QGridLayout( this );
    widgets = new Widgets( this );
    functionLayout = new FunctionLayout;
    derivationLayout = new DerivationLayout;
    integrationLayout = new IntegrationLayout;
    equationsLayout = new EquationsLayout;
    polynomialsLayout = new PolynomialsLayout;
}

void LeftWidget::initLayout( SpecialBuffer& buffer, pymodules::Modules module )
{
    if( module != pymodules::Modules::NIL )
    {
        emit switchToGraphBuilder();
    }

    hideButtons();

    switch( module )
    {
    case pymodules::Modules::NIL:
        currentLayout = functionLayout;
        break;
    case pymodules::Modules::DIFFERENTIATION:
        currentLayout = derivationLayout;
        break;
    case pymodules::Modules::INTEGRATION:
        currentLayout = integrationLayout;
        break;
    case pymodules::Modules::EQUATIONS:
        currentLayout = equationsLayout;
        break;
    case pymodules::Modules::POLYNOMIALS:
        currentLayout = polynomialsLayout;
        break;
    }

    hideAllWidgets( layout );
    widgets->initWidgets();
    currentLayout->generateWidgets( *widgets );
    connectLabels( buffer );
    layout->addLayout( currentLayout->get(), 0, 0 );
    applyStoredSettings();
}


void LeftWidget::hideAllWidgets( QLayout* layout )
{
    if ( !layout )
    {
        return;
    }

    if ( QGridLayout* gridLayout = qobject_cast<QGridLayout*>( layout ) )
    {
        for( int i = 0; i < gridLayout->count(); ++i )
        {
            QLayoutItem* item = gridLayout->itemAt( i );
            if ( item->widget() )
            {
                item->widget()->hide();
            }
            else if ( item->layout() )
            {
                hideAllWidgets( item->layout() );
            }
        }
    }
}

void LeftWidget::connectLabels( SpecialBuffer& buffer )
{
    connect( currentLayout->widgets->validator, &ValidateString::validExpression, currentLayout, &LayoutInitializer::onValidateDataValid );
    connect( currentLayout->widgets->validator, &ValidateString::invalidExpression, currentLayout, &LayoutInitializer::onValidateDataInvalid );
    connect( currentLayout->widgets->expressionInput, &QLineEdit::textChanged, currentLayout, &LayoutInitializer::onInputTextChanged );
    connect( currentLayout->widgets->derivativeExpressionInput, &QLineEdit::textChanged, currentLayout, &LayoutInitializer::onInputTextChanged );
    connect( currentLayout->widgets->parser, &StringParser::errorOccurred, currentLayout, &LayoutInitializer::handleParserError );
    connect( currentLayout->widgets->manualTableInput, &QPushButton::clicked, currentLayout, &LayoutInitializer::editTable );
    connect( currentLayout->widgets->clearTable, &QPushButton::clicked, currentLayout, &LayoutInitializer::clearDataTable );
    connect( currentLayout->widgets->typeOfVariableInput, QOverload<int>::of( &QComboBox::currentIndexChanged ), currentLayout, &LayoutInitializer::changeLayer );
    connect( currentLayout->widgets->solve, &QPushButton::clicked, [ &buffer, this ]()
            {
                currentLayout->onSolveButtonClicked( buffer );
            }
    );
    connect( currentLayout->widgets->solveEquations, &QPushButton::clicked, currentLayout, &LayoutInitializer::onSolveEquationsButtonClicked );
    connect( currentLayout->widgets->clearEquationsTable, &QPushButton::clicked, currentLayout, &LayoutInitializer::clearDataTable );
    connect( currentLayout->widgets->tableWidget, &QTableWidget::itemChanged, this, [ &buffer, this ]()
            {
                currentLayout->updateDataFromTable( buffer );
            }
    );

    connect( functionLayout, QOverload<const int&>::of( &FunctionLayout::switchPlots ), this, [ this ]( const int& index )
    {
            if( index == 0 )
            {
                emit switchToGraphBuilder();
                return;
            }
            emit switchToGL3DGraphBuilder();
    });

    connect( currentLayout->widgets->expressionInput, &QLineEdit::textChanged, this, [this](const QString& text )
    {
        if( text.length() > 30 )
        {
            emit functionTextChanged( "График заданной функции" );
            return;
        }
        emit functionTextChanged( "f(x) = " + text );
    });
    connect( currentLayout, &LayoutInitializer::tableEdited, this, &LeftWidget::onTableEdited );
    connect( equationsLayout, &EquationsLayout::equationsTableEdited, this, &LeftWidget::onEquationsTableEdited);

    connect( currentLayout->widgets->clearTable, &QPushButton::clicked, currentLayout, &LayoutInitializer::clearTableButtons );
    connect( functionLayout, &FunctionLayout::switchPlots, currentLayout, &LayoutInitializer::clearDataTable );
    connect( functionLayout, &FunctionLayout::switchPlots, currentLayout, &LayoutInitializer::clearTableButtons );
    connect( currentLayout->widgets->solve, &QPushButton::clicked, currentLayout, &LayoutInitializer::clearTableButtons );
    connect( currentLayout, &LayoutInitializer::readyToSendNonLinearSys, this, [this]( const QString& sysText )
        {
            emit sendNonLinearSys( sysText );
        }
    );
    connect( currentLayout, &LayoutInitializer::readyToDrawGraphsFromSys, this, [this](const QVector<double>& x, const QVector<double>& y)
    {
        emit acceptXYData( x, y );
    });
}

void LeftWidget::applyProgrammerSettings(double min, double Ymin, double max, double Ymax, double minStep, double maxStep, double minNodes, double maxNodes, int decimals)
{
    qDebug() << "Hello from leftWidget after programmer";

    programmerSetting.min = min;
    programmerSetting.max = max;
    programmerSetting.yMin = Ymin;
    programmerSetting.yMax = Ymax;
    programmerSetting.minStep = minStep;
    programmerSetting.maxStep = maxStep;
    programmerSetting.minNodes = minNodes;
    programmerSetting.maxNodes = maxNodes;
    programmerSetting.decimals = decimals;

    applyStoredSettings();
}

void LeftWidget::applyStoredSettings( void )
{
    widgets->step->setDecimals(programmerSetting.decimals);
    widgets->step->setRange(programmerSetting.minStep, programmerSetting.maxStep);
    widgets->min->setRange(programmerSetting.min, programmerSetting.max);
    widgets->max->setRange(programmerSetting.min, programmerSetting.max);
    widgets->yMin->setRange(programmerSetting.yMin, programmerSetting.yMax);
    widgets->yMax->setRange(programmerSetting.yMin, programmerSetting.yMax);
    widgets->nodes->setRange(programmerSetting.minNodes, programmerSetting.maxNodes);
}

void LeftWidget::onTableEdited()
{
    if (currentLayout == polynomialsLayout)
    {
        return;
    }
    connect( currentLayout->widgets->tableWidget->horizontalHeader(), &QHeaderView::sectionResized, currentLayout, &LayoutInitializer::updateButtonsPosition );
    connect( currentLayout->widgets->tableWidget->verticalHeader(), &QHeaderView::sectionResized, currentLayout, &LayoutInitializer::updateButtonsPosition );
    QTimer::singleShot(2, currentLayout, &LayoutInitializer::updateButtonsPosition);
    currentLayout->updateButtonsPosition();
}

void LeftWidget::onEquationsTableEdited()
{

    if (currentLayout != equationsLayout)
    {
        return;
    }
    connect( equationsLayout->widgets->equationsTableWidget->horizontalHeader(), &QHeaderView::sectionResized, equationsLayout, &EquationsLayout::updateEquationsButtonsPosition );
    connect( equationsLayout->widgets->equationsTableWidget->verticalHeader(), &QHeaderView::sectionResized, equationsLayout, &EquationsLayout::updateEquationsButtonsPosition );
    QTimer::singleShot(2, equationsLayout, &EquationsLayout::updateEquationsButtonsPosition );
    equationsLayout->updateEquationsButtonsPosition();

}

void LeftWidget::acceptXYData(const QVector<double> &x, const QVector<double> &y)
{
    emit buildFuncGraph(x, y);
}

void LeftWidget::hideButtons()
{
    if ( currentLayout != nullptr )
    {
        currentLayout->hideButtonsWidget();
        if (currentLayout == equationsLayout) {
            equationsLayout->hideEquationsButtonsWidget();
        }
    }
}

void LeftWidget::setNonLinearFlag( bool flag )
{
    equationsLayout->setNonLinearFlag( flag );
}

void LeftWidget::updateNonLinearSpinBoxes()
{
    if (currentLayout == equationsLayout)
    {
        equationsLayout->updateNonLinearSpinBoxes();
    }
}

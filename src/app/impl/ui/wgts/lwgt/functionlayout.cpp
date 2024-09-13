#include "functionlayout.h"

void FunctionLayout::generateWidgets( Widgets& widgets )
{
    layout = new QGridLayout;
    this->widgets = &widgets;
    cb = new QComboBox( this );
    cb->addItems( { "f(x) = ", "f(x, y) = " } );

    connect(cb, QOverload<int>::of( &QComboBox::currentIndexChanged ),
            [this]() {
        checkoutDimensionals();
    });

    delete widgets.label;

    layout->addWidget( cb, 0, 0 );
    layout->addWidget( widgets.expressionInput, 0, 1, 1, 10 );
    layout->addWidget( widgets.derivativeLabel, layout->rowCount() - layout->rowCount() + 1, 0 );
    layout->addWidget( widgets.derivativeExpressionInput, layout->rowCount() - layout->rowCount() + 1, 1, 1, 10 );
    //layout->addWidget( widgets.errLabel, 2, 0 );
    layout->addWidget( widgets.typeOfInput, 3, 0 );
    layout->addWidget( widgets.typeOfVariableInput, 3, 1 );
    layout->addWidget( widgets.minLabel, 4, 0 );
    layout->addWidget( widgets.min, 4, 1 );
    layout->addWidget( widgets.maxLabel, 5, 0 );
    layout->addWidget( widgets.max, 5, 1 );
    layout->addWidget( widgets.yMinLabel, 6, 0 );
    layout->addWidget( widgets.yMin, 6, 1 );
    layout->addWidget( widgets.yMaxLabel, 7, 0 );
    layout->addWidget( widgets.yMax, 7, 1 );
    layout->addWidget( widgets.stepLabel, 8, 0 );
    layout->addWidget( widgets.step, 8, 1 );
    layout->addWidget( widgets.nodesLabel, 9, 0 );
    layout->addWidget( widgets.nodes, 9, 1 );
    layout->addWidget( widgets.solve, 10, 0 );
    layout->addWidget( widgets.clearTable, 10, 1 );
    layout->addWidget( widgets.buildGraph, 11, 0 );
    layout->addWidget( widgets.manualTableInput, 11, 1 );
    layout->addWidget( widgets.tableWidget, 12, 0, 1, 2 );
    layout->addWidget( widgets.averError, layout->rowCount(), 0 );
    layout->addWidget( widgets.error = new QLineEdit( this ), layout->rowCount() - 1, 1 );
    layout->addWidget( widgets.calculatedArea = new QLabel( "Вычисленная площадь: ", this ),
                      layout->rowCount(), 0 );
    layout->addWidget( widgets.area = new QLineEdit( this ), layout->rowCount() - 1, 1 );
    layout->addWidget( widgets.modelLabel, layout->rowCount(), 0 );
    layout->addWidget( widgets.model, layout->rowCount() - 1, 1, 1, 10 );

    layout->setColumnStretch( 1, 10 );
    layout->setColumnStretch( 0, 2 );

    this->hideWidgets( widgets.derivativeExpressionInput, widgets.derivativeLabel,
                      widgets.error, widgets.averError, widgets.area,
                      widgets.calculatedArea, widgets.modelLabel, widgets.model  );

    hideFirstLayer();
    hideThirdDimensionalLayer();
}

QGridLayout *FunctionLayout::get()
{
    return layout;
}

void FunctionLayout::hideThirdDimensionalLayer( void )
{
    layout->itemAtPosition( 6, 0 )->widget()->hide();
    layout->itemAtPosition( 6, 1 )->widget()->hide();
    layout->itemAtPosition( 7, 0 )->widget()->hide();
    layout->itemAtPosition( 7, 1 )->widget()->hide();

    if( widgets->tableWidget->columnCount() == 3 )
    {
        widgets->tableWidget->removeColumn( 2 );
    }
}

void FunctionLayout::showThirdDimensionalLayer( void )
{
    layout->itemAtPosition( 6, 0 )->widget()->show();
    layout->itemAtPosition( 6, 1 )->widget()->show();
    layout->itemAtPosition( 7, 0 )->widget()->show();
    layout->itemAtPosition( 7, 1 )->widget()->show();

    int currentColumnCount = widgets->tableWidget->columnCount();
    widgets->tableWidget->setColumnCount( currentColumnCount + 1 );
    auto item = new QTableWidgetItem( "Z" );
    widgets->tableWidget->setHorizontalHeaderItem( currentColumnCount, item );
}

void FunctionLayout::checkoutDimensionals( void )
{
    if( cb->currentIndex() == 0 )
    {
        hideThirdDimensionalLayer();
        emit switchPlots( cb->currentIndex() );
        return;
    }
    showThirdDimensionalLayer();
    emit switchPlots( cb->currentIndex() );
}

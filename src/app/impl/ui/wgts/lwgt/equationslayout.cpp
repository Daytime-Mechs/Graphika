#include "equationslayout.h"

void EquationsLayout::generateWidgets( Widgets& widgets )
{
    layout = new QGridLayout;
    widgets.oddsInputLabel->setText( QString::asprintf( "Введите через пробел коэффициенты \n линейных уравнений и свободный член" ) );
    QStringList labels;
    labels << "C1 C2 ... Cn" << "X0";
    widgets.equationsTableWidget->setColumnCount( 2 );
    widgets.equationsTableWidget->setRowCount( 10 );
    for( int row{}; row < widgets.equationsTableWidget->rowCount(); ++row )
    {
        QRadioButton* button = new QRadioButton;
        widgets.equationsTableWidget->setCellWidget( row, 2, button );
    }
    widgets.equationsTableWidget->setHorizontalHeaderLabels( labels );
    widgets.solveEquations->setText( "Решить" );
    widgets.clearEquationsTable->setText( "Очистить Таблицу" );
    widgets.resultOfEquations->setText( "Результат вычислений: " );
    widgets.resultDescription->setText( "Описание: " );

    if (nonLinearFlag)
    {
        layout->addWidget( widgets.oddsInputLabel, 0, 0 );
        layout->addWidget( widgets.equationsTableWidget, 1, 0, 1, 2 );
        layout->addWidget( widgets.nonLinearXMinLabel, 2, 0);
        layout->addWidget( widgets.nonLinearXMin, 2, 1 );
        layout->addWidget( widgets.nonLinearXMaxLabel, 3, 0);
        layout->addWidget( widgets.nonLinearXMax, 3, 1 );
        layout->addWidget( widgets.nonLinearStepLabel, 4, 0);
        layout->addWidget( widgets.nonLinearStep, 4, 1 );
        layout->addWidget( widgets.solveEquations, 5, 0 );
        layout->addWidget( widgets.clearEquationsTable, 5, 1 );
        layout->addWidget( widgets.resultOfEquations, 6, 0 );
        layout->addWidget( widgets.eqResult, 7, 0 );
        layout->addWidget( widgets.resultDescription, 8, 0 );
        layout->addWidget( widgets.description, 9, 0 );

        // Установить минимальную ширину для столбца 0
        layout->setColumnMinimumWidth(0, 100);

        // Установить растяжение для столбцов
        layout->setColumnStretch(0, 1);
        layout->setColumnStretch(1, 2);
    } else {
        layout->addWidget( widgets.oddsInputLabel, 0, 0 );
        layout->addWidget( widgets.equationsTableWidget, 1, 0, 1, 2 );
        layout->addWidget( widgets.solveEquations, 5, 0 );
        layout->addWidget( widgets.clearEquationsTable, 5, 1 );
        layout->addWidget( widgets.resultOfEquations, 6, 0 );
        layout->addWidget( widgets.eqResult, 7, 0 );
        layout->addWidget( widgets.resultDescription, 8, 0 );
        layout->addWidget( widgets.description, 9, 0 );
    }

    // ВРЕМЕННО ЗАКРОЕМ ДОСТУП К ДАННЫМ ПУНКТАМ
    {
        widgets.resultDescription->setDisabled( true );
        widgets.description->setDisabled( true );
    }
    this->widgets = &widgets;
    emit equationsTableEdited();
}

void EquationsLayout::updateEquationsButtonsPosition()
{
    if ( widgets->equationsTableWidget->rowCount() == 0)
    {
        return;
    }

    // Проверяем, существует ли уже equationsButtonsWidget
    if (widgets->equationsButtonsWidget != nullptr)
    {
        widgets->equationsButtonsWidget->hide();
        delete widgets->equationsButtonsWidget;
    }

    widgets->equationsButtonsWidget = new QWidget(widgets->equationsTableWidget->parentWidget());
    QVBoxLayout* buttonsLayout = new QVBoxLayout(widgets->equationsButtonsWidget);

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

    widgets->equationsButtonsWidget->setLayout(buttonsLayout);

    QRect tableGeometry = widgets->equationsTableWidget->geometry();
    int columnCount = widgets->equationsTableWidget->columnCount();
    int lastColumnWidth = widgets->equationsTableWidget->columnWidth(columnCount - 1);

    // Получаем размер крайнего правого заголовка колонки
    int headerWidth = widgets->equationsTableWidget->horizontalHeader()->sectionSize(columnCount - 1);

    int buttonWidgetX = tableGeometry.left() + widgets->equationsTableWidget->verticalHeader()->width()
                        + widgets->equationsTableWidget->columnViewportPosition(columnCount - 1)
                        + headerWidth + 5; // +5 для небольшого отступа

    int rowCount = widgets->equationsTableWidget->rowCount();
    int lastRowY = widgets->equationsTableWidget->rowViewportPosition(rowCount - 1);
    int lastRowHeight = widgets->equationsTableWidget->rowHeight(rowCount - 1);

    int buttonWidgetY = tableGeometry.top() + lastRowY + (lastRowHeight / 2) - (widgets->equationsButtonsWidget->height() / 2);

    widgets->equationsButtonsWidget->setGeometry(buttonWidgetX, buttonWidgetY, widgets->equationsButtonsWidget->sizeHint().width(), widgets->equationsButtonsWidget->sizeHint().height());

    connect(addButton, &QPushButton::clicked, this, &EquationsLayout::onAddEquationRowButtonClicked);
    connect(removeButton, &QPushButton::clicked, this, &EquationsLayout::onRemoveEquationRowButtonClicked);

    widgets->equationsButtonsWidget->show();
}

void EquationsLayout::onAddEquationRowButtonClicked()
{
    int rowCount = widgets->equationsTableWidget->rowCount();
    widgets->equationsTableWidget->insertRow(rowCount);
    updateEquationsButtonsPosition();
}

void EquationsLayout::onRemoveEquationRowButtonClicked()
{
    int rowCount = widgets->equationsTableWidget->rowCount();
    if (rowCount > 1) // Не удалять последнюю строку
    {
        widgets->equationsTableWidget->removeRow(rowCount - 2);
    }
    updateEquationsButtonsPosition();
}

void EquationsLayout::hideEquationsButtonsWidget()
{
    if (widgets->equationsButtonsWidget != nullptr)
    {
        widgets->equationsButtonsWidget->hide();
    }
}

QGridLayout *EquationsLayout::get()
{
    return layout;
}

void EquationsLayout::setNonLinearFlag( bool flag )
{
    nonLinearFlag = flag;
}

void EquationsLayout::updateNonLinearSpinBoxes()
{
    if (nonLinearFlag)
    {
        if (!layout->indexOf(widgets->nonLinearXMin) != -1)
        {
            layout->addWidget( widgets->nonLinearXMinLabel, 2, 0);
            layout->addWidget( widgets->nonLinearXMin, 2, 1);
            layout->addWidget( widgets->nonLinearXMaxLabel, 3, 0);
            layout->addWidget( widgets->nonLinearXMax, 3, 1 );
            layout->addWidget( widgets->nonLinearStepLabel, 4, 0);
            layout->addWidget( widgets->nonLinearStep, 4, 1 );

            // Установить минимальную ширину для столбца 0
            layout->setColumnMinimumWidth(0, 100);

            // Установить растяжение для столбцов
            layout->setColumnStretch(0, 1);
            layout->setColumnStretch(1, 2);
        }
        widgets->nonLinearXMinLabel->show();
        widgets->nonLinearXMin->show();
        widgets->nonLinearXMaxLabel->show();
        widgets->nonLinearXMax->show();
        widgets->nonLinearStepLabel->show();
        widgets->nonLinearStep->show();
    }
    else
    {
        if (layout->indexOf(widgets->nonLinearXMin) != -1)
        {
            widgets->nonLinearXMinLabel->hide();
            widgets->nonLinearXMin->hide();
            widgets->nonLinearXMaxLabel->hide();
            widgets->nonLinearXMax->hide();
            widgets->nonLinearStepLabel->hide();
            widgets->nonLinearStep->hide();

            // Сбросить минимальную ширину и растяжение для столбцов
            layout->setColumnMinimumWidth(0, 0);
            layout->setColumnStretch(0, 0);
            layout->setColumnStretch(1, 0);
        }
    }
}

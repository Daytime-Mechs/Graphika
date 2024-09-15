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
    layout->addWidget( widgets.oddsInputLabel, 0, 0 );
    layout->addWidget( widgets.equationsTableWidget, 1, 0, 1, 2 );

    if (nonLinearFlag)
    {
        widgets.oddsInputLabel->setText(QString::asprintf("Вы находитесь в режиме поиска корней в системе нелинейных уравнений,\nгде слева: f(x), а справа константа.\nВ левую часть таблицы вводите мат. выражения вида f(x), а слева - целочисленную постоянную.\nПарсер математического выражения преобразует входные\nданные следующим образом: f(x) +/- C = 0 и найдет корни."));

        widgets.nonLinearXMinLayout->addWidget(widgets.nonLinearXMinLabel);
        widgets.nonLinearXMinLayout->addWidget(widgets.nonLinearXMin);
        widgets.nonLinearXMinLayout->setStretch(0, 1);
        widgets.nonLinearXMinLayout->setStretch(1, 2);
        widgets.nonLinearXMaxLayout->addWidget(widgets.nonLinearXMaxLabel);
        widgets.nonLinearXMaxLayout->addWidget(widgets.nonLinearXMax);
        widgets.nonLinearXMaxLayout->setStretch(0, 1);
        widgets.nonLinearXMaxLayout->setStretch(1, 2);
        widgets.nonLinearStepLayout->addWidget(widgets.nonLinearStepLabel);
        widgets.nonLinearStepLayout->addWidget(widgets.nonLinearStep);
        widgets.nonLinearStepLayout->setStretch(0, 1);
        widgets.nonLinearStepLayout->setStretch(1, 2);
        layout->addWidget(widgets.nonLinearXMinContainer, 2, 0, 1, 2);
        layout->addWidget(widgets.nonLinearXMaxContainer, 3, 0, 1, 2);
        layout->addWidget(widgets.nonLinearStepContainer, 4, 0, 1, 2);
    }

    layout->addWidget( widgets.solveEquations, 5, 0 );
    layout->addWidget( widgets.clearEquationsTable, 5, 1 );
    layout->addWidget( widgets.resultOfEquations, 6, 0 );
    layout->addWidget( widgets.eqResult, 7, 0 );
    layout->addWidget( widgets.resultDescription, 8, 0 );
    layout->addWidget( widgets.description, 9, 0 );

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
            widgets->nonLinearXMinLayout->addWidget(widgets->nonLinearXMinLabel);
            widgets->nonLinearXMinLayout->addWidget(widgets->nonLinearXMin);
            widgets->nonLinearXMinLayout->setStretch(0, 1);
            widgets->nonLinearXMinLayout->setStretch(1, 2);
            widgets->nonLinearXMaxLayout->addWidget(widgets->nonLinearXMaxLabel);
            widgets->nonLinearXMaxLayout->addWidget(widgets->nonLinearXMax);
            widgets->nonLinearXMaxLayout->setStretch(0, 1);
            widgets->nonLinearXMaxLayout->setStretch(1, 2);
            widgets->nonLinearStepLayout->addWidget(widgets->nonLinearStepLabel);
            widgets->nonLinearStepLayout->addWidget(widgets->nonLinearStep);
            widgets->nonLinearStepLayout->setStretch(0, 1);
            widgets->nonLinearStepLayout->setStretch(1, 2);
            layout->addWidget(widgets->nonLinearXMinContainer, 2, 0, 1, 2);
            layout->addWidget(widgets->nonLinearXMaxContainer, 3, 0, 1, 2);
            layout->addWidget(widgets->nonLinearStepContainer, 4, 0, 1, 2);
        }
        widgets->nonLinearStepContainer->show();
        widgets->nonLinearXMaxContainer->show();
        widgets->nonLinearXMinContainer->show();
    }
    else
    {
        widgets->nonLinearStepContainer->hide();
        widgets->nonLinearXMaxContainer->hide();
        widgets->nonLinearXMinContainer->hide();
    }
}

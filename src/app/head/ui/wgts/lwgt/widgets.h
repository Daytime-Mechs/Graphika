/*!
 * \attention In development.
 *
 * \author Malaniya Mark Timurovich, Dnevnie Mechaniki.
 *
 * \date last update: 21.05.2024.
 */

#ifndef WIDGETS_H
#define WIDGETS_H

#include "validatestring.h"
#include "errorlabel.h"
#include <QLineEdit>
#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QTableWidget>
#include <QLineEdit>
#include <QFont>
#include <stringparser.h>

/*!
 * \clas Widgets
 *
 * \brief The Widgets class which contains all labels and other left-widget control systems.
 */
class Widgets : public QWidget
{
    Q_OBJECT
public:
    ValidateString* validator;               ///< entered expression validator.
    StringParser* parser;                    ///< expression validator-calculator.

    QLabel* label;                           ///< Label for displaying information
    ErrorLabel* errLabel;
    QLabel* xIs;                             ///< Label for displaying the variable 'x'
    QSpinBox* xVariables;                    ///< Spin box for selecting the number of 'x' variables
    QLabel* typeOfInput;                     ///< Label for displaying the type of input
    QComboBox* typeOfVariableInput;          ///< Combo box for selecting the type of variable input
    QPushButton* clearTable;                 ///< Button for clearing the table
    QLabel* minLabel;                        ///< Label for displaying the minimum value
    QLabel* maxLabel;                        ///< Label for displaying the maximum value
    QLabel* stepLabel;                       ///< Label for displaying the step value
    QLabel* nodesLabel;                      ///< Label for displaying the number of nodes
    QDoubleSpinBox* nodes;                   ///< Spin box for setting the number of nodes
    QDoubleSpinBox* min;                     ///< Spin box for setting the minimum value
    QDoubleSpinBox* max;                     ///< Spin box for setting the maximum value
    QDoubleSpinBox* step;                    ///< Spin box for setting the step value
    QLabel* yMinLabel;
    QLabel* yMaxLabel;
    QDoubleSpinBox* yMin;
    QDoubleSpinBox* yMax;
    QLineEdit* expressionInput;              ///< Line edit for entering the expression
    QLabel* derivativeLabel;                 ///< Label for displaying the derivative
    QLineEdit* derivativeExpressionInput;    ///< Line edit for entering the derivative expression
    QPushButton* solve;                      ///< Button for solving the expression
    QPushButton* manualTableInput;           ///< Button for manual table input
    QTableWidget* tableWidget;               ///< Table widget for displaying data
    QGridLayout* layout;                     ///< Grid layout for arranging UI elements

    QLabel* averError;                       ///< Label for displaying the average error
    QLineEdit* error;                        ///< Line edit for displaying the error

    QLabel* oddsInputLabel;                  ///< Label for displaying the odds input
    QTableWidget* equationsTableWidget;      ///< Table widget for displaying equations
    QPushButton* solveEquations;             ///< Button for solving equations
    QPushButton* clearEquationsTable;        ///< Button for clearing the equations table
    QLabel* resultOfEquations;               ///< Label for displaying the result of equations
    QLineEdit* eqResult;                     ///< Line edit for displaying the equation result
    QLabel* resultDescription;               ///< Label for displaying the result description
    QLineEdit* description;                  ///< Line edit for displaying the description

    QLineEdit* model;                        ///< label for entering model string data.
    QLabel* modelLabel;                      ///< interpolation model label

    QLabel* calculatedArea;                  ///< Label for displaying the calculated area
    QLineEdit* area;                         ///< Line edit for displaying the area

    std::vector<double> X;                   ///< Vector for storing 'x' values
    std::vector<double> Y;                   ///< Vector for storing 'y' values
    std::vector<double> Z;                   ///< Vector for storing 'z' values
    std::vector<double> dY;                  ///< Vector for storing derivative values

    QPushButton* buildGraph;                 ///< Button for building the graph
    QWidget* buttonsWidget;
    QWidget* equationsButtonsWidget;

    std::vector<std::vector<double>> nonLinearSysResult;

    bool nonLinear{ false };

    QDoubleSpinBox* nonLinearXMin;
    QDoubleSpinBox* nonLinearXMax;
    QDoubleSpinBox* nonLinearStep;

    QLabel* nonLinearXMinLabel;
    QLabel* nonLinearXMaxLabel;
    QLabel* nonLinearStepLabel;
    /*!
     * \brief Widgets: constructor with standard heir argument.
     *
     * \param paren: pointer to the parent object.
     */
    explicit Widgets( QWidget* parent = nullptr ) : QWidget( parent ) {}

    /*!
     * \brief initWidgets: widget initialization method.
     */
    void initWidgets( void )
    {
        validator = new ValidateString( this );
        parser = new StringParser( this );

        label = new QLabel( "f(x) = ", this );
        errLabel = new ErrorLabel;

        typeOfInput = new QLabel( "Ввод значений:", this );

        typeOfVariableInput = new QComboBox( this );
        typeOfVariableInput->addItems( { "с шагом", "с узлами" } );

        model = new QLineEdit( this );
        modelLabel = new QLabel( "P(x) = ", this );

        {  // Переменные элементы
            minLabel = new QLabel( "Xmin", this );
            maxLabel = new QLabel( "Xmax", this );
            yMinLabel = new QLabel( "Ymin", this );
            yMaxLabel = new QLabel( "Ymax", this );
            stepLabel = new QLabel( "Шаг:", this );

            nodesLabel = new QLabel( "Кол-во узлов:", this );

            min = new QDoubleSpinBox( this );
            max = new QDoubleSpinBox( this );
            yMin = new QDoubleSpinBox( this );
            yMax = new QDoubleSpinBox( this );
            step = new QDoubleSpinBox( this );
            nodes = new QDoubleSpinBox( this );

            derivativeLabel = new QLabel();
            averError = new QLabel();
            error = new QLineEdit();

            oddsInputLabel = new QLabel();
            equationsTableWidget = new QTableWidget();
            solveEquations = new QPushButton();
            clearEquationsTable = new QPushButton();
            resultOfEquations = new QLabel();
            eqResult = new QLineEdit();
            resultDescription = new QLabel();
            description = new QLineEdit();
            nonLinearXMin = new QDoubleSpinBox( );
            nonLinearXMax = new QDoubleSpinBox( );
            nonLinearStep = new QDoubleSpinBox( );

            nonLinearXMinLabel = new QLabel( "X мин:" );
            nonLinearXMinLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            nonLinearXMaxLabel = new QLabel( "X макс:" );
            nonLinearXMaxLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            nonLinearStepLabel = new QLabel( "Шаг:" );
            nonLinearStepLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        } // Переменные элементы

        derivativeExpressionInput = new QLineEdit( this );
        expressionInput = new QLineEdit( this );
        QFont font( "Times New Roman", 12, -1, true );
        expressionInput->setFont( font );
        derivativeExpressionInput->setFont( expressionInput->font() );

        connect( expressionInput, &QLineEdit::textChanged, this, &Widgets::expressionInputChanged );

        manualTableInput = new QPushButton( "Ручной ввод", this );

        solve = new QPushButton( "Решить", this );
        solve->setStyleSheet( "background-color: tomato;" );
        solve->setEnabled( false );

        clearTable = new QPushButton( "Очистить таблицу", this );
        buildGraph = new QPushButton( "Построить график", this );
        buildGraph->setEnabled( false );

        tableWidget = new QTableWidget( this );
        tableWidget->setColumnCount( 2 );
        QStringList labels;
        labels << "X" << "Y";
        tableWidget->setHorizontalHeaderLabels( labels );
        buttonsWidget = new QWidget( this );
        equationsButtonsWidget = new QWidget( this );

        setRange();
    }

private:

    /*!
     * \brief setRange: setting up range for double-spinbox widgets.
     */
    void setRange( void )
    {
        min->setRange( -100.0, 100.0 );
        yMin->setRange( -100.0, 100.0 );
        min->setSingleStep( 0.1 );
        yMin->setSingleStep( 0.1 );
        max->setRange( -100.0, 100.0 );
        yMax->setRange( -100.0, 100.0 );
        max->setSingleStep( 0.1 );
        yMax->setSingleStep( 0.1 );
        step->setRange( 0.01, 10.0 );
        step->setSingleStep( 0.1 );
        nodes->setRange( 2.0, 1000.0 );
        nonLinearXMin->setRange( -100.0, 100.0 );
        nonLinearXMin->setSingleStep( 0.01 );
        nonLinearXMax->setRange( -100.0, 100.0 );
        nonLinearXMax->setSingleStep( 0.01 );
        nonLinearStep->setRange(0.001, 1);
        nonLinearStep->setSingleStep(0.001);
        nonLinearStep->setDecimals(3);
    }

signals:
    /*!
     * \brief expressionInputChanged: signal indicating a change in the state of a expression line.
     *
     * \param text: input data.
     */
    void expressionInputChanged( const QString& text );
};


#endif // WIDGETS_H

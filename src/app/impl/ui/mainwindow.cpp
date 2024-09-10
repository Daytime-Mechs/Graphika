#include "mainwindow.h"
#include "errorhandler.h"

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent )
{
    signal( SIGSEGV, signalHandler );
    Q_INIT_RESOURCE( themes );

    appearance = new Appearance( this );

    resize( 1920, 1080 );
    setMinimumSize( 640, 380 );
    setMaximumSize( QWIDGETSIZE_MAX, QWIDGETSIZE_MAX );
    setWindowTitle( "Graphika" );
    setWindowIcon( QIcon( ":/toolbaricons/resources/logo.png" ) );

    menu = new Menu( this );
    setMenuBar( menu->getMenu() );
    connect( menu, &Menu::sessionStarted, this, &MainWindow::startSession );
    connect( menu, &Menu::licenseMenuOppened, this, &MainWindow::openLicenseMenu );
    connect( menu, &Menu::aboutMenuOppened, this, &MainWindow::openAboutMenu );
    connect( menu, &Menu::authorsMenuOppened, this, &MainWindow::openAuthorsMenu );
    connect( menu, &Menu::programmatorOppened, this, &MainWindow::openProgrammerDialog );
    connect( menu, &Menu::viewMenuOppened, this, &MainWindow::openViewDialog );


    connect( qApp, &QApplication::aboutToQuit, this, &MainWindow::deleteWidgets );


    menubar = new MenuBar( this );
    menubar->setContextMenuPolicy( Qt::ContextMenuPolicy::PreventContextMenu );
    menubar->setIconSize( *new QSize( 40, 40 ) );
    addToolBar( Qt::LeftToolBarArea, menubar );

    QScrollArea* scrollArea = new QScrollArea( this );
    QWidget* scrollContentWidget = new QWidget;

    centralwidget = new QWidget( this );
    layout = new QGridLayout( centralwidget );

    leftWidget = new LeftWidget( this );
    leftWidget->initLayout( buffer, pymodules::Modules::NIL );
    menubar->actions().at( 0 )->setChecked( true );
    connect( leftWidget->currentLayout->widgets->buildGraph, &QPushButton::clicked, this, &MainWindow::draw );

    rightWidget = new RightWidget( this );

    connect( leftWidget, &LeftWidget::switchToGraphBuilder, rightWidget->graphBuilder, &GraphBuilder::switchToGraphBuilder );
    connect( leftWidget, &LeftWidget::switchToGL3DGraphBuilder, rightWidget->graphBuilder, &GraphBuilder::switchToGL3DGraphBuilder );

    centralwidget->setLayout( layout );
    setCentralWidget( centralwidget );
    scrollLayout = new QGridLayout( scrollContentWidget );

    scrollLayout->addWidget( leftWidget, 0, 0 );
    scrollLayout->addWidget( rightWidget, 0, 1 );
    scrollContentWidget->setLayout(scrollLayout);

    scrollArea->setWidget( scrollContentWidget );
    scrollArea->setWidgetResizable( true );

    reportGenerator = new ReportGenerator;

    connect( rightWidget, &RightWidget::readyToSendArea, leftWidget->currentLayout, &LayoutInitializer::acceptArea );
    scrollLayout->addWidget( leftWidget, 1, 0 );
    scrollLayout->addWidget( rightWidget, 1, 1 );
    connect( rightWidget, &RightWidget::errorOccured, leftWidget->currentLayout, &LayoutInitializer::handleParserError );
    connect( rightWidget, &RightWidget::readyToSendData, leftWidget->currentLayout, &LayoutInitializer::acceptData );
    connect( rightWidget, &RightWidget::readyToSendArea, leftWidget->currentLayout, &LayoutInitializer::acceptArea );
    connect( leftWidget->currentLayout, &LayoutInitializer::readyToDraw, rightWidget, &RightWidget::drawInterpolationGraph );
    connect( this, &MainWindow::rebuildWidgets, leftWidget, &LeftWidget::initLayout );

    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

    scrollArea->setWidget( scrollContentWidget );
    layout->addWidget( scrollArea, 0, 0 );

    //connect( rightWidget->graphBuilder, &GraphBuilder::couldSavePlotAsImage, this, &MainWindow::couldSavePlotAsImage );

    menuSlots.insert( menubar->actions().at( 0 ), [ this ]()
                     { openMenu( 0, pymodules::Modules::NIL ); } );
    menuSlots.insert( menubar->actions().at( 1 ), [ this ]()
                     { openMenu( 1, pymodules::Modules::DIFFERENTIATION ); } );
    menuSlots.insert( menubar->actions().at( 2 ), [ this ]()
                     { openMenu( 2, pymodules::Modules::INTEGRATION ); } );
    menuSlots.insert( menubar->actions().at( 3 ), [ this ]()
                     { openMenu( 3, pymodules::Modules::EQUATIONS ); } );
    menuSlots.insert( menubar->actions().at( 5 ), [ this ]()
                     { openMenu( 5, pymodules::Modules::POLYNOMIALS ); } );
    menuSlots.insert( menubar->actions().at( 6 ), [ this ]()
                     { openMenu( 6, pymodules::Modules::POLYNOMIALS ); } );
    menuSlots.insert( menubar->actions().at( 7 ), [ this ]()
                     { openMenu( 7, pymodules::Modules::POLYNOMIALS ); } );
    for( auto act = menuSlots.constBegin(); act != menuSlots.constEnd(); ++act )
    {
        connect( act.key(), &QAction::triggered, this, act.value() );
    }

    programmer = new ProgrammerDialog( this );
    connect( programmer, &ProgrammerDialog::settingsApplied, leftWidget, &LeftWidget::applyProgrammerSettings );
    connect( menubar, &MenuBar::currentMethodChanged, rightWidget, &RightWidget::updateLegend );
    connect( leftWidget, &LeftWidget::functionTextChanged, rightWidget, &RightWidget::setFunctionText );
}

void MainWindow::openMenu( int index, pymodules::Modules module )
{
    menubar->unsetChecked();
    menubar->actions().at( index )->setChecked( true );
    widgetState = module;
    buildSpecificWidget( index );
}

void MainWindow::buildSpecificWidget( int index )
{
    switch( index )
    {
    case 0:
        [[fallthrough]];
    case 1:
        [[fallthrough]];
    case 2:
        emit rebuildWidgets( buffer, widgetState );
        connect( leftWidget->currentLayout->widgets->buildGraph, &QPushButton::clicked, this, &MainWindow::draw );
        break;
    case 3:
        emit rebuildWidgets( buffer, widgetState );
        connect( leftWidget->currentLayout, &LayoutInitializer::readyToSendEquationsData, this, &MainWindow::calculateSys );
        connect( rightWidget, &RightWidget::readyToSendSysResult, leftWidget->currentLayout, &LayoutInitializer::setEquationsResult );
        break;
    case 5:
        emit rebuildWidgets( buffer, widgetState );
        methodOfInterpolation = pymodules::Methods::LAGRANGE;
        connect( leftWidget->currentLayout->widgets->buildGraph, &QPushButton::clicked, this, &MainWindow::buildPolynomeGraph );
        break;
    case 6:
        emit rebuildWidgets( buffer, widgetState );
        methodOfInterpolation = pymodules::Methods::NEWTON;
        connect( leftWidget->currentLayout->widgets->buildGraph, &QPushButton::clicked, this, &MainWindow::buildPolynomeGraph );
        break;
    case 7:
        emit rebuildWidgets( buffer, widgetState );
        methodOfInterpolation = pymodules::Methods::BEIRUT;
        connect( leftWidget->currentLayout->widgets->buildGraph, &QPushButton::clicked, this, &MainWindow::buildPolynomeGraph );
        break;
    }
}

void MainWindow::draw( void )
{
    switch( widgetState )
    {
    case pymodules::Modules::NIL:
        printFunctionGraph();
        break;
    case pymodules::Modules::DIFFERENTIATION:
        printDiffGraph();
        break;
    case pymodules::Modules::POLYNOMIALS:
        buildPolynomeGraph();
        break;
    case pymodules::Modules::INTEGRATION:
        calculateIntegral();
        break;
    default:
        break;
    }
}

void MainWindow::deleteWidgets( void )
{
    ReferenceMenu::cleanupWidgets( widgets );
    widgets.clear();
}

void MainWindow::buildPolynomeGraph( void )
{
    switch( methodOfInterpolation )
    {
    case pymodules::Methods::LAGRANGE:
        invokePolynomeMethod( pymodules::Methods::LAGRANGE );
        break;
    case pymodules::Methods::NEWTON:
        invokePolynomeMethod( pymodules::Methods::NEWTON );
        break;
    case pymodules::Methods::BEIRUT:
        invokePolynomeMethod( pymodules::Methods::BEIRUT );
        break;
    default:
        break;
    }
}

void MainWindow::printFunctionGraph( void )
{
    if( isSession )
    {
        rightWidget->printGraph( buffer, sender, &logStack );
        sessionWidget->addGraphToCounter();
        return;
    }
    rightWidget->printGraph( buffer, sender, nullptr );
}


void MainWindow::printDiffGraph( void )
{
    pymodules::Methods method = menubar->getSelectedDiffMethod();
    sender.setMacro( method, pymodules::Modules::DIFFERENTIATION );
    if( isSession )
    {
        rightWidget->printDiffGraph( buffer, sender, &logStack );
        sessionWidget->addGraphToCounter();
        return;
    }
    rightWidget->printDiffGraph( buffer, sender, nullptr );
}

void MainWindow::calculateIntegral( void )
{
    pymodules::Methods method = menubar->getSelectedIntegralMethod();
    sender.setMacro( method, pymodules::Modules::INTEGRATION );
    if( isSession )
    {
        rightWidget->calculateIntegral( buffer, sender, &logStack );
        sessionWidget->addGraphToCounter();
        return;
    }
    rightWidget->calculateIntegral( buffer, sender, nullptr );
}

void MainWindow::calculateSys( QVector<QVector<double>>& data )
{
    pymodules::Methods method = menubar->getSelectedSysMethod();
    sender.setMacro( method, pymodules::Modules::EQUATIONS );
    rightWidget->sysSolve( data, sender );
}

void MainWindow::invokePolynomeMethod( pymodules::Methods method )
{
    widgetState = pymodules::Modules::POLYNOMIALS;
    sender.setMacro( method, pymodules::Modules::POLYNOMIALS );
    if( isSession )
    {
        rightWidget->buildPolynome( buffer, sender, &logStack );
        sessionWidget->addGraphToCounter();
        return;
    }
    rightWidget->buildPolynome( buffer, sender, nullptr );
}


void MainWindow::openAboutMenu( void )
{
    widgets.append( ReferenceMenu::invokeAboutWidget() );
}

void MainWindow::openAuthorsMenu( void )
{
    widgets.append( ReferenceMenu::invokeAuthorsWidget() );
}

void MainWindow::openLicenseMenu( void )
{
    widgets.append( ReferenceMenu::invokeLicenseWidget() );
}

void MainWindow::openViewDialog( void )
{
    appearance->show();
}

void MainWindow::startSession( void )
{
    sessionWidget = new SessionWidget(this);

    addToolBar( Qt::RightToolBarArea, sessionWidget->getToolBar() );
    sessionWidget->startSession();

    isSession = true;
    menu->getStartSessionAction()->setEnabled( false );
    auto* l = new ErrorLabel( this );
    l->setPopupText( "Начался сессионный режим" );
    l->show();
    connect(sessionWidget, &SessionWidget::sessionStopped, this, &MainWindow::showSaveDialog);
}

void MainWindow::openProgrammerDialog()
{
    programmer->setWindowTitle(tr ( "Программатор" ) );
    programmer->resize( 600, 400 );
    programmer->move( 100, 100 );
    programmer->show();
}

void MainWindow::showSaveDialog( const QTime& elapsedTime )
{
    isSession = false;

    QDialog* saveDialog = new QDialog(this);
    saveDialog->setWindowTitle("Сохранить результат проделанной работы");

    QLineEdit* titleEdit = new QLineEdit();
    titleEdit->setPlaceholderText("Наименование работы");

    QLineEdit* authorsEdit = new QLineEdit();
    authorsEdit->setPlaceholderText("Работу выполнил(и)");

    QPushButton* saveButton = new QPushButton("Сохранить");
    QPushButton* cancelButton = new QPushButton("Отмена");

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(titleEdit);
    layout->addWidget(authorsEdit);
    layout->addWidget(saveButton);
    layout->addWidget(cancelButton);

    saveDialog->setLayout(layout);

    connect(saveButton, &QPushButton::clicked, [this, saveDialog, titleEdit, authorsEdit, elapsedTime]() {
        QString title = titleEdit->text();
        QString authors = authorsEdit->text();
        QString filePath = QFileDialog::getSaveFileName(this, "Save PDF", QString(), "PDF Files (*.pdf)");
        if (!filePath.isEmpty()) {
            reportGenerator->generateReport( logStack, title, authors, elapsedTime, filePath );
            saveDialog->accept();
        }
    });

    connect(cancelButton, &QPushButton::clicked, saveDialog, &QDialog::reject);

    saveDialog->exec();

    menu->getStartSessionAction()->setEnabled( true );
}

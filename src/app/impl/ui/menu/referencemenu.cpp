#include "referencemenu.h"

QWidget* ReferenceMenu::invokeLicenseWidget( void ) noexcept
{
    QTextEdit* textEdit = new QTextEdit;
    textEdit->setReadOnly( true );

    QFile file(":/references/resources/fdl-1.3.txt");
    if( file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        textEdit->setPlainText( file.readAll() );
        file.close();
    }
    else
    {
        qDebug() << "Unable to open license text!\n";
    }

    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidget( textEdit );
    scrollArea->setWidgetResizable( true );

    QWidget* wgt = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget( scrollArea );
    wgt->setLayout( layout );
    wgt->resize( textEdit->width(), textEdit->width() );
    wgt->setWindowTitle( "Лицензия" );
    wgt->setFixedSize( wgt->size() );
    wgt->setWindowIcon( QIcon( ":/toolbaricons/resources/logo2.PNG" ) );
    wgt->show();
    return wgt;
}

QWidget* ReferenceMenu::invokeAuthorsWidget( void ) noexcept
{
    QWidget* wgt = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout( wgt );

    QLabel* imgLabel = new QLabel;
    QPixmap pixmap = QPixmap( ":/toolbaricons/resources/authors.PNG" );
    imgLabel->setPixmap( pixmap );

    layout->addWidget( imgLabel );

    wgt->setLayout( layout );
    wgt->setWindowTitle( "Разработчики" );
    wgt->setFixedSize( pixmap.size() );
    wgt->setWindowIcon( QIcon( ":/toolbaricons/resources/logo2.PNG" ) );
    wgt->show();
    return wgt;
}

QWidget* ReferenceMenu::invokeAboutWidget( void ) noexcept
{
    QWidget* wgt = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout( wgt );

    QLabel* imgLabel = new QLabel;
    QPixmap pixmap = QPixmap( ":/references/resources/aboutSheet.jpg" );
    imgLabel->setPixmap( pixmap );

    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole( QPalette::Dark );
    scrollArea->setWidget( imgLabel );

    layout->addWidget( scrollArea );

    wgt->setLayout( layout );
    wgt->setWindowTitle( "Руководство пользователя" );
    wgt->resize( pixmap.width(), pixmap.width() / 1.5 );
    wgt->setFixedSize( wgt->size() );
    wgt->setWindowIcon( QIcon( ":/toolbaricons/resources/logo2.PNG" ) );
    wgt->show();
    return wgt;
}

void ReferenceMenu::cleanupWidgets( QList<QWidget*>& widgets )
{
    for( const auto wgt : widgets )
    {
        wgt->deleteLater();
    }
}

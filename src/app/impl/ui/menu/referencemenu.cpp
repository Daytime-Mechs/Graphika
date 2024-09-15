#include "referencemenu.h"

#include <QScrollBar>

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
    QVBoxLayout* layout = new QVBoxLayout(wgt);

    QLabel* imgLabel1 = new QLabel;
    QPixmap pixmap1(":/references/resources/doc2.png");
    imgLabel1->setPixmap(pixmap1);

    QLabel* imgLabel2 = new QLabel;
    QPixmap pixmap2(":/references/resources/doc1.png");
    imgLabel2->setPixmap(pixmap2);

    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imgLabel1);

    layout->addWidget(scrollArea);

    wgt->setLayout(layout);
    wgt->setWindowTitle("Руководство пользователя");
    wgt->resize(pixmap1.width(), pixmap1.width() / 2);
    wgt->setFixedSize(wgt->size());
    wgt->setWindowIcon(QIcon(":/toolbaricons/resources/logo2.PNG"));
    wgt->show();

    QObject::connect(scrollArea->verticalScrollBar(), &QScrollBar::valueChanged, [scrollArea, imgLabel1, imgLabel2](int value) {
        if (value == scrollArea->verticalScrollBar()->maximum()) {
            scrollArea->setWidget(imgLabel2);
        }
    });

    return wgt;
}

void ReferenceMenu::cleanupWidgets( QList<QWidget*>& widgets )
{
    for( const auto wgt : widgets )
    {
        wgt->deleteLater();
    }
}

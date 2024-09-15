/*!
               -=*#%@@@@@%#*=:
            -#@@@@@@@@@@@@@@@@@%
          :%@@@@@@@@@@@@@@@@@@@@
         =@@@@@@@@@@@@@@@@@@@@%:
        :@@@@@@@@@%-.    .:++:
        #@@@@@@@@*    ::::::::::.
        @@@@@@@@@.   =@@@@@@@@@@@
        @@@@@@@@@    =@@@@@@@@@@@
        #@@@@@@@@*   -@@@@@@@@@@@
        :@@@@@@@@@%+=----%@@@@@@@
         :@@@@@@@@@@@@@@@@@@@@@@@
           +@@@@@@@@@@@@@@@@@@@@@
             =#@@@@@@@@@@@@@@@@#-

        Project: Graphika,
        by: "Dnevnie Mechaniki".

        \authors
        Malaniya Mark Timurovich,
        Korshunov Ilya Dmitrievich,
        Vilenskiy Kirill Romanovich,
        Konak Vladimir Evgen'evich.
*/

#include <QApplication>
#include <QCoreApplication>
#include <QMessageLogContext>
#include <QDebug>
#include <iostream>
#include "config.h"
#include "mainwindow.h"
#include "consoleapplication.h"

enum class ApplicationMode
{
    CONSOLE, GUI
};

ApplicationMode getApplicationMode( int argc, char* argv[] )
{
    for( int i = 1; i < argc; ++i )
    {
        if( strcmp( argv[i], "--console" ) == 0 )
        {
            return ApplicationMode::CONSOLE;
        }
        else if( strcmp( argv[i], "--gui" ) == 0 )
        {
            return ApplicationMode::GUI;
        }
    }
    return ApplicationMode::GUI;
}

int main( int argc, char *argv[] )
{
    ApplicationMode mode = getApplicationMode( argc, argv );

#if defined( CONSOLE_MODE )
    if( mode == ApplicationMode::CONSOLE )
    {
        qInstallMessageHandler( []( QtMsgType type, const QMessageLogContext& context, const QString& msg )
            {
                if ( type == QtWarningMsg && ( msg.contains( "Attribute Qt::AA_UseDesktopOpenGL" ) || msg.contains( "index out of bounds" ) ) )
                {
                    return;
                }
            });
        QApplication a( argc, argv );
        ConsoleApplication consoleApp;
        consoleApp.run();

        return 0;
    }
#endif

#if defined( GUI_MODE )
    if( mode == ApplicationMode::GUI )
    {
        QApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
        QApplication a( argc, argv );

        MainWindow window;

        QSplashScreen* ss = new QSplashScreen;

        QPixmap logo( ":/toolbaricons/resources/logo.PNG" );
        QPixmap scaledLogo = logo.scaled( 255, 255, Qt::KeepAspectRatio, Qt::SmoothTransformation );

        ss->setPixmap( scaledLogo );
        ss->show();

        QTimer::singleShot( 1000, [&]()
        {
            ss->finish( &window );
            window.show();
        });

        return a.exec();
    }
#endif
    qCritical() << "Invalid appliation mode.\nExiting.\n";
    return 1;
}

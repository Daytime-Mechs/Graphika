#include "menu.h"

Menu::Menu( QWidget *parent )
    : QMainWindow{ parent }
{
    createMenu( parent );
}

void Menu::createMenu( QWidget* parent )
{
    menu = new QMenuBar( parent );

    QMenu* viewMenu = menu->addMenu( tr( "&Вид" ) );
    QAction* viewAction = viewMenu->addAction( "Внешний вид" );
    connect( viewAction, &QAction::triggered, this, &Menu::viewMenuOppened );

    QMenu* aboutMenu = menu->addMenu( tr( "&О программе" ) );
    QAction* licenceAction = aboutMenu->addAction( tr( "Лицензия" ) );
    QAction* authorsAction = aboutMenu->addAction( tr( "Об авторах" ) );
    QAction* programmAction = aboutMenu->addAction( tr( "Руководство" ) );
    connect( programmAction, &QAction::triggered, this, &Menu::aboutMenuOppened );
    connect( licenceAction, &QAction::triggered, this, &Menu::licenseMenuOppened );
    connect( authorsAction, &QAction::triggered, this, &Menu::authorsMenuOppened );

    QMenu* utilsMenu = menu->addMenu( tr( "&Утилиты" ) );
    startSessionAction = utilsMenu->addAction( tr( "Начать сессию" ) );
    QAction* programmatorAction = utilsMenu->addAction( tr( "Программатор" ) );
    connect( startSessionAction, &QAction::triggered, this, &Menu::sessionStarted );
    connect(programmatorAction, &QAction::triggered, this, &Menu::programmatorOppened );
}

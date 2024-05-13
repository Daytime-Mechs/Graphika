#ifndef MENU_H
#define MENU_H

#include <QMenuBar>
#include <QMainWindow>
#include <QtWidgets>

class Menu : public QMainWindow
{
    Q_OBJECT
private:
    QWidget* parent;
    QAction* startSessionAction;
    QAction* endSessionAction;
    QAction* authorsAction;
    QMenuBar* menu;

public:
    explicit Menu( QWidget *parent = nullptr );
    QMenuBar* getMenu( void ) { return menu; }
    QAction* getStartSessionAction( void ) const
    { return startSessionAction; }
    QAction* getEndSessionAction( void ) const
    { return endSessionAction; }

private:
    void createMenu( QWidget *parent );

signals:
    void sessionStarted( void );
    void sessionStopped( void );
    void licenseMenuOppened( void );
    void authorsMenuOppened( void );
    void aboutMenuOppened( void );
};

#endif // MENU_H

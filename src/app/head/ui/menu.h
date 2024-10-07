/*!
 * \attention In development.
 *
 * \author Malaniya Mark Timurovich, Dnevnie Mechaniki.
 *
 * \date last update: 15.09.2024.
 */

#ifndef MENU_H
#define MENU_H

#include <QMenuBar>
#include <QMainWindow>
#include <QtWidgets>
#include "programmerdialog.h"
#include "leftwidget.h"
#include "pluginDialog/PluginsListDialog.h"

/*!
 * \class Menu
 * \brief The Menu class. Contains a list of additional services.
 */
class Menu : public QMainWindow
{
    Q_OBJECT
public:
QAction* programmatorAction; ///< QAction to trigger the programmer dialog.

private:
    QWidget* parent; ///< A pointer to the parent widget of the menu.
    QAction* startSessionAction; ///< A QAction representing the action to start a session.
    QAction* authorsAction; ///< A QAction representing the action to display the authors' information.
    QMenuBar* menu; ///< A pointer to the QMenuBar object representing the main menu.


public:
    /*!
     * \brief Menu: standard constructor.
     *
     * \param parent: pointer to the parent object.
     */
    explicit Menu( QWidget *parent = nullptr );

    /*!
     * \brief getMenu: standard getter.
     * \return menu
     */
    QMenuBar* getMenu( void ) { return menu; }

    /*!
     * \brief getStartSessionAction: standard getter.
     *
     * \return startSessionAction.
     */
    QAction* getStartSessionAction( void ) const
    { return startSessionAction; }

private:
    /*!
     * \brief createMenu: A method that creates and configures the main menu of the application.
     *
     * \param parent: pointer to the parent object.
     */
    void createMenu( QWidget *parent );

signals:
    /*!
     * \brief sessionStarted: A signal emitted when a session is started.
     */
    void sessionStarted( void );

    /*!
     * \brief sessionStopped: A signal emitted when a session is stopped.
     */
    void sessionStopped( void );

    /*!
     * \brief licenseMenuOppened: A signal emitted when the license menu is opened.
     */
    void licenseMenuOppened( void );

    /*!
     * \brief authorsMenuOppened: A signal emitted when the authors menu is opened.
     */
    void authorsMenuOppened( void );

    /*!
     * \brief aboutMenuOppened: A signal emitted when the about menu is opened.
     */
    void aboutMenuOppened( void );

    /*!
     * \brief programmatorOppened: A signal emitted when the programmator menu is opened
     */
    void programmatorOppened( void );

    void pluginsOppened( void );

    /*!
     * \brief viewMenuOppened: A signal when the view menu is opened
     */
    void viewMenuOppened( void );
};

#endif // MENU_H

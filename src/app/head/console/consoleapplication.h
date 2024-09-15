#ifndef CONSOLEAPPLICATION_H
#define CONSOLEAPPLICATION_H

#include <QMap>
#include <functional>
#include <iostream>
#include <sstream>
#include <tuple>
#include "commandhandler.h"
#include "commandcompleter.h"
#include "stringparser.h"
#include "rightwidget.h"
#include <vector>


#ifdef Q_OS_WIN
#include <windows.h>
#elif defined( Q_OS_UNIX )
#include <unistd.h>
#include <pwd.h>
#endif

typedef std::vector<std::vector<double>> pair;

/*!
 * \brief The ConsoleApplication class for non GUI systems ( in development )
 */
class ConsoleApplication
{
private:
    CommandCompleter* completer;
    Statement statement = Statement::HOME;
    std::string state;
    using CommandFunction = std::function<void(const QStringList&)>;
    QMap<QString, std::pair<QString, CommandFunction>> commands;
    StringParser* parser;

    pymodules::Methods methodOfInterpolation;
    pymodules::Methods methodOfIntegration;

public:
    ConsoleApplication( void )
    {
        parser = new StringParser();
        initCommandMap();
    }
    int run( void );

    void registerCommand( const QString& name, const QString& description, CommandFunction function );
    void handleCommand( const QString& input );

private:
    void initCommandMap( void );
    void executeCommand( const QString& command );
    void printHelp( void );
    void printCommandHelp( const QString& commandName );
    QString getUserName( void );

    void solveInterpolation( void );
    void solveInterpolation( const std::vector<double>& x, const std::vector<double>& y );
    void solveIntegration( void );
    void solveIntegration( const std::vector<double>& x, const std::vector<double>& y );
    pair enterFunction( void );
    int manualInput( std::vector<std::vector<double>>& xy );
    pair setFunctionData( void );
    bool parseArguments( const std::string& input, std::vector<double>& x, std::vector<double>& y );
    QVector<double> enterRanges( const QString& func );

    template<typename T>
    pair setDataVariables( T ranges, const QString& func );
};

#endif

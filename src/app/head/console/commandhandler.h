#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <QStringList>

enum Statement
{
    HOME, FUNCTION, DERIVATION, INTEGRAION, EQUATION, INTERPOLATION
};

class CommandHandler
{
public:
    static std::string checkoutStatement( const Statement& state );
    static void initFunction( const std::string& command );
    static int handleCommand( const QStringList& args );
};

#endif // COMMANDHANDLER_H

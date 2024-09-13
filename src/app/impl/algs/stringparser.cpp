#include "stringparser.h"

StringParser::StringParser( QObject *parent ) : QObject( parent ) {}

std::optional<Expression> StringParser::parseExpression()
{
    return parseBinaryExpression( 0 );
}

std::vector<double> StringParser::parseExpression( QString input, int dimensional )
{
    QByteArray bytes = input.toUtf8();
    this->input = reinterpret_cast<unsigned const char*>( bytes.constData() );
    std::optional<Expression> parsed = parseExpression();

    if( dimensional == 3 )
    {
        double z{};
        std::vector<double> zTable;
        for( size_t i = 0; i < yTable.size(); ++i )
        {
            z = eval( parsed, xTable[i], yTable[i] );
            zTable.push_back( z );
        }
        return zTable;
    }
    else
    {
        double y{};
        std::vector<double> yTable;
        for( const auto& x : xTable )
        {
            y = eval( parsed, x );
            yTable.push_back( y );
        }
        return yTable;
    }
    [[unlikely]]
    return std::vector<double>();
}

std::string StringParser::parseToken()
{
    while( std::isspace( *input ) )
    {
        ++input;
    }

    if(  std::isdigit( *input ) )
    {
        std::string number;
        while( std::isdigit( *input ) || *input == '.' )
        {
            number.push_back( *input++ );
        }
        return number;
    }

    if( *input == 'x' )
    {
        ++input;
        return "x";
    }

    if( *input == 'y' )
    {
        ++input;
        return "y";
    }

    static const std::vector<std::string> tokens =
        { "+", "-", "*", "/", "^", "abs", "sin",
          "sh", "cos", "ch", "ln", "lg", "tan", "asin",
          "acos", "atan", "cot", "th", "sqrt", "cbrt", "ceil",
          "floor", "round", "exp", "(", ")" };
    for( auto& t : tokens )
    {
        if( std::strncmp( reinterpret_cast<const char*>( input ), t.c_str(), t.size() ) == 0 )
        {
            input += t.size();
            return t;
        }
    }

    return "";
}

std::optional<Expression> StringParser::parseSimpleExpression()
{
    auto token = parseToken();

    if( token.empty() )
    {
        return std::nullopt;
    }

    if( token == "x" )
    {
        return Expression( token, true );
    }

    if( token == "y" )
    {
        return Expression( token, true );
    }

    if ( token == "(" )
    {
        auto result = parseExpression();

        if ( parseToken() != ")" )
        {
            return std::nullopt;
        }

        return result;
    }

    if ( std::isdigit( token[0] ) )
    {
        return Expression( token ) ;
    }


    auto nestedExpr = parseSimpleExpression();
    if( !nestedExpr.has_value() )
    {
        return std::nullopt;
    }
    return Expression( token, *nestedExpr );
}

std::optional<Expression> StringParser::parseBinaryExpression( const int minPriority )
{
    auto leftExpr = parseSimpleExpression();

    while( true )
    {
        auto operation = parseToken();
        auto priority = getPriority( operation );

        if( priority <= minPriority )
        {
            input -= operation.size();
            return leftExpr;
        }

        auto rightExpr = parseBinaryExpression( priority );
        if( !rightExpr.has_value() )
        {
            return std::nullopt;
        }
        leftExpr = Expression( operation, leftExpr.value(), rightExpr.value() );
    }
}

int StringParser::getPriority( const std::string &token )
{
    if( token == "+" )   return 1;
    if( token == "-" )   return 1;
    if( token == "*" )   return 2;
    if( token == "/" )   return 2;
    if( token == "%" )   return 2;
    if( token == "^" )   return 3;
    return 0;
}

double StringParser::eval( const std::optional<Expression> &e, double x, std::optional<double> y )
{
    if( !e.has_value() )
    {
        emit errorOccurred( "Ошибка ввода" );
        return 0.0;
    }

    auto expr = e.value();
    if( expr.isVariable )
    {
        if( expr.token == "x" )
        {
            return x;
        }
        else if( expr.token == "y" )
        {
            if( y.has_value() )
            {
                return y.value();
            }
            else
            {
                emit errorOccurred( "Переменная 'y' не определена" );
                return 0.0;
            }
        }
    }

    switch( expr.arguments.size() )
    {
    case 2:
    {
        auto a = eval( expr.arguments[0], x, y );
        auto b = eval( expr.arguments[1], x, y );

        if( expr.token == "+" )   return a + b;
        if( expr.token == "-" )   return a - b;
        if( expr.token == "*" )   return a * b;
        if( expr.token == "^" )
        {
            if( b < 0 && a == 0 )
            {
                emit errorOccurred( "На ноль делить нельзя" );
            }
            return pow( a, b );
        }
        if( expr.token == "/" )
        {
            if( b == 0 )
            {
                emit errorOccurred( "На ноль делить нельзя" );
            }
            return a / b;
        }
        emit errorOccurred( "Неизвестный бинарный оператор" );
    }
    case 1:
    {
        auto a = eval( expr.arguments[0], x, y );

        //TODO: подумать над инкапсуляции реализации путем добавления хеш таблицы.
        if( expr.token == "+"    )     return +a;
        if( expr.token == "-"    )     return -a;
        if( expr.token == "abs"  )     return abs( a );
        if( expr.token == "sin"  )     return sin( a );
        if( expr.token == "sh"   )     return sinh( a );
        if( expr.token == "cos"  )     return cos( a );
        if( expr.token == "ch"   )     return cosh( a );
        if( expr.token == "tan"  )     return tan( a );
        if( expr.token == "cot"  )     return cos( a ) / sin( a );
        if( expr.token == "th"   )     return tanh( a );
        if( expr.token == "asin" || expr.token == "acos" || expr.token == "atan" )
        {
            if( a < -1 || a > 1 )
            {
                emit errorOccurred( "Не определен" );
            }
            else
            {
                if( expr.token == "atan" )     return atan( a );
                if( expr.token == "acos" )     return acos( a );
                if( expr.token == "asin" )     return asin( a );
            }
        }
        if( expr.token == "ceil" )     return ceil( a );
        if( expr.token == "floor")     return floor( a );
        if( expr.token == "round")     return round( a );
        if( expr.token == "sqrt" || expr.token == "cbrt" )
        {
            if( a < 0 )
            {
                emit errorOccurred( "Не определен" );
            }
            else
            {
                if( expr.token == "sqrt" ) return sqrt( a );
                return cbrt( a );
            }
        }
        if( expr.token == "ln" || expr.token == "lg" )
        {
            if( a <= 0 )
            {
                emit errorOccurred( "Функция не определена" );
            }
            else
            {
                if( expr.token == "ln" ) return log( a );
                return log10( a );
            }
        }
        if( expr.token == "exp" ) return exp( a );
    }
    case 0:
    {
        if( expr.token == "x" )
        {
            return x;
        }
        return std::strtod( expr.token.c_str(), nullptr );
    }
    }
    emit errorOccurred( "Неизвестный вид выражения" );
}

double StringParser::eval( const std::optional<Expression> &e )
{
    return eval( e, 0.0 );
}


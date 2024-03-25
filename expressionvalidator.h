#ifndef EXPRESSIONVALIDATOR_H
#define EXPRESSIONVALIDATOR_H

#include <iostream>
#include <regex>
#include <stack>
#include <QRegExpValidator>
#include <QRegularExpressionValidator>
#include <QRegularExpression>

/*
 * TODO:
 * 1) Реализовать алгоритм проверки чтобы после x не было иных символов кроме задуманных
 * 2) Добавить поддержку double чисел на манер Z = a.b
*/
class ExpressionValidator {

public:
    static bool validateExpression( const QString& expression) {

        if ( !isExpressionValid( expression ) )
        {
            return false;
        }

        if ( !areParenthesesValid( expression ) )
        {
            return false;
        }

        return true;
    }

private:
    static bool isExpressionValid( const QString& expression )
    {
        QRegularExpression expr(R"(^(?!.*xx)[x([(\d+\.\d+)|(0-9))+\-*/^()%]|sin|cos|ln|lg|abs|\s]+$)");
        QRegularExpressionMatch match = expr.match(expression.toStdString().c_str());
        if (match.hasMatch()) {
            return true;
        }
        return false;
    }

    static bool areParenthesesValid( const QString& expression )
    {
        std::stack<char> parenthesesStack;
        for ( char c : expression.toStdString() ) {
            if ( c == '(' )
            {
                parenthesesStack.push( c );
            }
            else if ( c == ')' )
            {
                if ( parenthesesStack.empty() || parenthesesStack.top() != '(' )
                {
                    return false;
                }
                parenthesesStack.pop();
            }
        }
        return parenthesesStack.empty();
    }
};


#endif // EXPRESSIONVALIDATOR_H

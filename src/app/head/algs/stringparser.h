/*!
 * \attention In development.
 *
 * \author Malaniya Mark Timurovich, Dnevnie Mechaniki.
 *
 * \date last update: 18.05.2024.
 */

#ifndef STRINGPARSER_H
#define STRINGPARSER_H

#include <QtWidgets>
#include <optional>
#include <vector>
#include <cctype>
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <optional>
#include <regex>
#include <cmath>

/*!
 * \class Expression.
 *
 * \brief The Expression class
 */
struct Expression
{
    /*!
     * \brief Expression: 1 argument constructor.
     *
     * \param token: a string typed math expression.
     */
    Expression( std::string token ) :
        token( token ), isVariable( false ) {}

    /*!
     * \brief Expression: 2 argument constructor.
     *
     * \param token: a string typed math expression.
     * \param isVariable: variable or part of a complex mathematical function.
     */
    Expression( std::string token, bool isVariable ) :
        token( token ), isVariable( isVariable ) {}

    /*!
     * \brief Expression: 2 argument constructor.
     *
     * \param token: a string typed math expression.
     * \param a: first argument ( ex.: 5 ).
     */
    Expression( std::string token, Expression a ) :
        token( token ), arguments{ a }, isVariable( false ) {}

    /*!
     * \brief Expression: 3 argument constructor.
     *
     * \param token: a string typed math expression.
     * \param a: first argument.
     * \param b: second argument.
     *
     * Ex.: 5 + x.
     */
    Expression( std::string token, Expression a, Expression b ) :
        token( token ), arguments{ a, b }, isVariable( false ){};

    std::string token; ///< a string of math expression
    std::vector<Expression> arguments; ///<
    bool isVariable; ///< variable or part of a complex mathematical function.
};

/*!
 * \class Expression.
 *
 * \brief The StringParser class
 */
class StringParser : public QObject
{
    Q_OBJECT
private:
    unsigned const char* input; ///< a line into which a string mathematical expression is entered.
    double x; ///< the value of the number x at a specific node.
    double y;

    std::vector<double> xTable; ///< table of x data values.
    std::vector<double> yTable;

public:
    /*!
     * \brief StringParser: constructor with standard heir argument.
     *
     * \param parent: pointer to the parent object.
     */
    explicit StringParser( QObject *parent = nullptr );

    /*!
     * \brief StringParser: constructor with a math expression argument and one node.
     *
     * \param input: a line with a string mathematical expression.
     * \param x: discretely specified node.
     */
    explicit StringParser( unsigned char* input, const double x )
        : input( input ), x( x ) {}

    /*!
     * \brief StringParser: constructor with a math expression argument.
     *
     * \param input: a line with a string mathematical expression.
     */
    explicit StringParser( unsigned char* input )
        : input( input ) {}

    /*!
     * \brief parseExpression: generic method for calling expression parsing.
     *
     * \return nested expression.
     */
    std::optional<Expression> parseExpression( void );

    /*!
     * \brief parseExpression: a method that converts the parameters given to it into the result of a mathematical calculation by substituting the values ​​at point x into the given expression.
     *
     * \param input: a string typed math expression.
     *
     * \return a vector of y values for every given x node.
     */
    std::vector<double> parseExpression( QString input, int dimensional );

    /*!
     * \brief setX: setter.
     *
     * \param x: const value of x node.
     */
    void setX( const double& x ) { this->x = x; }

    /*!
     * \brief setDataX: vector setter.
     *
     * \param x: a list of x values.
     */
    void setDataX( const std::vector<double>& x ) { xTable = x; }

    void setDataY( const std::vector<double>& y ) { yTable = y; }

    void setDataZ( const std::vector<double>& z ) { yTable = z; }

    /*!
     * \brief eval: method of comparing a given expression with installed functions.
     *
     * \param e: a structure of math expressions.
     * \param x: a double node.
     *
     * \return: result of expression evaluation.
     */
    double eval( const std::optional<Expression>& e, double x, std::optional<double> y = std::nullopt );

    /*!
     * \brief eval: method of comparing a given expression with installed functions.
     *
     * \param e: a structure of math expressions.
     *
     * \return evaluation of expression where x = 0.
     */
    double eval( const std::optional<Expression>& e );

private:
    /*!
     * \brief parseToken: checks and corrects a given string of a mathematical expression.
     *
     * \return correct math expression as string.
     */
    std::string parseToken( void );

    /*!
     * \brief parseSimpleExpression: parses a simple mathematical expression.
     *
     * \return: structure of a mathematical expression.
     */
    std::optional<Expression> parseSimpleExpression( void );

    /*!
     * \brief parseBinaryExpression: parses a mathematical expression of the form "left" "operation" "right".
     *
     * \param minPriority: priority of a given operation.
     *
     * \return structure of a mathematical expression.
     */
    std::optional<Expression> parseBinaryExpression( const int minPriority );

    /*!
     * \brief getPriority: calculating the priority of a mathematical operation between expressions.
     *
     * \param token: math expression.
     *
     * \return degree of priority. The higher, the more priority.
     */
    int getPriority( const std::string& token );

signals:
    /*!
     * \brief errorOccurred: signal providing information about an incorrectly entered line.
     *
     * \param err: error message info.
     */
    void errorOccurred( const QString& err );
};


#endif // STRINGPARSER_H

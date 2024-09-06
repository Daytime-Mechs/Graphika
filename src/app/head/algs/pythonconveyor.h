/*!
 * \class PythonConveyor.
 *
 * \brief The PythonConveyor class for interacting with Python From C++.
 *
 * \author Korshunov Ilya Dmitrievich, Dnevnie Mechaniki.
 *
 * \date last update: 20.05.2024.
 */

#ifndef PYTHONCONVEYOR_H
#define PYTHONCONVEYOR_H

#include <Python.h>
#include <QObject>
#include <QString>
#include <vector>

class PythonConveyor : public QObject
{
    Q_OBJECT

public:
    /*!
     * \brief PythonConveyor: Constructor with parent pointer.
     *
     * \param parent - pointer to the parent object.
     */
    explicit PythonConveyor( QObject* parent = nullptr );

    /*!
     * \brief PythonConveyor: Constructor specifying the path to the Python file and the name of the function.
     *
     * \param pythonFilePath - path to the Python file on system.
     * \param functionName - name of the function in python file.
     * \param parent - pointer to the parent object.
     */
    PythonConveyor( const QString& pythonFilePath, const QString& functionName, QObject* parent = nullptr );

    /*!
     * \brief convertVectorToStringList: Converts vector<double> to QStringList.
     *
     * \param inputVector - vector of values of double type.
     *
     * \return A list of strings.
     */
    QStringList convertVectorToStringList( const std::vector< double >& inputVector );

    /*!
     * \brief convertVectorToQString: Converts QVector<double> to QString.
     *
     * \param vector - vector of values of the double type.
     *
     * \return The string representation of the vector.
     */
    QString convertVectorToQString( const QVector<double>& vector );

    /*!
     * \brief convertPyObjectToQVector: Converts PyObject to QVector<double>.
     *
     * \param pyList - pointer to Python list object.
     *
     * \return Vector of double values.
     */
    QVector<double> convertPyObjectToQVector( PyObject* pyList );


    /*!
     * \brief sendArraysToPythonFunction: Sends lists of strings of numbers to functions in python file.
     */
    void sendArraysToPythonFunction();

    /*!
     * \brief getResourceFilePath: Returns absolute path from resource file path.
     *
     * \param resourcePath - path to resource file.
     *
     * \return Absolute file path in system.
     */
    QString getResourceFilePath( const QString& resourcePath );


    /*!
     * \brief sendDataToDifferentiation: Sends data to differentiation function.
     */
    void sendDataToDifferentiation();

    /*!
     * \brief sendDataToIntegration: Sends data to integration function.
     */
    void sendDataToIntegration();

    /*!
     * \brief sendDataToSolveSys: Sends data to solve a system of equations.
     */
    void sendDataToSolveSys();

    void sendDataToSolveNonLinearSys();

    /*!
     * \brief initPythonInterpreter: Initializes the Python interpreter.
     */
    void initPythonInterpreter();


    /*!
     * \brief getPythonFunction: Checking if function is callable.
     *
     * \param functionName - name of the function in python file.
     *
     * \return Pointer to function in python file.
     */
    PyObject* getPythonFunction(const QString& functionName);

    /*!
     * \brief callPythonFunction: Calls a python function with arguments.
     *
     * \param function - pointer to function in python file.
     * \param args - arguments for calling function.
     *
     * \return Pointer to result of executing a function.
     */
    PyObject* callPythonFunction( PyObject* function, PyObject* args );

    /*!
     * \brief buildPyListFromQStringList: Builds python list from QStringList.
     *
     * \param stringList - list of strings.
     *
     * \return Pointer to python list object.
     */
    PyObject* buildPyListFromQStringList( const QStringList &stringList );

    /*!
     * \brief buildPyListFromStdVector: Builds python list from std::vector<double>.
     *
     * \param vector - vector of values of the double type.
     *
     * \return Pointer to python list object.
     */
    PyObject* buildPyListFromStdVector( const std::vector< double >& vector );

    /*!
     * \brief setData: Sets the value to a class member.
     *
     * \param member - pointer to a class member.
     * \param value - value to set.
     */
    template<typename T>
    void setData( T PythonConveyor::* member, const T& value )
    {
        this->*member = value;
    }

    /*!
     * \brief getData: Gets the value of a class member.
     *
     * \param member - pointer to a class member.
     *
     * \return Value of a class member.
     */
    template<typename T>
    T getData( T PythonConveyor::* member ) const
    {
        return this->*member;
    }

private:
    QString pythonFilePath; ///< Path to pyhton file.
    QString functionName; ///< Name of the function in pyhton file.
    QString resultString; ///< Result in string format.
    QString functionToDiff; ///< Name of the function for differentiation.
    QString functionToIntegration; ///< Name of the function for integration.
    QString nonLinearSys;

    double startNumToIntegration; ///< Start value for integration.
    double endNumToIntegration; ///< Final value for integration.
    double startNumToDiff; ///< Start value for differentiation.
    double endNumToDiff; ///< Final value for differentiation.
    double resultValue; ///< Result in double format.

    std::vector<double> xVector; ///< Vector of x values of points.
    std::vector<double> yVector; ///< Vector of y values of points.
    std::vector<double> numVector; ///< Vector of nums.

    PyObject* module; ///< Pointer to python file.
    PyObject* globals; ///< Pointer to global context needed for python initialization.
    PyObject* resultPyObj; ///< Pointer to result from execution of function in python file.

    QVector<double> result_Vector; ///< Result vector.
    QVector<double> resultDiff_XVector; ///< Result vector of x values of points from differentiation.
    QVector<double> resultDiff_YVector; ///< Result vector of y values of points from differentiation.
    QVector<double> resultSys_Vector; ///< Result vector from solving system of equations.
    QVector<double> resultNonLinearSys_Vector;

    QVector<QVector<double>> sys; ///< System of simple equations.

    /*!
     * \brief isResourcePath: Checks if path is resource.
     *
     * \param path - path to check.
     *
     * \return true, if path is resource, else false.
     */
    bool isResourcePath( const QString& path );

    friend class RightWidget;
};

#endif // PYTHONCONVEYOR_H

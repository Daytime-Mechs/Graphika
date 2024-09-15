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
    QString sendArraysToPythonFunction( const QString& pythonFilePath, const QString& functionName, const std::vector<double>& xVector, const std::vector<double>& yVector );

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
    std::pair< QVector<double>, QVector<double> > sendDataToDifferentiation( const QString& pythonFilePath, const QString& functionName, const QVector<double>& xVector, const QVector<double>& yVector );

    /*!
     * \brief sendDataToIntegration: Sends data to integration function.
     */
    QString sendDataToIntegration( const QString& pythonFilePath, const QString& functionName, const QVector<double>& xVector, const QVector<double>& yVector );
    /*!
     * \brief sendDataToSolveSys: Sends data to solve a system of equations.
     */
    QString sendDataToSolveSys( const QString& pythonFilePath, const QString& functionName, const QVector< QVector<double> >& sys );

    /*!
     * \brief initPythonInterpreter: Initializes the Python interpreter.
     */
    void initPythonInterpreter( const QString& pythonFilePath );


    /*!
     * \brief getPythonFunction: Checking if function is callable.
     *
     * \param functionName - name of the function in python file.
     *
     * \return Pointer to function in python file.
     */
    PyObject* getPythonFunction( const QString& functionName );

    /*!
     * \brief callPythonFunction: Calls a python function with arguments.
     *
     * \param function - pointer to function in python file.
     * \param args - arguments for calling function.
     *
     * \return Pointer to result of executing a function.
     */
    PyObject* callPythonFunction( PyObject* function, PyObject* args, const QString& functionName );

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

private:
    PyObject* module; ///< Pointer to python file.
    PyObject* globals; ///< Pointer to global context needed for python initialization.
    PyObject* resultPyObj; ///< Pointer to result from execution of function in python file.
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

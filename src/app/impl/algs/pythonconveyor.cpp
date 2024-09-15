/*!
 * \class PythonConveyor.
 *
 * \brief The PythonConveyor class for interacting with Python From C++.
 *
 * \author Korshunov Ilya Dmitrievich, Dnevnie Mechaniki.
 *
 * \date last update: 20.05.2024.
 */

#include "pythonconveyor.h"
#include <Python.h>
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QResource>
#include <QTemporaryFile>
#include <QDir>
#include <QTemporaryDir>
#include <QFileDialog>


PythonConveyor::PythonConveyor( QObject* parent ) : QObject( parent ) {}

PythonConveyor::PythonConveyor( const QString& pythonFilePath, const QString& functionName, QObject* parent )
    : QObject( parent ) {}


/*!
 * \brief PythonConveyor::convertVectorToStringList: Converts a vector of doubles to a QStringList.
 *
 * \param inputVector - the input vector of doubles.
 *
 * \return QStringList containing the string representations of the vector elements.
 */
QStringList PythonConveyor::convertVectorToStringList( const std::vector< double >& inputVector )
{
    QStringList stringList;
    for ( double value : inputVector )
    {
        stringList.append( QString::number( value ) );
    }
    return stringList;
}

/*!
 * \brief PythonConveyor::isResourcePath: Checks if a given path is a Qt resource path.
 *
 * \param path - the path to check.
 *
 * \return True if the path is resource path, false otherwise.
 */
bool PythonConveyor::isResourcePath( const QString& path )
{
    return path.startsWith( ":/" );
}

/*!
 * \brief PythonConveyor::getResourceFilePath: Retrieves the absolute file path for a given resource path.
 *
 * \param resourcePath - the resource path.
 *
 * \return The absolute file path, or an empty QString if the resource does not exist.
 */
QString PythonConveyor::getResourceFilePath( const QString& resourcePath )
{
    if ( !QFile::exists( resourcePath ) )
    {
        qDebug() << "Resource file does not exist: " << resourcePath;
        return QString();
    }

    QString absoluteFilePath = QFileInfo( resourcePath ).absoluteFilePath();

    return absoluteFilePath;
}

/*!
 * \brief PythonConveyor::callPythonFunction: Calls a python function with the specified arguments.
 *
 * \param function - the python function to call.
 * \param args - the arguments to pass to the python function.
 *
 * \return The result of the python function call, or nullptr if the call failed.
 */
PyObject* PythonConveyor::callPythonFunction( PyObject* function, PyObject* args, const QString& functionName )
{
    PyObject* result = PyObject_CallObject( function, args );
    if ( !result )
    {
        qDebug() << "Failed to call function" << functionName;
        PyErr_Print();
    }
    return result;
}

/*!
 * \brief PythonConveyor::buildPyListFromQStringList: Builds a python list from QStringList.
 *
 * \param stringList - the input QStringList.
 *
 * \return A python list containing the elements of the QStringList.
 */
PyObject* PythonConveyor::buildPyListFromQStringList( const QStringList &stringList )
{
    PyObject* pyList = PyList_New( stringList.size() );
    for ( int i = 0; i < stringList.size(); ++i )
    {
        PyList_SetItem( pyList, i, Py_BuildValue( "s", stringList[i].toStdString().c_str() ) );
    }
    return pyList;
}

/*!
 * \brief PythonConveyor::buildPyListFromStdVector: Builds a python list from a std::vector of doubles.
 *
 * \param vector - the input vector of doubles.
 *
 * \return A python list containing the elements of the vector.
 */
PyObject* PythonConveyor::buildPyListFromStdVector( const std::vector< double >& vector )
{
    PyObject* pyList = PyList_New( vector.size() );
    for ( int i = 0; i < vector.size(); ++i )
    {
        PyList_SetItem( pyList, i, PyFloat_FromDouble( vector[i] ) );
    }
    return pyList;
}

/*!
 * \brief PythonConveyor::convertPyObjectToQVector: Converts a python list to a QVector of doubles.
 *
 * \param pyList - the python list to convert.
 *
 * \return A QVector containing the elements of the python list.
 */
QVector<double> PythonConveyor::convertPyObjectToQVector( PyObject* pyList )
{
    QVector<double> result;
    int size = PyList_Size( pyList );
    for ( int i = 0; i < size; ++i )
    {
        PyObject* item = PyList_GetItem( pyList, i );
        double value = PyFloat_AsDouble( item );
        result.append( value );
    }
    return result;
}

/*!
 * \brief PythonConveyor::convertVectorToQString: Converts a QVector of doubles to a QString.
 *
 * \param vector - the input QVector of doubles.
 *
 * \return A QString containing the string representations of the vector elements.
 */
QString PythonConveyor::convertVectorToQString( const QVector<double>& vector )
{
    QString result;
    for ( double value : vector )
    {
        result += QString::number( value ) + " ";
    }
    return result;
}

/*!
 * \brief PythonConveyor::sendArraysToPythonFunction: Sends x and y data arrays to a python function.
 */
QString PythonConveyor::sendArraysToPythonFunction( const QString& pythonFilePath, const QString& functionName, const std::vector<double>& xVector, const std::vector<double>& yVector )
{
    initPythonInterpreter( pythonFilePath );
    PyObject* function = getPythonFunction( functionName );
    if ( !function )
    {
        return QString();
    }
    PyObject* args = PyTuple_New( 2 );
    QStringList xStringList = convertVectorToStringList( xVector );
    QStringList yStringList = convertVectorToStringList( yVector );
    PyObject* xList = buildPyListFromQStringList( xStringList );
    PyObject* yList = buildPyListFromQStringList( yStringList );
    PyTuple_SetItem( args, 0, xList );
    PyTuple_SetItem( args, 1, yList );
    PyObject* pyResult = callPythonFunction( function, args, functionName );
    QString resultString;
    if ( pyResult )
    {
        resultString = PyUnicode_AsUTF8( pyResult );
        Py_DECREF( pyResult );
    }
    Py_DECREF( args );
    Py_DECREF( function );
    Py_Finalize();
    return resultString;
}

/*!
 * \brief PythonConveyor::sendDataToIntegration: Sends data for integration to a python function.
 */
QString PythonConveyor::sendDataToIntegration( const QString& pythonFilePath, const QString& functionName, const QVector<double>& xVector, const QVector<double>& yVector )
{
    initPythonInterpreter( pythonFilePath );
    PyObject* function = getPythonFunction( functionName );
    if ( !function )
    {
        return QString();
    }
    PyObject* args = PyTuple_New( 2 );
    PyObject* xList = buildPyListFromStdVector( xVector.toStdVector() );
    PyObject* yList = buildPyListFromStdVector( yVector.toStdVector() );
    PyTuple_SetItem( args, 0, xList );
    PyTuple_SetItem( args, 1, yList );
    PyObject* pyResult = callPythonFunction( function, args, functionName );
    QString resultString;
    if ( pyResult )
    {
        double resultValue = PyFloat_AsDouble( pyResult );
        resultString = QString::number( resultValue );
        Py_DECREF( pyResult );
    }
    Py_DECREF( args );
    Py_DECREF( function );
    Py_Finalize();
    return resultString;
}

/*!
 * \brief PythonConveyor::sendDataToDifferentiation: Sends data for differentiation to a python function.
 */
std::pair< QVector<double>, QVector<double> > PythonConveyor::sendDataToDifferentiation( const QString& pythonFilePath, const QString& functionName, const QVector<double>& xVector, const QVector<double>& yVector )
{
    initPythonInterpreter( pythonFilePath );
    PyObject* function = getPythonFunction( functionName );
    if ( !function )
    {
        return std::make_pair( QVector<double>(), QVector<double>() );
    }
    PyObject* args = PyTuple_New( 2 );
    PyObject* xList = buildPyListFromStdVector( xVector.toStdVector() );
    PyObject* yList = buildPyListFromStdVector( yVector.toStdVector() );
    PyTuple_SetItem( args, 0, xList );
    PyTuple_SetItem( args, 1, yList );
    PyObject* pyResult = callPythonFunction( function, args, functionName );
    std::pair< QVector<double>, QVector<double> > result;
    if ( pyResult )
    {
        if ( PyTuple_Check( pyResult ) && PyTuple_Size( pyResult ) == 2 )
        {
            PyObject* xResult = PyTuple_GetItem( pyResult, 0 );
            PyObject* yResult = PyTuple_GetItem( pyResult, 1 );
            result.first = convertPyObjectToQVector( xResult );
            result.second = convertPyObjectToQVector( yResult );
            Py_DECREF( xResult );
            Py_DECREF( yResult );
        }
        Py_DECREF( pyResult );
    }
    Py_DECREF( args );
    Py_DECREF( function );
    Py_Finalize();
    return result;
}

/*!
 * \brief PythonConveyor::sendDataToSolveSys: Sends data to solve a system of equations to a python function.
 */
QString PythonConveyor::sendDataToSolveSys( const QString& pythonFilePath, const QString& functionName, const QVector< QVector<double> >& sys )
{
    initPythonInterpreter( pythonFilePath );
    PyObject* function = getPythonFunction( functionName );
    if ( !function )
    {
        return QString();
    }
    PyObject* args = PyTuple_New( 1 );
    PyObject* dataList = PyList_New( sys.size() );
    for ( int i = 0; i < sys.size(); ++i )
    {
        PyObject* sublist = PyList_New( sys[i].size() );
        for ( int j = 0; j < sys[i].size(); ++j )
        {
            PyList_SetItem( sublist, j, PyFloat_FromDouble( sys[i][j] ) );
        }
        PyList_SetItem( dataList, i, sublist );
    }
    PyTuple_SetItem( args, 0, dataList );
    PyObject* pyResult = callPythonFunction( function, args, functionName );
    QString resultString;
    if ( pyResult )
    {
        if ( PyList_Check( pyResult ) )
        {
            QVector<double> resultList;
            int size = PyList_Size( pyResult );
            for ( int i = 0; i < size; ++i )
            {
                PyObject* item = PyList_GetItem( pyResult, i );
                double value = PyFloat_AsDouble( item );
                resultList.append( value );
                resultString += QString::number( value ) + " ";
            }
        }
        Py_DECREF( pyResult );
    }
    Py_DECREF( args );
    Py_DECREF( function );
    Py_Finalize();
    return resultString;
}

/*!
 * \brief PythonConveyor::initPythonInterpreter: Initializes the Python interpreter and executes the Python script.
 */
void PythonConveyor::initPythonInterpreter( const QString& pythonFilePath )
{
    if ( !Py_IsInitialized() )
    {
        Py_Initialize();
    }

    QString absoluteFilePath = isResourcePath( pythonFilePath ) ? getResourceFilePath( pythonFilePath ) : pythonFilePath;

    QFile file( absoluteFilePath );
    if ( !file.exists() )
    {
        qDebug() << "Python file does not exist: " << absoluteFilePath;
        Py_Finalize();
        return;
    }

    if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        qDebug() << "Failed to open Python file: " << absoluteFilePath;
        Py_Finalize();
        return;
    }

    QByteArray byteArray = file.readAll();
    module = PyImport_AddModule("__main__");
    globals = PyModule_GetDict( module );
    resultPyObj = PyRun_String( byteArray.constData(), Py_file_input, globals, globals );
    if ( !resultPyObj )
    {
        qDebug() << "Failed to execute Python script: " << absoluteFilePath;
        PyErr_Print();
        file.close();
        Py_Finalize();
        return;
    }

    file.close();
}

/*!
 * \brief PythonConveyor::getPythonFunction: Retrieves a python function by name.
 *
 * \param functionName - the name of the python function to retrieve.
 *
 * \return A PyObject representing the Python function, or nullptr if the function does not exist or is not callable.
 */
PyObject* PythonConveyor::getPythonFunction( const QString &functionName )
{
    PyObject* function = PyObject_GetAttrString( module, functionName.toStdString().c_str() );
    if ( !function || !PyCallable_Check( function ) )
    {
        qDebug() << "Function" << functionName << "is not callable or does not exist";
        Py_Finalize();
        return nullptr;
    }
    return function;
}

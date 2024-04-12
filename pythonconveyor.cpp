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
    : QObject( parent ), m_pythonFilePath( pythonFilePath ), m_functionName( functionName ) {}

void PythonConveyor::setPythonFilePath( const QString& pythonFilePath )
{
    m_pythonFilePath = pythonFilePath;
}

QString PythonConveyor::getPythonFilePath() const
{
    return m_pythonFilePath;
}

void PythonConveyor::setFunctionName( const QString& functionName )
{
    m_functionName = functionName;
}

QString PythonConveyor::getFunctionName() const
{
    return m_functionName;
}

void PythonConveyor::setPrecision( double precision )
{
    m_precision = precision;
}

double PythonConveyor::getPrecision() const
{
    return m_precision;
}

void PythonConveyor::setDataX( const std::vector< double >& vector )
{
    m_xVector = vector;
}

std::vector< double > PythonConveyor::get_X_Vector() const
{
    return m_xVector;
}

void PythonConveyor::setDataY( const std::vector< double >& vector )
{
    m_yVector = vector;
}

std::vector< double > PythonConveyor::get_Y_Vector() const
{
    return m_yVector;
}

void PythonConveyor::setResult( const QString& result )
{
    m_result = result;
}

QString PythonConveyor::getResult() const
{
    return m_result;
}

std::vector< double > PythonConveyor::get_Nums_Vector() const
{
    return m_numVector;
}

void PythonConveyor::setDataNums( const std::vector< double > &vector )
{
    m_numVector = vector;
}

void PythonConveyor::setFunctionToDiff( const QString& expression )
{
    m_functionToDiff = expression;
}

QString PythonConveyor::getFunctionToDiff() const
{
    return m_functionToDiff;
}

void PythonConveyor::setFunctionToIntegration(const QString &expression)
{
    m_functionToIntegration = expression;
}
QString PythonConveyor::getFunctionToIntegration() const
{
    return m_functionToIntegration;
}

void PythonConveyor::setStartNumToIntegration(double startNumToIntegration)
{
    m_startNumToIntegration = startNumToIntegration;
}

double PythonConveyor::getStartNumToIntegration() const
{
    return m_startNumToIntegration;
}

void PythonConveyor::setEndNumToIntegration(double endNumToIntegration)
{
    m_endNumToIntegration = endNumToIntegration;
}

double PythonConveyor::getEndNumToIntegration() const
{
    return m_endNumToIntegration;
}

void PythonConveyor::setStartNumToDiff(double startNumToDiff)
{
    m_startNumToDiff = startNumToDiff;
}

double PythonConveyor::getStartNumToDiff() const
{
    return m_startNumToDiff;
}

void PythonConveyor::setEndNumToDiff(double endNumToDiff)
{
    m_endNumToDiff = endNumToDiff;
}

double PythonConveyor::getEndNumToDiff() const
{
    return m_endNumToDiff;
}

QStringList PythonConveyor::convertVectorToStringList( const std::vector< double >& inputVector )
{
    QStringList stringList;
    for ( double value : inputVector )
    {
        stringList.append( QString::number( value ) );
    }
    return stringList;
}

bool PythonConveyor::isResourcePath( const QString& path )
{
    return path.startsWith( ":/" );
}

QString PythonConveyor::getResourceFilePath( const QString& resourcePath )
{
    // Проверяем, существует ли ресурсный файл
    if ( !QFile::exists( resourcePath ) ) {
        qDebug() << "Resource file does not exist: " << resourcePath;
        return QString(); // Возвращаем пустую строку в случае ошибки
    }

    // Получаем абсолютный путь к ресурсному файлу
    QString absoluteFilePath = QFileInfo( resourcePath ).absoluteFilePath();

    // Возвращаем абсолютный путь к ресурсному файлу
    return absoluteFilePath;
}

void PythonConveyor::sendArraysToPythonFunction()
{
    initPythonInterpreter();

    PyObject* function = getPythonFunction(m_functionName);
    if (!function)
    {
        return;
    }

    // Подготовка аргументов для вызова функции Python
    PyObject* args = PyTuple_New( 2 );

    // Преобразование векторов чисел в вектора строк
    QStringList xStringList = convertVectorToStringList(m_xVector);
    QStringList yStringList = convertVectorToStringList(m_yVector);

    // Создание списков Python для значений x и y
    PyObject* xList = PyList_New(xStringList.size());
    PyObject* yList = PyList_New(yStringList.size());

    // Заполнение списков значениями из xStringList и yStringList
    for (int i = 0; i < xStringList.size(); ++i) {
        PyList_SetItem(xList, i, Py_BuildValue("s", xStringList[i].toStdString().c_str()));
    }
    for (int i = 0; i < yStringList.size(); ++i) {
        PyList_SetItem(yList, i, Py_BuildValue("s", yStringList[i].toStdString().c_str()));
    }

    // Установка списков в аргументы кортежа
    PyTuple_SetItem(args, 0, xList);
    PyTuple_SetItem(args, 1, yList);
    //PyTuple_SetItem( args, 2, Py_BuildValue( "f", m_precision ) );

    // Вызов функции Python
    PyObject* pyResult = PyObject_CallObject( function, args );
    if ( !pyResult ) {
        qDebug() << "Failed to call function" << m_functionName;
        PyErr_Print();
    } else {
        // Обработка результата, если это необходимо
        setResult( PyUnicode_AsUTF8( pyResult ) );
        Py_DECREF( pyResult );
    }

    // Освобождение ресурсов
    Py_DECREF( args );
    Py_DECREF( function );
    Py_DECREF( result );

    // Финализация интерпретатора Python
    Py_Finalize();
}

void PythonConveyor::sendDataToIntegration()
{
    initPythonInterpreter();

    PyObject* function = getPythonFunction(m_functionName);
    if (!function)
    {
        return;
    }

    // Подготовка аргументов для вызова функции Python
    PyObject* args = PyTuple_New( 3 );

    //Установка строки функции в аргументы
    PyTuple_SetItem( args, 0, Py_BuildValue( "s", m_functionToIntegration.toUtf8().constData() ) );

    // Установка начального числа интегрирования в аргументы кортежа
    PyTuple_SetItem(args, 1, Py_BuildValue("d", m_startNumToIntegration));

    // Установка конечного числа интегрирования в аргументы кортежа
    PyTuple_SetItem(args, 2, Py_BuildValue("d", m_endNumToIntegration));

    // Вызов функции Python
    PyObject* pyResult = PyObject_CallObject( function, args );
    if ( !pyResult ) {
        qDebug() << "Failed to call function" << m_functionName;
        PyErr_Print();
    } else {
        // Обработка результата, если это необходимо
        double resultValue = PyFloat_AsDouble(pyResult); // Преобразование результата в число
        setResultValue(resultValue); // Установка результата в поле класса

        // Преобразование результата в строку для вывода в консоль
        QString resultString = QString::number(resultValue);
        qDebug() << "Integration result: " << resultString;
        setResult(resultString);

        Py_DECREF( pyResult );
    }

    // Освобождение ресурсов
    Py_DECREF( args );
    Py_DECREF( function );
    Py_DECREF( result );

    // Финализация интерпретатора Python
    Py_Finalize();
}

void PythonConveyor::sendDataToDifferentiation()
{
    initPythonInterpreter();

    PyObject* function = getPythonFunction(m_functionName);
    if (!function)
    {
        return;
    }

    // Подготовка аргументов для вызова функции Python
    PyObject* args = PyTuple_New( 2 );

    PyObject* numList = PyList_New(m_numVector.size());

    for ( int i = 0; i < m_numVector.size(); ++i )
    {
        PyList_SetItem(numList, i, PyFloat_FromDouble(m_numVector[i]));
    }


    //Установка строки функции в аргументы
    PyTuple_SetItem( args, 0, Py_BuildValue( "s", m_functionToDiff.toUtf8().constData() ) );

    PyTuple_SetItem( args, 1, numList);

    // Вызов функции Python
    PyObject* pyResult = PyObject_CallObject( function, args );
    if ( !pyResult ) {
        qDebug() << "Failed to call function" << m_functionName;
        PyErr_Print();
    } else {
        // Преобразование списка в вектор чисел
        if (PyList_Check(pyResult)) {
            int size = PyList_Size(pyResult);
            QVector<double> resultList;
            QString resultString;
            for (int i = 0; i < size; ++i) {
                PyObject* item = PyList_GetItem(pyResult, i);
                // Получение числового значения элемента списка
                double value = PyFloat_AsDouble(item);
                resultList.append(value);
                // Добавление значения в строку для вывода в консоль
                resultString += QString::number(value) + " ";
            }
            // Вывод результата в консоль как строку
            qDebug() << "Differentiation result: " << resultString;
            // Установка результирующего вектора чисел в поле класса
            setResultVector(resultList);
            setResult(resultString);
        }
        Py_DECREF( pyResult );
    }

    // Освобождение ресурсов
    Py_DECREF( args );
    Py_DECREF( function );
    Py_DECREF( result );

    // Финализация интерпретатора Python
    Py_Finalize();
}

void PythonConveyor::setResultVector(const QVector<double>& resultVector)
{
    m_resultVector = resultVector;
}

QVector<double> PythonConveyor::getResultVector() const{
    return m_resultVector;
}

void PythonConveyor::setResultValue(const double resultValue)
{
    m_resultValue = resultValue;
}

double PythonConveyor::getResultValue() const
{
    return m_resultValue;
}

void PythonConveyor::initPythonInterpreter()
{
    Py_Initialize();

    QString absoluteFilePath = isResourcePath( m_pythonFilePath ) ? getResourceFilePath( m_pythonFilePath ) : m_pythonFilePath;

    QFile file( absoluteFilePath );
    if ( !file.exists() ) {
        qDebug() << "Python file does not exist: " << absoluteFilePath;
        Py_Finalize();
        return;
    }

    if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
        qDebug() << "Failed to open Python file: " << absoluteFilePath;
        Py_Finalize();
        return;
    }

    QByteArray byteArray = file.readAll();
    module = PyImport_AddModule( "__main__" );
    globals = PyModule_GetDict( module );
    result = PyRun_String( byteArray.constData(), Py_file_input, globals, globals );
    if ( !result ) {
        qDebug() << "Failed to execute Python script: " << absoluteFilePath;
        PyErr_Print();
        file.close();
        Py_Finalize();
        return;
    }

    file.close();
    Py_DECREF(result);
}

PyObject* PythonConveyor::getPythonFunction(const QString &functionName)
{
    PyObject* function = PyObject_GetAttrString(module, functionName.toStdString().c_str());
    if (!function || !PyCallable_Check(function)) {
        qDebug() << "Function" << functionName << "is not callable or does not exist";
        Py_DECREF(result);
        Py_Finalize();
        return nullptr;
    }
    return function;
}

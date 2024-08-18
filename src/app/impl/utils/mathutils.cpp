#include "mathutils.h"


std::vector<double> MathUtils::multiplyPoints( const double a, const double b ) noexcept
{
    std::vector<double> x;
    for( double i = a; i <= b; i += 0.02 )
    {
        x.push_back( i );
    }
    if( x.back() != b )
    {
        x.push_back( b );
    }
    return x;
}

void MathUtils::setupNodes( std::vector<double>& data, const double nodes, const double min, const double max ) noexcept
{
    double step = ( max - min ) / ( nodes - 1 );
    for( auto i = 0; i < nodes; ++i )
    {
        double node = min + i * step;
        data.push_back( node );
    }
}

double MathUtils::calculateAverageError( std::vector<double> &X1, std::vector<double> &X2 ) noexcept
{
    if( X1.size() == 0 || X2.size() == 0 )
    {
        return 0.0;
    }

    double error = 0.0;
    for( std::size_t i = 0; i < X1.size(); ++i )
    {
        error += abs( X1[i] - X2[i] );
    }
    error /= X1.size();
    return error;
}

QVector<QVector<double>> MathUtils::formTheSystemOfEquations( const QTableWidget& table )
{
    QVector<QVector<double>> result;

    for( int row = 0; row < table.rowCount(); ++row )
    {
        QVector<double> rowValues;

        QTableWidgetItem* firstColumnItem = table.item( row, 0 );
        QTableWidgetItem* secondColumnItem = table.item( row, 1 );

        if( firstColumnItem == nullptr || secondColumnItem == nullptr )
        {
            continue;
        }

        QString firstColumnData = firstColumnItem->text();
        QStringList firstColumnValues = firstColumnData.split( " " );

        foreach( const QString& value, firstColumnValues )
        {
            rowValues.append( value.toDouble() );
        }

        QString secondColumnData = secondColumnItem->text();
        rowValues.append( secondColumnData.toDouble() );

        result.append( rowValues );
    }

    return result;
}

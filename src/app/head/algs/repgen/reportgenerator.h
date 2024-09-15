/*!
 * \attention In development.
 *
 * \author Malaniya Mark Timurovich, Dnevnie Mechaniki.
 *
 * \date last update: 15.09.2024.
 */

#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include <QApplication>
#include <QTime>
#include <QString>
#include <QTableWidget>
#include <QLineEdit>
#include <QPdfWriter>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextTable>
#include <QTextTableFormat>
#include <type_traits>
#include <qcustomplot/qcustomplot.h>
#include <QPrinter>
#include <QFileInfo>
#include <QDebug>
#include <QPainter>
#include "loglist.h"

/*!
 * \class ReportGenerator.
 *
 * \brief A class whose task is to create and save a PDF report in FS.
 */
class ReportGenerator
{
public:
    /*!
     * \brief generateReport: generating PDF report in file system
     *
     * \param logList list of plot's
     * \param title
     * \param authors
     * \param elapsedTime
     * \param fileName
     */
    void generateReport( const LogList& logList, const QString& title, const QString& authors, const QTime& elapsedTime, const QString& fileName );
};

#endif // REPORTGENERATOR_H

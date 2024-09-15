#include "reportgenerator.h"
#include <QPdfWriter>
#include <QPainter>


void ReportGenerator::generateReport( const LogList& logList, const QString& title, const QString& authors, const QTime& elapsedTime, const QString& fileName )
{
    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setPageOrientation(QPageLayout::Portrait);
    pdfWriter.setResolution(300);
    QPainter painter(&pdfWriter);
    painter.setRenderHint(QPainter::Antialiasing);

    // title
    QPixmap logo(":/toolbaricons/resources/logo.PNG");
    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.fillRect(0, 0, pdfWriter.width(), pdfWriter.height(), Qt::white);
    painter.setPen(Qt::black);

    int logoWidth = 1000;
    int logoHeight = 1000;
    int logoX = pdfWriter.width() / 3 - 1;
    int logoY = pdfWriter.height() / 3 - 1;
    painter.drawPixmap(logoX, logoY, logoWidth, logoHeight, logo);

    int textY = logoY + logoHeight + 60;
    painter.drawText(pdfWriter.width() / 3, textY, title);
    painter.setFont(QFont("Arial", 12));
    painter.drawText(pdfWriter.width() / 3, textY + 30, !authors.isEmpty() ? "работы выполнила(-а)(-и): " + authors : "");

    pdfWriter.newPage();

    int pageCount = 1;
    int pageWidth = pdfWriter.width();
    int pageHeight = pdfWriter.height();
    int currentY = 0;

    for (const auto& item : logList.getItems())
    {
        switch (item.item)
        {
        case LogList::ItemType::QCustomPlot: {
            GraphBuilder* customPlot = static_cast<GraphBuilder*>(item.gbd);

            QRect plotRect = customPlot->graph2d->rect();

            qreal scaleX = pdfWriter.width() / static_cast<qreal>(plotRect.width());
            qreal scaleY = pdfWriter.height() / static_cast<qreal>(plotRect.height());
            qreal scale = qMin(scaleX, scaleY);

            int graphWidth = plotRect.width() * scale / 1.5;
            int graphHeight = plotRect.height() * scale / 1.5;
            int graphX = (pageWidth - graphWidth) / 2;
            int graphY = currentY;

            if (currentY + graphHeight > pageHeight)
            {
                currentY = 0;
                pageCount++;
                painter.setFont(QFont("Arial", 12));
                painter.drawText((pageWidth - 20) / 2, pageHeight - 10, QString::number(pageCount));
                pdfWriter.newPage();
                graphY = currentY;
            }

            painter.save();
            painter.translate(graphX, graphY);
            painter.scale(scale / 1.5, scale / 1.5);

            customPlot->render(&painter);
            painter.restore();

            currentY += graphHeight + 20;

            break;
        }
        case LogList::ItemType::QString:
        {
            QRect textRect(10, currentY, pageWidth - 20, pageHeight - currentY);

            painter.drawText(textRect, Qt::AlignLeft | Qt::TextWordWrap, *item.str);
            int textHeight = painter.boundingRect(textRect, Qt::TextWordWrap, *item.str).height();

            if (currentY + textHeight > pageHeight)
            {
                currentY = 0;
                pageCount++;
                painter.setFont(QFont("Arial", 12));
                painter.drawText((pageWidth - 20) / 2, pageHeight - 10, QString::number(pageCount));
                pdfWriter.newPage();
                textRect.moveTo(10, currentY);
            }

            painter.drawText(textRect, Qt::AlignLeft | Qt::TextWordWrap, *item.str);
            currentY += textHeight + 20;

            break;
        }
        }
    }
    painter.drawText( 0, currentY, "Общая длительность сессии(м:с:мс): " + elapsedTime.toString("mm:ss.zzz") );
}

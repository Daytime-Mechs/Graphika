#ifndef SESSIONWINDOW_H
#define SESSIONWINDOW_H

#include <QWidget>
#include <QToolBar>
#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QPushButton>
#include <QVBoxLayout>
#include <iostream>
#include <string>
#include <QTime>

/*!
 * \brief The SessionWidget class: generates when session process started
 */
class SessionWidget : public QWidget
{
    Q_OBJECT
private:
    QToolBar* toolbar;
    QPushButton* stopButton;
    QLabel* graphCountLabel;
    QLabel* timeLabel;
    QLabel* statusIcon;
    QTimer* timer;
    QTime sessionStartTime;
    int graphCount;

public:
    SessionWidget(QWidget* parent = nullptr) : QWidget(parent)
    {
        toolbar = new QToolBar(this);

        graphCountLabel = new QLabel("Получено графиков: 0");
        toolbar->addWidget(graphCountLabel);

        timeLabel = new QLabel("00:00:000");
        toolbar->addWidget(timeLabel);

        stopButton = new QPushButton("Закончить сессию");
        toolbar->addWidget(stopButton);

        statusIcon = new QLabel();
        statusIcon->setPixmap( QPixmap( ":/toolbaricons/resources/reddot.png" )
                                  .scaled( 32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation ) );
        toolbar->addWidget(statusIcon);

        connect(stopButton, &QPushButton::clicked, this, &SessionWidget::stopSession);

        timer = new QTimer();
        connect(timer, &QTimer::timeout, this, &SessionWidget::updateTime);

        QVBoxLayout* layout = new QVBoxLayout(this);

        layout->addWidget(toolbar);
    }

    void addGraphToCounter( void ) {
        graphCount++;
        graphCountLabel->setText(QString("Получено графиков: %1").arg(graphCount));
    }

    QToolBar* getToolBar( void ) { return toolbar; }

public slots:
    void startSession() {
        toolbar->show();
        timer->start(10);
        sessionStartTime = QTime::currentTime();
    }

private slots:
    void updateTime() {
        qint64 elapsed = sessionStartTime.msecsTo(QTime::currentTime());
        int ms = elapsed % 1000;
        int s = (elapsed / 1000) % 60;
        int m = (elapsed / 60000);
        timeLabel->setText(QString("%1:%2:%3").arg(m, 2, 10, QLatin1Char('0')).arg(s, 2, 10, QLatin1Char('0')).arg(ms, 3, 10, QLatin1Char('0')));
    }

    void stopSession() {
        timer->stop();
        toolbar->hide();
        QTime currentTime = QTime::currentTime();
        QTime elapsedTime = QTime(0, 0).addMSecs(sessionStartTime.msecsTo(currentTime));
        emit sessionStopped(elapsedTime);
    }

signals:
    void sessionStopped(const QTime& elapsedTime);
};

#endif //SESSIONWINDOW_H

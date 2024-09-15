#ifndef APPEARANCE_H
#define APPEARANCE_H

#include <QDialog>
#include <QVBoxLayout>
#include <QComboBox>
#include <QFontComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QSettings>
#include <QLabel>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include "qdebug.h"

/*!
 * \brief The Appearance class for view dialog window
 */
class Appearance : public QDialog
{
    Q_OBJECT
private:
    QVBoxLayout* layout;
    QComboBox* theme;
    QFontComboBox* font;
    QSpinBox* fontSize;
    QHBoxLayout* buttonLayout;
    QPushButton* apply;
    QPushButton* cancel;
    QString currentStyle;
    QWidget* parent;

public:
    Appearance( QWidget* parent = nullptr );
    void loadSettings( void );

private:
    Qt::GlobalColor setWindowColor( const QString& theme );
    QString loadStyleSheet( const QString& path );
    QString setWindowStyleSheet( const QString& style );
    void setStyleSheet( void );

private slots:
    void applySettings( void );
};

#endif //APPEARANCE_H

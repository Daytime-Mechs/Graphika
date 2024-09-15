/*!
 * \attention In development.
 *
 * \author Malaniya Mark Timurovich, Dnevnie Mechaniki.
 *
 * \date last update: 15.09.2024.
 */

#ifndef GRAPHBAR_H
#define GRAPHBAR_H

#include <QToolBar>

/*!
 * \brief The GraphBar class
 */
class GraphBar : public QToolBar
{
    Q_OBJECT
public:
    bool legendEnabled{ false };

public:
    explicit GraphBar( QWidget* parent = nullptr );
};

#endif //GRAPHBAR_H

#ifndef PLUGINSLISTDIALOG_H
#define PLUGINSLISTDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QListWidget>

class PluginsListDialog : public QDialog {
    Q_OBJECT

public:
    explicit PluginsListDialog(QWidget *parent = nullptr);

private slots:
    void refreshPluginList();

private:
    QListWidget *pluginList;
};

#endif // PLUGINSLISTDIALOG_H

#ifndef PLUGINSLISTDIALOG_H
#define PLUGINSLISTDIALOG_H

#include <QFile>
#include <QDesktopServices>
#include <QDialog>
#include <QLabel>
#include <QWidget>
#include <QListWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QCoreApplication>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QProgressDialog>
#include <QTimer>


class PluginsListDialog : public QDialog {
    Q_OBJECT

public:
    explicit PluginsListDialog(QWidget *parent = nullptr);

private slots:
    void refreshPluginList();
    void onPluginsListReceived( QNetworkReply *reply );
    void onNetworkError( QNetworkReply::NetworkError code );
    void addJsonToList( const QJsonObject &jsonObject, const QString &prefix );
    void addJsonToList( const QJsonArray &jsonArray, const QString &prefix );
    bool fileExists(const QString &filename);
    void addItemToList(const QString &key, const QString &value, const QString &description);
    void downloadFile(const QString &fileName);
    void deleteFile(const QString &fileName);

private:
    void showError(const QString &message);

    QTableWidget *pluginTable;
    QNetworkAccessManager *networkManager;
    QProgressDialog *progressDialog;
};

#endif // PLUGINSLISTDIALOG_H

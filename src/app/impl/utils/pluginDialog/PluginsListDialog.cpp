#include "pluginDialog/PluginsListDialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QProgressDialog>

PluginsListDialog::PluginsListDialog(QWidget *parent) : QDialog(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    pluginTable = new QTableWidget(this);
    pluginTable->setColumnCount(4);
    pluginTable->setHorizontalHeaderLabels(QStringList() << "Файл" << "Описание" << "Статус" << "Действие");
    pluginTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    pluginTable->verticalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    pluginTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(pluginTable);

    QPushButton *refreshButton = new QPushButton("Обновить список", this);
    layout->addWidget(refreshButton);
    connect(refreshButton, &QPushButton::clicked, this, &PluginsListDialog::refreshPluginList);

    setLayout(layout);

    // Создаем сетевой менеджер для отправки запросов
    networkManager = new QNetworkAccessManager(this);
}


void PluginsListDialog::refreshPluginList() {
    QNetworkRequest request(QUrl("http://localhost:8080/files"));  // Замените на ваш IP-адрес и порт

    // Добавляем заголовок авторизации Basic Auth (логин и пароль)
    QString username = "user";  // Укажите ваш логин
    QString password = "5c3ba259-3fb6-4727-a059-a554bf1e4234";  // Укажите сгенерированный пароль
    QString concatenatedCredentials = username + ":" + password;
    QByteArray base64Credentials = concatenatedCredentials.toUtf8().toBase64();
    QString authHeader = "Basic " + QString(base64Credentials);
    request.setRawHeader("Authorization", authHeader.toUtf8());

    // Отправляем GET-запрос с аутентификацией
    QNetworkReply *reply = networkManager->get(request);

    // Подключаем сигналы для обработки ответа
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onPluginsListReceived(reply);
    });
}


void PluginsListDialog::onPluginsListReceived(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);

        pluginTable->setRowCount(0);

        if (jsonDoc.isObject()) {
            // Если получен объект JSON
            QJsonObject jsonObject = jsonDoc.object();
            addJsonToList(jsonObject, "");
        } else if (jsonDoc.isArray()) {
            // Если получен массив JSON
            QJsonArray jsonArray = jsonDoc.array();
            addJsonToList(jsonArray, "");
        } else {
            // Обработка других случаев или некорректного JSON
            showError("Получен некорректный JSON.");
        }
    } else {
        showError(reply->errorString());
    }
    reply->deleteLater();
}

void PluginsListDialog::addJsonToList(const QJsonObject &jsonObject, const QString &prefix) {
    // Рекурсивно обходит объект JSON и добавляет его элементы в список
    for (const QString &key : jsonObject.keys()) {
        QJsonValue value = jsonObject.value(key);
        QString fullKey = prefix.isEmpty() ? key : prefix + "." + key;

        if (value.isObject()) {
            addJsonToList(value.toObject(), fullKey);
        } else if (value.isArray()) {
            addJsonToList(value.toArray(), fullKey);
        } else {
            QString description = jsonObject.contains("description") ? jsonObject.value("description").toString() : "";
            addItemToList(fullKey, value.toString(), description);
        }
    }
}

void PluginsListDialog::addJsonToList(const QJsonArray &jsonArray, const QString &prefix) {
    // Рекурсивно обходит массив JSON и добавляет его элементы в список
    for (int i = 0; i < jsonArray.size(); ++i) {
        QJsonValue value = jsonArray.at(i);
        QString indexKey = prefix.isEmpty() ? QString::number(i) : prefix + "[" + QString::number(i) + "]";

        if (value.isObject()) {
            addJsonToList(value.toObject(), indexKey);
        } else if (value.isArray()) {
            addJsonToList(value.toArray(), indexKey);
        } else {
            QString description = jsonArray.at(i).toObject().contains("description") ? jsonArray.at(i).toObject().value("description").toString() : ""; // Получаем описание
            addItemToList(indexKey, value.toString(), description);
        }
    }
}

void PluginsListDialog::addItemToList(const QString &key, const QString &value, const QString &description) {
    int row = pluginTable->rowCount();
    pluginTable->insertRow(row);

    // Добавляем названия файлов
    QTableWidgetItem *fileItem = new QTableWidgetItem(value);
    pluginTable->setItem(row, 0, fileItem);

    // Добавляем описание файлов
    QTableWidgetItem *descriptionItem = new QTableWidgetItem(description);
    pluginTable->setItem(row, 1, descriptionItem);

    // Добавляем индикатор для скаченных или доступных к скачиванию файлов
    QLabel *indicator = new QLabel();
    QPixmap pixmap = fileExists(value) ? QPixmap(":/pluginDialog/resources/file_thicc.png") : QPixmap(":/pluginDialog/resources/download_thicc.png");
    indicator->setPixmap(pixmap.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    pluginTable->setCellWidget(row, 2, indicator);

    // Добавляем кнопку для скачивания файла, если его нет у клиента
    if (!fileExists(value)) {
        QPushButton *downloadButton = new QPushButton("Скачать", this);
        connect(downloadButton, &QPushButton::clicked, this, [this, value] {
            downloadFile(value);
        });
        pluginTable->setCellWidget(row, 3, downloadButton);
    } else {
        QPushButton *deleteButton = new QPushButton("Удалить", this);
        connect(deleteButton, &QPushButton::clicked, this, [this, value] {
            deleteFile(value);
        });
        pluginTable->setCellWidget(row, 3, deleteButton);
    }
}

void PluginsListDialog::downloadFile(const QString &fileName) {
    QString downloadUrl = QString("http://localhost:8080/download/%1").arg(fileName);
    QNetworkRequest request;
    request.setUrl(QUrl(downloadUrl));

    QString username = "user";
    QString password = "5c3ba259-3fb6-4727-a059-a554bf1e4234";
    QString concatenatedCredentials = username + ":" + password;
    QByteArray base64Credentials = concatenatedCredentials.toUtf8().toBase64();
    QString authHeader = "Basic " + QString(base64Credentials);
    request.setRawHeader("Authorization", authHeader.toUtf8());

    QProgressDialog *progressDialog = new QProgressDialog("Скачивание файла...", "Отмена", 0, 100, this);
    progressDialog->setWindowModality(Qt::WindowModal);
    progressDialog->setValue(0);
    progressDialog->show();

    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::downloadProgress, progressDialog, &QProgressDialog::setValue);
    connect(reply, &QNetworkReply::finished, this, [this, reply, fileName, progressDialog] {
        progressDialog->close();

        if (reply->error() == QNetworkReply::NoError) {
            QByteArray fileData = reply->readAll();

            // Указываем путь к папке resources/web
            QString downloadDirectory = QCoreApplication::applicationDirPath() + "/resources/web";

            // Проверяем, существует ли папка, если нет, создаем ее
            QDir dir(downloadDirectory);
            if (!dir.exists()) {
                dir.mkpath(downloadDirectory);
            }

            QString filePath = downloadDirectory + "/" + fileName;
            qDebug() << "Путь для загрузки файлов:" << filePath;

            QFile file(filePath);
            if (file.open(QIODevice::WriteOnly)) {
                file.write(fileData);
                file.close();
                QMessageBox::information(this, "Скачивание завершено", QString("Файл \"%1\" был успешно скачан.").arg(fileName));
            } else {
                QMessageBox::critical(this, "Ошибка", QString("Не удалось сохранить файл \"%1\".").arg(fileName));
            }
        } else {
            QMessageBox::critical(this, "Ошибка скачивания", reply->errorString());
        }
        reply->deleteLater();
        refreshPluginList();
    });
}

void PluginsListDialog::deleteFile(const QString &fileName) {
    QString downloadDirectory = QCoreApplication::applicationDirPath() + "/resources/web";
    QString filePath = downloadDirectory + "/" + fileName;

    if (!isVisible()) {
        return;
    }

    if (QFile::remove(filePath)) {
        QTimer::singleShot(0, this, &PluginsListDialog::refreshPluginList);
    } else {
        showError("Ошибка при удалении файла.");
    }
}

bool PluginsListDialog::fileExists(const QString &filename) {
    QString downloadDirectory = QCoreApplication::applicationDirPath() + "/resources/web";
    QString filePath = downloadDirectory + "/" + filename;

    return QFile::exists(filePath);
}

void PluginsListDialog::onNetworkError(QNetworkReply::NetworkError code) {
    showError("Не удалось выполнить запрос. Код ошибки: " + QString::number(code));
}

void PluginsListDialog::showError(const QString &message) {
    QMessageBox::critical(this, "Ошибка", message);
}

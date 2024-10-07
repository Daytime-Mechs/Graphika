#include "pluginDialog/PluginsListDialog.h"
#include <QVBoxLayout>
#include <QPushButton>

PluginsListDialog::PluginsListDialog(QWidget *parent) : QDialog(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    pluginList = new QListWidget(this);
    layout->addWidget(pluginList);

    QPushButton *refreshButton = new QPushButton("Обновить список", this);
    layout->addWidget(refreshButton);

    connect(refreshButton, &QPushButton::clicked, this, &PluginsListDialog::refreshPluginList);

    refreshPluginList();
    setLayout(layout);
}

void PluginsListDialog::refreshPluginList() {
    // Здесь будет логика обновления списка плагинов
    pluginList->clear();
}

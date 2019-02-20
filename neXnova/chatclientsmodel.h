#ifndef CHATCLIENTSMODEL_H
#define CHATCLIENTSMODEL_H

#include <QObject>
#include <QPixmap>
#include <QAbstractListModel>

#include "connection.h"

class ChatClientsModel : public QAbstractListModel
{
    enum ChatClientsRoles {
        NameRole = Qt::UserRole + 1
    };
    Q_OBJECT


public:
    explicit ChatClientsModel(connection*conn = nullptr,QObject *parent = nullptr);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
signals:
    void changedd();
public slots:
    void jsonClientsData(QJsonObject data);
    void updateRequest();

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    connection*connptr;
    QJsonArray users;
    QTimer updater;

};

#endif // CHATCLIENTSMODEL_H

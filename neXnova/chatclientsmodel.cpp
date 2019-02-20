#include "chatclientsmodel.h"

ChatClientsModel::ChatClientsModel(connection*conn,QObject *parent)
    : QAbstractListModel(parent),connptr(conn)
{
    connect(&updater,&QTimer::timeout,this,&ChatClientsModel::updateRequest);
    updater.start(10000);
}

int ChatClientsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return users.count();
}

QVariant ChatClientsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= users.count())
        return QVariant();

    const QJsonObject & user = users[index.row()].toObject();
    if (role == NameRole)
        return user["name"].toString();
    return QVariant();
}

void ChatClientsModel::jsonClientsData(QJsonObject data)
{
    QJsonArray usersnewdata = data["users"].toArray();
    QJsonArray newusers;
    QJsonArray disconnectedUsers;

    Q_FOREACH(QJsonValue userToCheck,users){
        if(!usersnewdata.contains(userToCheck)){
            disconnectedUsers.append(userToCheck);
        }
    }
    Q_FOREACH(QJsonValue disuser,disconnectedUsers){
        for(uint i = 0;i<users.count();i++){
            if(users[i]==disuser){
                beginRemoveRows(QModelIndex(),i,i);
                users.removeAt(i);
                endRemoveRows();
                if(users.isEmpty()){
                    beginInsertRows(QModelIndex(),rowCount(),rowCount());
                    QJsonObject obj;//! TODO Implement offline roster
                    obj["name"]="All disconnected";
                    users << obj;
                    endInsertRows();
                }
            }
        }

    }

    Q_FOREACH(QJsonValue oneuser,usersnewdata){
        if(!users.contains(oneuser)){
            newusers.append(oneuser);
        }
    }

    beginInsertRows(QModelIndex(),rowCount(),rowCount() + newusers.count() - 1);
    Q_FOREACH(QJsonValue newvalue,newusers)
        users.append(newvalue);
    endInsertRows();
    emit changedd();

}

void ChatClientsModel::updateRequest()
{
    QMetaObject::invokeMethod(connptr,"requestClientsInfo",Qt::QueuedConnection);
}

QHash<int, QByteArray> ChatClientsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    return roles;
}

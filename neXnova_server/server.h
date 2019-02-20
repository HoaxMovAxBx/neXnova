#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QTcpServer>
#include <QStack>

#include <QSslCertificate>
#include <QSslKey>
#include <QSslConfiguration>
#include <QFile>

#include <QDebug>
#include <QCoreApplication>

#include "client_thread.h"
#include <stdlib.h>


class client_thread;

class server : public QTcpServer
{
    Q_OBJECT
public:
    explicit server(QObject *parent = nullptr);

private:
    QStack<client_thread*>stack;

    void incomingConnection(qintptr socketDescriptor);

signals:
    void serverMsg(QByteArray msg,QString name);

public slots:
    void sendMsg(QString name,QByteArray msg);
    void audioChannelRele(QJsonObject data);
    void removeClientFromStack(client_thread * client);

    QJsonObject connectedClients(const QString user);


};

#endif // SERVER_H

#include "server.h"

Server::Server(QObject *parent) : QThread(parent)
{

}


void Server::setHeader(const QByteArray &data)
{
    header = data;
}

void Server::newConnection()
{
    qDebug() << "New audio client"<<endl;
    QTcpSocket *socket = server->nextPendingConnection();
    connect(socket, &QTcpSocket::disconnected, this, &Server::disconnected);
    sockets.append(socket);
}

void Server::disconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    sockets.removeAll(socket);
    socket->deleteLater();
}

void Server::run()
{
    qDebug() << "Starting server..."<<endl;
    input = new AudioInput;
    server = new QTcpServer;
    connect(server, &QTcpServer::newConnection,
            this, &Server::newConnection,Qt::DirectConnection);
    bool listening = server->listen(QHostAddress::Any, port);

    if (!listening)
        qFatal(server->errorString().toLatin1());

    connect(input,&AudioInput::dataReady,this,&Server::writeData,Qt::DirectConnection);

    input->start(devinfo);

    exec();
}

void Server::writeData(const QByteArray &data)
{
    qDebug() <<"Audio server sending data "<<endl;

    foreach (QTcpSocket *socket, sockets)
    {
#ifdef OPUS
        socket->write(IntToArray(data.size()));
#endif
        socket->write(data);
        socket->flush();
    }
}

QByteArray Server::IntToArray(qint32 value)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << value;
    return data;
}

void Server::initialize(quint16 cport)
{
    devinfo = audioengine::catchOutAsIn();
    port = cport;
}

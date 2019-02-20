#include "server.h"

server::server(QObject *parent) : QTcpServer(parent)
{


    if(!this->listen(QHostAddress::Any,50500)){
        qCritical() << "Could not start " << this->errorString() << endl;
        exit(-1);
    }


}

void server::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "incomming connection "<<endl;

    stack.push(new client_thread(socketDescriptor,this));
    stack.top()->start();

}

void server::sendMsg(QString name, QByteArray msg)
{
    qCritical() << "Sending "<<msg<<" to client "<<name<<endl;
    Q_FOREACH(client_thread * client,stack){
        if(name == client->getName()){
            //emit serverMsg(msg,name);
            QMetaObject::invokeMethod(client,"serverMsgSender",Q_ARG(QByteArray,msg));
            return;
        }
    }
    qWarning() << "Client "<<name<<" not found "<<endl;
}

void server::audioChannelRele(QJsonObject data)
{
    qDebug() << "Audiorele summoned with data "<<data<<endl;

    QString peer = data["peer"].toString();
    QJsonDocument doctop(data);

    Q_FOREACH(client_thread * client,stack){
        if(client->getName() == peer){
            QByteArray msg;
            msg.append("AUDIO CHANNEL REQUEST ");
            msg.append(doctop.toBinaryData());
            msg.append(FOOTER);
            QMetaObject::invokeMethod(client,"serverMsgSender",
                                      Qt::QueuedConnection,Q_ARG(QByteArray,msg));
            qDebug() <<"Audio rele conmutation betw " << peer <<" and "
                     << data["sender"].toString() << endl;
            return;
        }
    }

    QByteArray error;
    error.append("AUDIO CHANNEL ERROR : unable to find client ");
    error.append(peer.toLatin1());
    error.append(FOOTER);

    QMetaObject::invokeMethod(sender(),"serverMsgSender",
                              Qt::QueuedConnection,Q_ARG(QByteArray,error));

}

void server::removeClientFromStack(client_thread *client)
{
    stack.removeOne(client);
}

QJsonObject server::connectedClients(const QString user)
{
    QJsonArray users = QJsonArray();
    Q_FOREACH(client_thread * client,stack){
        if(!client->getName().isEmpty() && client->getName()!=user){
            QJsonObject jsuser;
            jsuser["name"]=client->getName();
            jsuser["avatar"]=client->getAvatar();
            users.append(jsuser);
        }
    }
    QJsonObject jsobj = QJsonObject();
    jsobj["users"]=users;
    return jsobj;
}



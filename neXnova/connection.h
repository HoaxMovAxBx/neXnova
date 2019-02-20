#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QSslSocket>
#include <QThread>
#include <QByteArray>
#include <QCryptographicHash>

#include <QElapsedTimer>
#include <QTimer>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <QDebug>

#define FOOTER ";FAFFAAAA"

class connection : public QThread
{
    Q_OBJECT
public:
    explicit connection();

private:
    void run()override;

    QTcpSocket*sock;
    QByteArray buffer;
    QElapsedTimer * pingpong;
    QTimer * pingtimer;
    QString loggeduser;
    bool islogged;

signals:
    void connectionError(QString cause);

    void waiting();
    void finished();

    void authresultOk();
    void authresultBad();

    void Message(QString msg);

    void pingcompleted(quint32 msec);

    void accountCreated();
    void accountCreationError(quint8 errorcode);
    void accountEmailVerification();

    void chatMsg(QJsonObject msgobj);

    void connectedClientsInfo(QJsonObject clientsinfo);

    void universeData(QJsonObject data);

    void audiochannelconfirm();
    void videochannelconfirm();

    void videochannelrequest(QJsonObject data);
    void audiochannelrequest(QJsonObject data);

public slots:
    bool testConnection(QString address);
    void sendAuthMsg(QString user,QString password);
    void parse();
    void ping();
    void accountCreate(QJsonObject msg);
    void sendMsg(QJsonObject msg);
    void requestClientsInfo();
    void requestSolarSystem(QString id);
    void defineRaze(QString user, QString raze);

    void requestAudioStreamToPeer(QJsonObject formatoanduser);
    void requestVideoStreamToPeer(QString user);

    QString loggedUser() const;



};

#endif // CONNECTION_H

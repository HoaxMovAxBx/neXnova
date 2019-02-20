#ifndef SERVER_H
#define SERVER_H

#include <QtCore>
#include <QtNetwork>
#include "audioinput.h"
#include "audioengine.h"

class AudioInput;
class audioengine;

class Server : public QThread
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);

    static QByteArray IntToArray(qint32 value);

signals:
    void error(QString);

public slots:
    void initialize(quint16 cport);
    void writeData(const QByteArray &data);
    void setHeader(const QByteArray &data);

private slots:
    void newConnection();
    void disconnected();

private:
    QTcpServer *server;
    QList<QTcpSocket*> sockets;
    QByteArray header;
    QAudioDeviceInfo devinfo;
    AudioInput * input;
    uint port;

    void run() override;
};

#endif // SERVER_H

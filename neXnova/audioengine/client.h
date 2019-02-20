#ifndef CLIENT_H
#define CLIENT_H

#include <QtCore>
#include <QtNetwork>
#include "audiooutput.h"
#ifdef OPUS
#include "opusdecode.h"
#endif

class Client : public QThread
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);

    static qint32 ArrayToInt(const QByteArray &data);

signals:
    void error(QString);
    void audioReady(QByteArray);
    void currentlevel(float);

public slots:
    void initialize(const QString chost, uint timetobuffer, uint Port = 9800);
    QAudioFormat currentAudioFormat();
    void setVolume(int vol);
    void preferedFormat(QAudioFormat formatx);

private slots:
    void stateChange(QTcpSocket::SocketState socketState);
    void finishedfunc();
    void readyRead();
    void connectedToPeer();
#ifdef OPUS
    void processInput(const QByteArray &data);
#endif

private:
    QTcpSocket *socket;
    AudioOutput *output;
    QAudioFormat format;
    int volume;
    uint ttb;
    QString host;
    uint port;

    void run() override;

#ifdef OPUS
    QByteArray buffer;
    qint32 size;
    OpusDecode *opus;
#endif
};

#endif // CLIENT_H

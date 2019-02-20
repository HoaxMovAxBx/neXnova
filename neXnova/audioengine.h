#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QElapsedTimer>

#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QByteArray>
#include <QAudioDecoder>
#include <QAudioBuffer>
#include <QTcpSocket>
#include <QDataStream>
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QDir>
#include <QDirIterator>
#include <QMutex>

#include "connection.h"

#include "audioengine/client.h"
#include "audioengine/server.h"
#include "audioengine/audioinput.h"
#include "audioengine/audiooutput.h"

class Server;

class audioengine : public QThread
{
    Q_OBJECT
public:
    explicit audioengine(connection * conn,QObject *parent = nullptr);
    static const QAudioFormat defaultformat();
    static const QAudioDeviceInfo catchOutAsIn();

    void iniAudioServer();

private:
    void run() override;
    connection * conn;
    QHash<QString,QSoundEffect*>sounds;

    QMediaPlaylist * playlist;
    QMediaPlayer * player;
    QThread * playerthread;

    Client *client;
    QAudioFormat outprefered;
    Server *server;

    void loadSounds();
    void loadMusicPlayer();

signals:
    void level(float);
    void audiochanneFrom(QString);

public slots:
    void audiochannelinncoming(QJsonObject data);
    void videochannelinncoming(QJsonObject data);

    void iniaudiochat(QString target);

    void effect(QString name);
};


#endif // AUDIOENGINE_H

#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <QtCore>
#include <QtMultimedia>
#include "server.h"
#ifdef OPUS
#include <audioengine/r8brain.h>
#include <audioengine/opusencode.h>
#endif

class AudioInput : public QObject
{
    Q_OBJECT
public:
    explicit AudioInput(QObject *parent = 0);

signals:
    void error(QString);
    void dataReady(QByteArray);

public slots:
    void start(const QAudioDeviceInfo &devinfo);
    QByteArray header();
    void stateChanged(QAudio::State state);

private slots:
    void readyRead();

private:
    QAudioInput *audio;
    QIODevice *device;
    QAudioFormat format;
#ifdef OPUS
    r8brain *res;
    OpusEncode *opus;
    QByteArray buffer;
#endif
};

#endif // AUDIOINPUT_H

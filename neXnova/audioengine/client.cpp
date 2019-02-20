#include "client.h"

Client::Client(QObject *parent) : QThread(parent)
{
    qDebug()<<"Building client "<<endl;
}

void Client::initialize(const QString chost,uint timetobuffer,uint Port)
{
    qDebug() << "initializing audio client"<<endl;
    host = chost;
    ttb = timetobuffer;
    port = Port;
}

void Client::stateChange(QTcpSocket::SocketState socketState)
{
    if (socketState == QTcpSocket::UnconnectedState)
        finishedfunc();
}

QAudioFormat Client::currentAudioFormat()
{
    return format;
}

void Client::finishedfunc()
{
    emit error(socket->errorString());
}

void Client::readyRead()
{
    qDebug() << "Reading audio data"<<endl;

    if (socket->bytesAvailable() < 8)
            return;

        output = new AudioOutput(currentAudioFormat(), ttb);
        connect(output, &AudioOutput::currentlevel, this, &Client::currentlevel);
        output->setVolume(volume);

#ifdef OPUS
        opus = new OpusDecode;
        opus->start(format.channelCount(), format.sampleRate());
        size = 0;
#endif

#ifdef OPUS
    while (socket->bytesAvailable() > 0)
    {
        buffer.append(socket->readAll());
        while ((size == 0 && buffer.size() >= 4) || (size > 0 && buffer.size() >= size))
        {
            if (size == 0 && buffer.size() >= 4)
            {
                size = ArrayToInt(buffer.mid(0, 4));
                buffer.remove(0, 4);
            }
            if (size > 0 && buffer.size() >= size)
            {
                QByteArray data = buffer.mid(0, size);
                buffer.remove(0, size);
                size = 0;
                processInput(data);
            }
        }
    }
#else
    while (socket->bytesAvailable() > 0)
    {
        QByteArray data = socket->readAll();
        emit audioReady(data);
        output->writeData(data);
    }
#endif
}

void Client::connectedToPeer()
{
    qCritical() << "Client connected sucessfully"<<endl;
}

#ifdef OPUS
void Client::processInput(const QByteArray &data)
{
    QByteArray decoded = opus->decode(data);
    emit audioReady(decoded);
    output->writeData(decoded);
}

void Client::run()
{
    qDebug() <<"audioclient running "<<endl;
    socket = new QTcpSocket;
    connect(socket, &QTcpSocket::readyRead, this, &Client::readyRead,Qt::DirectConnection);
    connect(socket, &QTcpSocket::stateChanged, this, &Client::stateChange,Qt::DirectConnection);
    connect(socket, &QTcpSocket::connected ,this ,&Client::connectedToPeer,Qt::DirectConnection);
    qCritical() << "Connecting to "<<host<<":"<<port<<endl;
    socket->connectToHost(host,port);
    exec();
}
#endif

void Client::preferedFormat(QAudioFormat formatx)
{
    qDebug() << "setting audioinput"<<endl;
    format = formatx;
}

void Client::setVolume(int vol)
{
    volume = vol;
}

qint32 Client::ArrayToInt(const QByteArray &data)
{
    qint32 value;
    QDataStream stream(data);
    stream >> value;
    return value;
}

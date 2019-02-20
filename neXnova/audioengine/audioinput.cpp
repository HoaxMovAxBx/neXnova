#include "audioinput.h"

AudioInput::AudioInput(QObject *parent) : QObject(parent)
{
    device = nullptr;
}

void AudioInput::start(const QAudioDeviceInfo &devinfo)
{
    qDebug() << "Starting recording with device "<<devinfo.deviceName()<<endl;

    format = devinfo.preferredFormat();

    audio = new QAudioInput(devinfo,format);//(devinfo, format);
    connect(audio,&QAudioInput::stateChanged,this,&AudioInput::stateChanged);
    audio->setBufferSize(16384);


    device=audio->start();

    if (device==nullptr)
    {
        emit error("Failed to open audio device");
        return;
    }

#ifdef OPUS
    if (format.channelCount() > 2 || (format.sampleSize() != 16 && format.sampleSize() != 32) ||
            format.sampleType() != QAudioFormat::SignedInt || format.byteOrder() != QAudioFormat::LittleEndian)
    {
        emit error("This demo with opus only support 2 or 1 channels, 16 or 32 bits per sample,\n"
                   "signed integer as sample type and little endian byte order!");
        return;
    }

    res = new r8brain;
    res->start(format.channelCount(), format.sampleRate(), format.sampleSize());
    format.setSampleRate(48000);
    format.setSampleSize(16);
    opus = new OpusEncode;
    opus->start(format.channelCount(), format.sampleRate());
#endif

    connect(device, &QIODevice::readyRead, this, &AudioInput::readyRead,Qt::DirectConnection);
}

void AudioInput::readyRead()
{
    QByteArray data;
    qDebug() << "emiting data bytes "<<audio->bytesReady()<<endl;

    //Check the number of samples in input buffer
    int len = audio->bytesReady();

    //Read sound samples from input device to buffer
    if (len)
    {
        data.resize(len);
        device->read(data.data(), len);
    }

#ifdef OPUS
    QByteArray resampleddata = res->resample(data);
    int size = sizeof(qint16)*format.channelCount()*FRAME_SIZE;
    buffer.append(resampleddata);
    while (buffer.size() >= size)
    {
        QByteArray encodeddata = opus->encode(buffer.mid(0, size));
        buffer.remove(0, size);
        qDebug() << "emiting data "<<endl;
        emit dataReady(encodeddata);
    }
#else
    emit dataReady(data);
#endif
}

QByteArray AudioInput::header()
{
    QByteArray data;

    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << (quint8)format.sampleSize();
    stream << (quint32)format.sampleRate();
    stream << (quint8)format.channelCount();
    stream << (quint8)format.sampleType();
    stream << (quint8)format.byteOrder();

    return data;
}

void AudioInput::stateChanged(QAudio::State state )
{
    switch (state) {
    case QAudio::ActiveState:
        qDebug() << "Audio Input activated "<<endl;
        break;
    case QAudio::SuspendedState:
        qDebug() << "Audio Input suspended "<<endl;
        break;
    case QAudio::StoppedState:
        qDebug() << "Audio Input stopped   "<<endl;
        break;
    case QAudio::IdleState:
        qDebug() << "Audio Input Idle "<<endl;
        break;
    default:
        qFatal("Unknown audio state\n");
        break;
    }
}

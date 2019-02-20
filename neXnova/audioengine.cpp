#include "audioengine.h"

audioengine::audioengine(connection *conn, QObject *parent) : QThread(parent),conn(conn)
{
    //loadSounds();
    //loadMusicPlayer();

}

void audioengine::run()
{
    connect(conn,&connection::audiochannelrequest,
            this,&audioengine::audiochannelinncoming,Qt::QueuedConnection);
    loadMusicPlayer();
    loadSounds();
    exec();
}

void audioengine::iniAudioServer()
{
    server = new Server;
    server->initialize(9800);
    server->start();
}

void audioengine::loadSounds()
{
    QDir soundspath(":/sfx/sounds/");
    QStringList entris = soundspath.entryList(QDir::Files);
    Q_FOREACH(QString entry,entris){

        QSoundEffect * currentEffect = new QSoundEffect;
        currentEffect->setSource(QUrl("qrc:/sfx/sounds/"+entry));
        QString key = entry.remove(".wav");
        sounds.insert(key,currentEffect);
        qDebug() << "sound "<<key <<" loaded "<<endl;

    }
}

void audioengine::loadMusicPlayer()
{
    playerthread = new QThread(this);
    playerthread->start();

    player = new QMediaPlayer;


    playlist = new QMediaPlaylist;

    QDir soundspath(":/sfx/music/");
    QStringList entris = soundspath.entryList(QDir::Files);
    Q_FOREACH(QString entry,entris){

    playlist->addMedia(QUrl("qrc:/sfx/music/"+entry));
    playlist->setPlaybackMode(QMediaPlaylist::Random);

    }

    qCritical() << "Loaded "<<playlist->mediaCount() << " songs "<<endl;

    player->setPlaylist(playlist);
    player->moveToThread(playerthread);
    playlist->moveToThread(playerthread);

    QMetaObject::invokeMethod(player,"play");

}

void audioengine::audiochannelinncoming(QJsonObject data)
{
    qDebug() << "audiochannel recived "<<data<<endl;

    QString sender = data["sender"].toString();
    QString senderaddres = data["senderaddress"].toString();

    outprefered.setCodec("audio/pcm");
    outprefered.setSampleSize(data["sampleSize"].toInt());
    outprefered.setSampleRate(data["sampleRate"].toInt());
    outprefered.setChannelCount(data["channelCount"].toInt());
    outprefered.setSampleType((QAudioFormat::SampleType)data["sampleType"].toInt());
    outprefered.setByteOrder((QAudioFormat::Endian)data["byteOrder"].toInt());

    client = new Client;
    client->preferedFormat(outprefered);
    client->initialize(senderaddres,300,9801);
    client->setVolume(100);
    client->start();
    emit audiochanneFrom(sender);

}

void audioengine::videochannelinncoming(QJsonObject data)
{

}

void audioengine::iniaudiochat(QString target)
{
    //! TODO change for default input format funcion
    QJsonObject formatdata;
    QAudioFormat format = catchOutAsIn().preferredFormat();

    formatdata["peer"]=target;
    formatdata["sampleSize"]=format.sampleSize();
    formatdata["sampleRate"]=format.sampleRate();
    formatdata["channelCount"]=format.channelCount();
    formatdata["sampleType"]=format.sampleType();
    formatdata["byteOrder"]=format.byteOrder();

    iniAudioServer();
    QMetaObject::invokeMethod(conn,"requestAudioStreamToPeer",
                              Qt::QueuedConnection,Q_ARG(QJsonObject,formatdata));

}

void audioengine::effect(QString name)
{
    QSoundEffect * sound = sounds[name];
    if(sound==nullptr)
        return;
    sound->play();
}

const QAudioFormat audioengine::defaultformat()
{
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    return info.preferredFormat();
}

const QAudioDeviceInfo audioengine::catchOutAsIn()
{
    Q_FOREACH(auto device,QAudioDeviceInfo::availableDevices(QAudio::AudioInput)){
        qDebug() << device.deviceName() << endl;
        if(device.deviceName() == "alsa_output.pci-0000_00_1b.0.analog-stereo.monitor"){
            qDebug() << "GotCha!!!!"<<endl;
            return device;
        }
    }
    return QAudioDeviceInfo::defaultInputDevice();
}


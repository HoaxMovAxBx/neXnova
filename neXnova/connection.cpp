#include "connection.h"

connection::connection() : QThread()
{
    start();
    moveToThread(this);
}


void connection::run()
{
    sock = new QTcpSocket();
    pingpong = new QElapsedTimer;
    pingtimer = new QTimer;
    loggeduser = QString("");
    islogged = false;

    connect(pingtimer,&QTimer::timeout,this,&connection::ping);
    connect(sock,&QTcpSocket::readyRead,this,&connection::parse,Qt::DirectConnection);

    qDebug() << "Running connection... from thread"<<QThread::currentThreadId() << endl;

    pingtimer->start(5000);
    exec();

}

bool connection::testConnection(QString address)
{
    qDebug() << "Testing connection..." << endl;
    sock->connectToHost(address,50500);
    emit waiting();
    return sock->waitForConnected(1000);
}

void connection::sendAuthMsg(QString user, QString password)
{
    qDebug() << "authenticating... from thread "<<QThread::currentThreadId()<<endl;
    if(!sock->isOpen())
        return;

    loggeduser = user;

    QCryptographicHash hash(QCryptographicHash::Sha3_512);

    hash.addData(password.trimmed().toLatin1());

    QByteArray encryptedpass =  hash.result();
    encryptedpass = encryptedpass.toBase64();

    qDebug() << "Encrpypted password "<<QString::fromLatin1(encryptedpass)<<endl;


    QJsonObject authdata;
    authdata["user"]=user;
    authdata["password"]=QString::fromLatin1(encryptedpass);
    QJsonDocument doc(authdata);

    sock->write("MAKELOGIN");
    sock->write(doc.toBinaryData());
    sock->write(FOOTER);

}

void connection::parse()
{
    qDebug() << "parsing response..."<<endl;
    buffer.append(sock->readAll());
    while (buffer.contains(FOOTER)) {

        qDebug() << "Iterating for reciver "<<endl;

        int pos = buffer.indexOf(FOOTER);
        QByteArray piece = buffer.mid(0,pos);
        buffer.remove(0,pos+9);

        if(piece.contains("LOGIN OK")){
            islogged = true;
            qDebug() << "LOGIN ok"<<endl;
            emit authresultOk();}
        else if(piece.contains("LOGIN BAD")){
            qDebug() << "LOGIN bad"<<endl;
            emit authresultBad();}
        else if(piece.contains("PONG" )){
            quint64 pingtime = pingpong->elapsed();
            qDebug()<<"ping time "<<pingtime<<" ms "<<endl;
            emit pingcompleted(pingtime);
        }
        else if(piece.contains("ACCOUNT CREATED")){
            emit accountCreated();
            qDebug() << "ACCOUNT CREATED "<<endl;
        }
        else if(piece.contains("ACCOUNT ERROR CODE ")){
            piece.remove(0,19);
            piece=piece.trimmed();
            qDebug() <<"Resulting data "<<piece<<endl;
            qDebug() << "ACCOUNT ERROR CODE "<<piece.toUShort()<<endl;
            emit accountCreationError(piece.toUShort());
        }
        else if(piece.contains("ACCOUNT EMAIL")){
            qDebug()<<"EMAIL VERIFICATION REQ"<<endl;
            emit accountEmailVerification();
        }
        else if(piece.contains("CHAT ")){
            piece.remove(0,5);
            QJsonDocument doc = QJsonDocument::fromBinaryData(piece);
            qDebug() << "Reciving chat msg "<<endl;
            emit chatMsg(doc.object());
        }
        else if(piece.contains("CLIENTS LIST RESP ")){
            piece.remove(0,18);
            QJsonDocument doc = QJsonDocument::fromBinaryData(piece);
            emit connectedClientsInfo(doc.object());
        }
        else if(piece.contains("UNIVERSE DATA ")){
            piece.remove(0,14);
            qDebug() << "Reciving universe data  "<<endl;
            QJsonDocument doc = QJsonDocument::fromBinaryData(piece);
            emit universeData(doc.object());
        }
        else if(piece.contains("AUDIO CHANEL CONFIRMATION")){
            emit audiochannelconfirm();
            qDebug() << "Reciving audio channel confirmation "<<endl;
        }
        else if(piece.contains("VIDEO CHANEL CONFIRMATION")){
            emit videochannelconfirm();
            qDebug() << "Reciving video channel confirmation "<<endl;
        }
        else if(piece.contains("VIDEO CHANNEL REQUEST ")){
            piece.remove(0,22);
            QJsonDocument doc = QJsonDocument::fromBinaryData(piece);
            emit videochannelrequest(doc.object());
        }
        else if(piece.contains("AUDIO CHANNEL REQUEST ")){
            piece.remove(0,22);
            qDebug() << "AUDIO CHANNEL REQUEST msg recived"<<endl;
            QJsonDocument doc = QJsonDocument::fromBinaryData(piece);
            emit audiochannelrequest(doc.object());
        }
        else
            qWarning() << "Unrecongnaiced Command " << piece<<endl;

    }
}

void connection::ping()
{
    QByteArray ping;
    ping.append("PING ");
    ping.insert(ping.size(),18 - ping.size(),'F');
    ping.append(FOOTER);
    pingpong->start();
    sock->write(ping);
}

void connection::accountCreate(QJsonObject msg)
{
    QJsonDocument doc(msg);

    qDebug() << "Creating account "<<msg<<endl;

    QByteArray binaryfromated;
    binaryfromated.append("ACOUNTCREATE ");
    binaryfromated.append(doc.toBinaryData());
    binaryfromated.append(FOOTER);
    sock->write(binaryfromated);
}


void connection::sendMsg(QJsonObject msg)
{
    QJsonDocument doctop(msg);
    qCritical() << "sending chat msg "<<endl;
    sock->write("CHAT ");
    sock->write(doctop.toBinaryData());
    sock->write(FOOTER);

}

void connection::requestClientsInfo()
{
    sock->write("CLIENTS LIST ");
    sock->write(FOOTER);
}

void connection::requestSolarSystem(QString id)
{
    sock->write("SOLARSYSTEM ");
    sock->write(id.toLatin1());
    sock->write(FOOTER);
}

void connection::defineRaze(QString user, QString raze)
{
    sock->write("DEFINERAZE ");
    sock->write(user.toLatin1().trimmed());
    sock->write(" ");
    sock->write(raze.toLatin1().trimmed());
    sock->write(FOOTER);
}

void connection::requestAudioStreamToPeer(QJsonObject formatoanduser)
{

    QJsonDocument doc(formatoanduser);
    sock->write("AUDIOCHANNEL ");
    sock->write(doc.toBinaryData());
    sock->write(FOOTER);
}

void connection::requestVideoStreamToPeer(QString user)
{
    sock->write("VIDEOCHANNEL TO ");
    sock->write(user.toLatin1());
    sock->write(FOOTER);
}

QString connection::loggedUser() const
{
    if(islogged)
        return loggeduser;
    return QString("");
}




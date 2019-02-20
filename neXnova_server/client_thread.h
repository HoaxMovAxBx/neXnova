#ifndef CLIENT_THREAD_H
#define CLIENT_THREAD_H

#include <QObject>
#include <QSslSocket>

#include <QSslCertificate>
#include <QSslKey>
#include <QSslConfiguration>
#include <QFile>

#include <QSqlQuery>
#include <QSqlError>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <QCryptographicHash>

#include <QHostAddress>
#include <QThread>

#include <QString>
#include <QDataStream>

#define FOOTER ";FAFFAAAA"
#define UNIVERSE_RADIUS 6000
#define SS_RADIUS 450

#include "server.h"

class server;
class client_thread : public QThread
{
    Q_OBJECT
public:
    explicit client_thread(qintptr descriptor,server*serverptr);
    client_thread(const client_thread & copy);
    client_thread();
    ~client_thread();

    QString getName() const;
    QString getAvatar() const;    
    QTcpSocket *getHandler() const;
    int getSqlId() const;
    bool getState() const;
    server *getServerPtr() const;
    qintptr getDescriptor() const;

private:
    void run() override;

    qintptr copy;
    QTcpSocket*handler;
    QByteArray buffer;

    //!Client data
    QString avatar;
    QString name;
    int sql_id;
    bool singin;

    server*serverptr;

    bool authenticate(const QString user,const QString pass);
    void createUser(QJsonObject jsobj);
    void defineRaze(QString user,QString raze);

    QVector<uint> listUserSolarSystems(QString user);
    void loadSolarSystem(uint id);
    void createMainPlanet(QString user, QString Name);
    static uint lastStarGeneratedPlusOne();


    void generateRandomSolarSystem(uint id = lastStarGeneratedPlusOne());

    QString generateSunName() const;
    QString generatePlanetName() const;
    QString generateMoonName() const;

    QJsonObject generateRootParam() const;

    float generateSunRadius() const;
    float generateMoonRadius(float planetRadius = 0.0) const;
    float generatePlanetRadius() const;

    const QVector<float> generateSolarSysPos() const;
    const QVector<float> generatePlanetPos(QVector<float>sunpos,float sunradius) const;

    QHash<quint16,QJsonDocument>cache;

    QJsonObject connectedUsersInfo();

signals:

public slots:
    void disconnected();
    void serve();
    void serverMsgSender(QByteArray msg);
};

Q_DECLARE_METATYPE(client_thread)

#endif // CLIENT_THREAD_H

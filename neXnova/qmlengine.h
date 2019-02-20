#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>

#include <QMetaObject>
#include <QSettings>

#include <QQmlEngine>
#include <QQmlApplicationEngine>
#include <QQmlAspectEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQuickStyle>
#include <QQuickView>


#include "connection.h"
#include "razeselector.h"
#include "audioengine.h"
#include "chatclientsmodel.h"
#include "imageprovider.h"
#include "planetbarmodel.h"
#include "jsoncachelayer.h"
#include "videochat.h"

#include "qmlpluguins/circleimage.h"
#include "qmlpluguins/fps.h"
#include "qmlpluguins/videowidget.h"

#include "3d/skybox.h"
#include "3d/star.h"
#include "3d/planet.h"
#include "3d/meteor.h"
#include <3d/universe.h>

class JsonCacheLayer;
class qmlengine : public QObject {
    Q_OBJECT

public:
    explicit qmlengine();
    connection*conn;
    RazeSelector * razeModels;
    ChatClientsModel * chatmodel;
    planetbarmodel * barmodel;
private:
    QQmlEngine * engine;
    QQmlContext*rootContext;
    QQuickView * window;
    QSurfaceFormat format;

    audioengine * sfxengine;
    AsyncImageProvider * imgprovider;

    QSettings * settings;

    void setSurfaceFormat();

    JsonCacheLayer * cache;
    Universe* verso;

    videochat * vchat;

    fps * fpscalc;

public Q_SLOTS:
    void check_passwork(QString user, QString pass);
    void accountCreate(QString user,QString pass,QString email,QString mainPlanetName,QString avatar);
    void defineRaze(QString user,QString raze);
    void test_slot();

    void sendChatText(QString dest,QString msg);
    void chatMsgClasifier(QJsonObject chatobj);
    void iniAudioChatTo(QString target);
    void iniVideoChat(QString target);

    void openVideoChannel(QJsonObject info);

    void sfx(QString name);

signals:
    void authSuccess();
    void authFailed();

    void pong(quint32);
    void serverTimeOut();

    void accountCreatedSuccessfully();
    void accountCreationError(quint8 errorcode);
    void accountWaitingForEmailVerification();

    void chatTxtMsg(QString sender,QString msg);
    void avatarToCach(QPixmap avatar);

    void audiolevel(float levelval);
    void videochannelOpen();

};


#endif // MAINWINDOW_H

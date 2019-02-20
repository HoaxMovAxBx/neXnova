#ifndef UNIVERSE_H
#define UNIVERSE_H

#include <QObject>
#include <QDebug>

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include <Qt3DAnimation>
#include <Qt3DLogic>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <3d/solarsistem.h>
#include <3d/skybox.h>
#include <3d/controledcamera.h>

class Universe : public Qt3DCore::QEntity
{
    Q_OBJECT
    Q_PROPERTY(Qt3DInput::QInputSettings* inputset READ inputset WRITE setInputset NOTIFY inputsetChanged)
public:
    explicit Universe();

public slots:
    void setRenderData(QJsonObject data);
    void entityChanged(QString id,QJsonObject newdata);
    void focusPlanet(QString planetName);

    void setInputset(Qt3DInput::QInputSettings* inputset);
    void setEventSource(QObject *eventSource);
    Qt3DInput::QInputSettings *inputset();

    void animator();

signals:
    void loading();
    void loaded();
    void inputsetChanged();

private:
    SolarSistem * sceneRoot;
    skybox * sky;
    ControledCamera * camera;

    Qt3DExtras::QFirstPersonCameraController * controler;
    Qt3DRender::QRenderSettings * rendersettings;
    Qt3DExtras::QForwardRenderer * forwardrender;
    Qt3DInput::QInputSettings * input;
    Qt3DLogic::QFrameAction * syncanimator;

    float day;

};

#endif // UNIVERSE_H






#ifndef SOLARSISTEM_H
#define SOLARSISTEM_H

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

#include <3d/moon.h>
#include <3d/planet.h>
#include <3d/star.h>
#include <3d/ring.h>
#include <3d/meteor.h>

class SolarSistem : public Qt3DCore::QEntity
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(Qt3DCore::QTransform* transform READ transform WRITE setTransform NOTIFY transformChanged)

public:
    explicit SolarSistem(Qt3DCore::QEntity *root = nullptr);
    static int timeScale(int year = 2000,int month = 1,int day = 1);
    static SolarSistem * generateSolarSystemFromJson(QJsonObject data, Qt3DCore::QEntity *parent);

    QString name() const;
    void setName(QString name);

    QVector3D position() const;
    void setPosition(QVector3D position);

    Qt3DCore::QTransform * transform() const;
    void setTransform(Qt3DCore::QTransform * transform);


signals:
    void positionChanged();
    void transformChanged();

public slots:
    void generateFromJson(QJsonObject solarSystemData);
    float getPlanetRadius(QString name) const;
    QVector3D getPlanetPos(QString name) const;
    void animate(float day);

private:
    QList<star*>stars;
    Qt3DCore::QTransform* Transform;
    QString Name;

};

#endif // SOLARSISTEM_H

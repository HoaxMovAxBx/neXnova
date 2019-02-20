#ifndef PLANET_H
#define PLANET_H

#include <QObject>

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include <Qt3DAnimation>

#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <math.h>

#include "star.h"
#include "ring.h"
#include "moon.h"
#include "spacestation.h"

#include <rotation.h>

class planet : public Qt3DCore::QEntity
{
    Q_OBJECT
    Q_PROPERTY(QString eid READ eid WRITE setEid)
    Q_PROPERTY(Qt3DCore::QTransform* transform READ transform WRITE setTransform NOTIFY transformChanged)
    Q_PROPERTY(Qt3DRender::QMaterial* material READ material WRITE setMaterial NOTIFY materialChanged)
    Q_PROPERTY(Qt3DExtras::QSphereMesh* mesh READ mesh WRITE setMesh NOTIFY meshChanged)
    Q_PROPERTY(RotationParameters* rotational READ rotational WRITE setRotational NOTIFY rotationalChanged)
    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(quint16 campus READ campus WRITE setCampus NOTIFY campusChanged)
    Q_PROPERTY(double temperature READ temperature WRITE setTemperature NOTIFY temperatureChanged)
    Q_PROPERTY(QJsonObject structures READ structures WRITE setStructures NOTIFY structuresChanged)
    Q_PROPERTY(float scale READ scale WRITE setScale NOTIFY scaleChanged)

public:
    explicit planet(Qt3DCore::QEntity *root = nullptr);

    static planet *generateFromJson(QJsonObject data,Qt3DCore::QEntity * parent);

signals:
    void transformChanged();
    void materialChanged();
    void meshChanged();
    void rotationalChanged();
    void typeChanged();
    void campusChanged();
    void temperatureChanged();
    void structuresChanged();
    void scaleChanged();
    void clicked(Qt3DRender::QPickEvent *pick);

public:
    QString eid() const;
    void setEid(QString eid);

    Qt3DCore::QTransform* transform() const;
    void setTransform(Qt3DCore::QTransform * newTransform);

    Qt3DRender::QMaterial * material() const;
    void setMaterial(Qt3DRender::QMaterial * material);

    Qt3DExtras::QSphereMesh * mesh() const;
    void setMesh(Qt3DExtras::QSphereMesh * mesh);

    RotationParameters* rotational() const;
    void setRotational(RotationParameters* rotational);

    QString name() const;
    void setName(QString name);

    quint16 campus() const;
    void setCampus(quint16 campus);

    double temperature() const;
    void setTemperature(double temperature);

    int type() const;
    void setType(int type);

    void setStructures(QJsonObject structures);
    QJsonObject structures() const;

public slots:
    void setScale(float scale);
    float scale() const;

    void setMoon(moon* newmoon);
    void removeMoon(moon * moonToRemove);

    void setRing(ring* newring);
    void removeRing(ring * ringToRemove);

    void setSpaceStation(spacestation* newspacestation);
    void removeSpaceStation(spacestation* spacestationToRemove);

    void picked(Qt3DRender::QPickEvent * ev);
    void animate(float day);

private:
    Qt3DExtras::QSphereMesh * Mesh;
    Qt3DRender::QMaterial * Material;
    Qt3DCore::QTransform* Transform;
    Qt3DRender::QObjectPicker * MrPickles;
    RotationParameters * Rotational;
    QParallelAnimationGroup * Animations;

    QString Eid;
    QString Name;
    quint16 usable_campus;
    double Temperature;
    int Type;
    float Scale;
    QJsonObject Structres;

    QVector<moon*>moons;
    QVector<ring*>rings;
    QVector<spacestation*>spacestations;


};

#endif // PLANET_H

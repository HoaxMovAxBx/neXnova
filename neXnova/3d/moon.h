#ifndef MOON_H
#define MOON_H

#include <QObject>

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include <Qt3DAnimation>

#include <Qt3DRender/QMesh>
#include <Qt3DExtras/QSphereMesh>

#include "rotation.h"

#define D3Debug 0

class moon : public Qt3DCore::QEntity
{
    Q_OBJECT
    Q_PROPERTY(QString eid READ eid WRITE setEid)

    Q_PROPERTY(Qt3DExtras::QSphereMesh * moonmesh READ moonmesh WRITE setMoonmesh
               NOTIFY moonmeshChanged)
    Q_PROPERTY(Qt3DCore::QTransform * moontransform READ moontransform WRITE setMoontransform
               NOTIFY moontransformChanged)
    Q_PROPERTY(Qt3DRender::QMaterial * moonmaterial READ moonmaterial WRITE setMoonmaterial
               NOTIFY moonmaterialChanged)
    Q_PROPERTY(Qt3DRender::QPointLight * moonlight READ moonlight WRITE setMoonlight
               NOTIFY moonlightChanged)
    Q_PROPERTY(RotationParameters * moonrotation READ moonrotation WRITE setMoonrotation
               NOTIFY moonrotationChanged)
    Q_PROPERTY(float scale READ scale WRITE setScale NOTIFY scaleChanged)

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)

    Q_PROPERTY(int type READ type WRITE setType)

    Q_PROPERTY(QJsonObject structures READ structures WRITE setStructures NOTIFY structuresChanged)


public:
    explicit moon(Qt3DCore::QEntity *root = nullptr);
    void ini();
    static moon * iniFromJson(QJsonObject data, Qt3DCore::QEntity *parent);

signals:
    void moonrotationChanged();
    void moonlightChanged();
    void moonmaterialChanged();
    void moonmeshChanged();
    void moontransformChanged();
    void scaleChanged();
    void nameChanged();
    void descriptionChanged();
    void structuresChanged();
    void clicked(Qt3DRender::QPickEvent *pick);

public slots:
    void animate(float day);

    RotationParameters * moonrotation();
    Qt3DRender::QPointLight *moonlight();
    Qt3DRender::QMaterial * moonmaterial();
    Qt3DCore::QTransform * moontransform();
    Qt3DExtras::QSphereMesh * moonmesh();
    float scale();
    QString name();
    QString description();
    int type();
    QJsonObject structures();
    QString eid() const;

    void setEid(QString eid);
    void setMoonrotation(RotationParameters * rotation);
    void setMoonlight (Qt3DRender::QPointLight * light);
    void setMoonmaterial (Qt3DRender::QMaterial * mater);
    void setMoontransform (Qt3DCore::QTransform * transform);
    void setMoonmesh(Qt3DExtras::QSphereMesh * m);
    void setScale(float newscale);
    void setName(QString newname);
    void setDescription(QString newdes);
    void setStructures(QJsonObject newstructures);
    void setType(int type);

private:
    Qt3DExtras::QSphereMesh * mesh;
    Qt3DRender::QMaterial * material;
    Qt3DCore::QTransform* transformptr;
    Qt3DRender::QPointLight * moonlightptr;
    Qt3DRender::QObjectPicker * MrPickles;
    RotationParameters * rotational;

    float Scale;
    QString Eid;
    QString Name;
    QString Description;
    int Type;
    QJsonObject Structures;


};

#endif // MOON_H

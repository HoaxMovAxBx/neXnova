#ifndef METEOR_H
#define METEOR_H

#include <QObject>

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include <Qt3DAnimation>

#include "rotation.h"

class meteor : public Qt3DCore::QEntity
{
    Q_OBJECT
    Q_PROPERTY(QString eid READ eid WRITE setEid)
    Q_PROPERTY(Qt3DRender::QMesh* mesh READ mesh WRITE setMesh NOTIFY meshChanged)
    Q_PROPERTY(Qt3DCore::QTransform* transform READ transform WRITE setTransform NOTIFY transformChanged)
    Q_PROPERTY(Qt3DRender::QMaterial* material READ material WRITE setMaterial NOTIFY materialChanged)
    Q_PROPERTY(RotationParameters* rotational READ rotational WRITE setRotational NOTIFY rotationalChanged)
    Q_PROPERTY(Qt3DRender::QPointLight* light READ light WRITE setLight NOTIFY lightChanged)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)

public:
    explicit meteor(Qt3DCore::QEntity *parent = nullptr);
    static meteor * generateFromJson(QJsonObject data,Qt3DCore::QEntity* parent);

    void setMesh(Qt3DRender::QMesh*mesh);
    Qt3DRender::QMesh* mesh() const;

    void setTransform(Qt3DCore::QTransform * transform);
    Qt3DCore::QTransform* transform () const;

    void setMaterial(Qt3DRender::QMaterial* material);
    Qt3DRender::QMaterial* material() const;

    void setRotational(RotationParameters * rotational);
    RotationParameters* rotational() const;


    void setLight(Qt3DRender::QPointLight * light);
    Qt3DRender::QPointLight* light() const;

    void setName(QString name);
    QString name() const;

    void setPosition(QVector3D position);
    QVector3D position()const;

    QString eid() const;
    void setEid(QString eid);

signals:
    void meshChanged();
    void transformChanged();
    void materialChanged();
    void rotationalChanged();
    void lightChanged();
    void positionChanged();
    void orbitCenterChanged();
    void clicked(Qt3DRender::QPickEvent *pick);

public slots:
    void animate(float day);

private:
    Qt3DRender::QMesh * Mesh;
    Qt3DRender::QMaterial * Material;
    Qt3DCore::QTransform * Transform;
    Qt3DRender::QPointLight * Light;
    Qt3DRender::QObjectPicker * MrPickles;
    RotationParameters * Rotational;

    QString Name;
    QString Eid;
};

#endif // METEOR_H

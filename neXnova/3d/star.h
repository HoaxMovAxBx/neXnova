#ifndef STAR_H
#define STAR_H

#include <QObject>

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include <Qt3DAnimation>
#include <QVariant>

#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

#include <rotation.h>
#include <3d/planet.h>
#include <3d/spacestation.h>

class planet;
class spacestation;

class star : public Qt3DCore::QEntity
{
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QString starname READ starname WRITE setStarname)
    Q_PROPERTY(QString eid READ eid WRITE setEid)
    Q_PROPERTY(Qt3DExtras::QSphereMesh* mesh READ mesh WRITE setMesh NOTIFY meshChanged)
    Q_PROPERTY(Qt3DCore::QTransform* transform READ transform WRITE setTransform NOTIFY transformChanged)
    Q_PROPERTY(Qt3DRender::QMaterial* material READ material WRITE setMaterial NOTIFY materialChanged)
    Q_PROPERTY(RotationParameters* rotational READ rotational WRITE setRotational NOTIFY rotationalChanged)
    Q_PROPERTY(Qt3DRender::QPointLight* starfire READ starfire WRITE setStarfire NOTIFY starfireChanged)
    Q_PROPERTY(float scale READ scale WRITE setScale NOTIFY scaleChanged)

    Q_PROPERTY(float roll READ roll WRITE setRoll NOTIFY rollChanged)
    Q_PROPERTY(Qt3DRender::QObjectPicker* picker READ picker WRITE setPicker NOTIFY pickerChanged)


public:
    explicit star(Qt3DCore::QEntity *root = nullptr);
    static star* generateStarFromJson(QJsonObject data,Qt3DCore::QEntity*parent);

signals:
    void positionChanged();
    void meshChanged();
    void transformChanged();
    void materialChanged();
    void rotationalChanged();
    void starfireChanged();
    void rollChanged();
    void scaleChanged();
    void clicked(Qt3DRender::QPickEvent *pick);
    void pickerChanged();

public slots:
    QString eid() const;
    void setEid(QString eid);

    void setPosition(QVector3D & newpostion);
    const QVector3D position() const;

    QString starname() const;
    void setStarname(QString starname);

    void setRoll(float roll);
    float roll();

    Qt3DExtras::QSphereMesh * mesh();
    void setMesh(Qt3DExtras::QSphereMesh * mesh);

    Qt3DRender::QMaterial * material();
    void setMaterial(Qt3DRender::QMaterial * material);

    Qt3DCore::QTransform * transform();
    void setTransform(Qt3DCore::QTransform * transform);

    Qt3DRender::QPointLight * starfire();
    void setStarfire(Qt3DRender::QPointLight * starfire);

    RotationParameters * rotational();
    void setRotational(RotationParameters * rotational);

    float scale();
    void setScale(float newscale);

    Qt3DRender::QObjectPicker* picker() const;
    void setPicker(Qt3DRender::QObjectPicker * newpicker);

    void shaderLog(QString log);
    void shaderStatus(Qt3DRender::QShaderProgram::Status status);

    void analizeShader(Qt3DRender::QShaderProgram *shader);

    QList<planet*> getPlanets() const;

    void clickedMsg(Qt3DRender::QPickEvent *pick);

public slots:

    void setPlanet(planet* newplanet);
    void removePlanet(planet* planetToRemove);

    void setSpaceStation(spacestation* newspacestation);
    void removeSpaceStation(spacestation* spacestationToRemove);
    void animate(float day);

private: 
    //! material , mesh , etc
    Qt3DExtras::QSphereMesh * Mesh;
    Qt3DRender::QMaterial * Material;
    Qt3DCore::QTransform * Transform;
    Qt3DRender::QPointLight * Starfire;
    Qt3DRender::QObjectPicker * MrPickles;
    RotationParameters * Rotation;

    QPropertyAnimation * Sunrotation;

    QString Eid;
    QString Name;
    qint16 Temperature;
    int Type;
    float Scale;

    QList<planet*>planets;
    QList<spacestation*>spacestations;

};

#endif // STAR_H

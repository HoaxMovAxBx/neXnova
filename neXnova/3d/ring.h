#ifndef RING_H
#define RING_H

#include <QObject>

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include <Qt3DAnimation>

class ring : public Qt3DCore::QEntity
{
    Q_OBJECT
    Q_PROPERTY(QString eid READ eid WRITE setEid)
    Q_PROPERTY(Qt3DRender::QMesh* mesh READ mesh WRITE setMesh NOTIFY meshChanged)
    Q_PROPERTY(Qt3DRender::QMaterial* material READ material WRITE setMaterial NOTIFY materialChanged)
    Q_PROPERTY(Qt3DCore::QTransform* transform READ transform WRITE setTransform NOTIFY transformChanged)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(int type READ type WRITE setType)

public:
    explicit ring(Qt3DCore::QEntity *parent = nullptr);

signals:
    void meshChanged();
    void materialChanged();
    void transformChanged();
public slots:
    static ring *generateFromJson(QJsonObject ring_data, Qt3DCore::QEntity *parent);
    void defaultRing(float parentRadius);

    Qt3DRender::QMesh* mesh();
    void setMesh(Qt3DRender::QMesh* newmesh);

    Qt3DRender::QMaterial* material();
    void setMaterial(Qt3DRender::QMaterial* newmaterial);

    Qt3DCore::QTransform* transform();
    void setTransform(Qt3DCore::QTransform*newtransform);

    void setName(QString newname);
    QString name();

    void setType(int type);
    int type();

    QString eid() const;
    void setEid(QString eid);

private:
    Qt3DRender::QMesh * Mesh;
    Qt3DRender::QMaterial * Material;
    Qt3DCore::QTransform * Transform;

    int Type;
    QString Name;
    QString Eid;


};

#endif // RING_H

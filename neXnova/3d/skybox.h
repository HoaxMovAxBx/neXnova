#ifndef SKYBOX_H
#define SKYBOX_H

#include <QObject>


#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include <Qt3DAnimation>

class skybox : public Qt3DCore::QEntity
{
    Q_OBJECT
public:
    explicit skybox(Qt3DCore::QEntity *root = nullptr);

signals:

public slots:
    Qt3DRender::QEffect * generateEffects();

private:
    Qt3DExtras::QDiffuseMapMaterial * skymaterial;
    Qt3DRender::QMesh * mesh;
    Qt3DCore::QTransform * transform;
    Qt3DRender::QEffect * effect;

};

#endif // SKYBOX_H

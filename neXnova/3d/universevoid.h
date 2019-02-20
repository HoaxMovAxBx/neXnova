#ifndef UNIVERSEVOID_H
#define UNIVERSEVOID_H

#include <QObject>

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include <Qt3DAnimation>

class UniverseVoid : public Qt3DCore::QEntity
{
    Q_OBJECT
public:
    explicit UniverseVoid(Qt3DCore::QEntity *root = nullptr);

signals:

public slots:

private:
    Qt3DRender::QTexture2D * shadowTexture;
    Qt3DCore::QEntity* lightEntity;
    Qt3DRender::QCamera * lightCameraLens;

    Qt3DRender::QEffect * generateVoidEffect();
};

#endif // UNIVERSEVOID_H

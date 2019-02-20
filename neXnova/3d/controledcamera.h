#ifndef CONTROLEDCAMERA_H
#define CONTROLEDCAMERA_H

#include <QObject>

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include <Qt3DAnimation>
#include <Qt3DLogic>

class ControledCamera : public Qt3DRender::QCamera
{
    Q_OBJECT
public:
    explicit ControledCamera(Qt3DCore::QEntity *parent = nullptr);

signals:

public slots:
    void setFocusedItem(QVector3D newPos, float near);

private:
    QPropertyAnimation * lookAtOffsetAnimation;
    QPropertyAnimation * cameraPosAnimation;

};




#endif // CONTROLEDCAMERA_H

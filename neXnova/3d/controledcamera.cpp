#include "controledcamera.h"

ControledCamera::ControledCamera(Qt3DCore::QEntity *parent) : Qt3DRender::QCamera(parent)
{
    setProjectionType(Qt3DRender::QCameraLens::PerspectiveProjection);
    setFieldOfView(45);
    setAspectRatio(16/9);
    setNearPlane(10);
    setFarPlane(2000000.0);
    setPosition(QVector3D(0.0,0.0,-40.0));
    setUpVector(QVector3D(0.0,1.0,0.0));
    setViewCenter(QVector3D(0.0,0.0,0.0));
}

void ControledCamera::setFocusedItem(QVector3D newPos,float near)
{
    setNearPlane(near);
    //! TODO improbements
    QVector3D defaultUp = upVector();
    QVector3D center = newPos.normalized();
    defaultUp = QVector3D::crossProduct(defaultUp,center);
    defaultUp = defaultUp *(near*4);//! CAMERA DISTANCE
    defaultUp += newPos;
    defaultUp += QVector3D(0.0,near,0.0);


    lookAtOffsetAnimation = new QPropertyAnimation(this,"viewCenter");
    lookAtOffsetAnimation->setEndValue(newPos);
    lookAtOffsetAnimation->setDuration(20000);
    lookAtOffsetAnimation->setEasingCurve(QEasingCurve::InOutQuad);

    cameraPosAnimation = new QPropertyAnimation(this,"position");
    cameraPosAnimation->setEndValue(defaultUp);
    cameraPosAnimation->setDuration(20000);
    cameraPosAnimation->setEasingCurve(QEasingCurve::InOutQuad);

    cameraPosAnimation->start();
    lookAtOffsetAnimation->start();


}


#ifndef ROTATION_H
#define ROTATION_H

#include <QObject>
#include <QJsonObject>
#include <QJsonValue>

#include <Qt3DCore/QTransform>
#include <math.h>

#define auScale 149.870700

class RotationParameters : public QObject
{
    Q_OBJECT
public:
    explicit RotationParameters(QObject *parent = nullptr);
    void jsonIni(QJsonObject data);

    void calculateTransform(Qt3DCore::QTransform * tranform,float day);

signals:

public slots:

private:
    float N,N1,N2;
    float I,I1,I2;
    float W,W1,W2;
    float A,A1,A2;
    float E,E1,E2;
    float M,M1,M2;
    float e;
    float rotz,roty,rotx;
};

#endif // ROTATION_H

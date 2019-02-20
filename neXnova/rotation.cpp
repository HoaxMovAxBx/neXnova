#include "rotation.h"

RotationParameters::RotationParameters(QObject *parent) : QObject(parent)
{

}

void RotationParameters::jsonIni(QJsonObject data)
{
    N1 = data["N1"].toDouble();
    N2 = data["N2"].toDouble();

    A1 = data["A1"].toDouble();
    A2 = data["A2"].toDouble();

    I1 = data["I1"].toDouble();
    I2 = data["I2"].toDouble();

    W1 = data["W1"].toDouble();
    W2 = data["W2"].toDouble();

    E1 = data["E1"].toDouble();
    E2 = data["E2"].toDouble();

    M1 = data["M1"].toDouble();
    M2 = data["M2"].toDouble();

    rotx = data["rotx"].toDouble();
    roty = data["roty"].toDouble();
    rotz = data["rotz"].toDouble();

    qInfo() << "Initialization Completed "<<endl;

}

void RotationParameters::calculateTransform(Qt3DCore::QTransform *tranform, float day)
{
    if(tranform==nullptr)
        return;

    N = (N1 + N2 * day) * M_PI / 180;
    I = (I1 + I2 * day) * M_PI / 180;
    W = (W1 + W2 * day) * M_PI / 180;
    A = A1 + A2 * day;
    E = E1 + E2 * day;
    M = (M1 + M2 * day) * M_PI / 180;
    e = M + E * std::sin(M) * (1.0 + E * std::cos(M));

    tranform->setRotationX(day*rotx);
    tranform->setRotationY(day*roty);
    tranform->setRotationZ(day*rotz);

    if(qFuzzyCompare(N,0.0)&&
       qFuzzyCompare(I,0.0)&&
       qFuzzyCompare(W,0.0)&&
       qFuzzyCompare(A,0.0)&&
       qFuzzyCompare(E,0.0)&&
       qFuzzyCompare(M,0.0))
        return;

    float xv = A * (std::cos(e) - E);
    float yv = A * (std::sqrt(1.0 - E * E) * std::sin(e));
    float v = std::atan2(yv, xv);

    float r = std::sqrt(xv * xv + yv * yv);

    auto xh = r * (std::cos(N) * std::cos(v + W)
                   - std::sin(N) * std::sin(v + W) * std::cos(I));
    auto zh = -r * (std::sin(N) * std::cos(v + W)
                    + std::cos(N) * std::sin(v + W) * std::cos(I));
    auto yh = r * (std::sin(W + v) * std::sin(I));

    tranform->setTranslation(QVector3D(xh,yh,zh)*auScale);

    // Planet Data
        // tilt - planet axis angle
        // N1/2 - longitude of the ascending node
        // i1/2 - inclination to the ecliptic (plane of the Earth's orbit)
        // w1/2 - argument of perihelion
        // a1/2 - semi-major axis, or mean distance from Sun
        // e1/2 - eccentricity (0=circle, 0-1=ellipse, 1=parabola)
        // M1/2 - mean anomaly (0 at perihelion; increases uniformly with time)
        // period - sidereal rotation period
        // centerOfOrbit - the planet in the center of the orbit
        // (orbital elements based on http://www.stjarnhimlen.se/comp/ppcomp.html)

}

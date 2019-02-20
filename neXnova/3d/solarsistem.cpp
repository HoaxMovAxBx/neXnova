#include "solarsistem.h"

SolarSistem::SolarSistem(Qt3DCore::QEntity *root) : Qt3DCore::QEntity(root)
{

}

int SolarSistem::timeScale(int year, int month, int day)
{
    return int(367 * year - 7 * (year + (month + 9) / 12) / 4 + 275 * month / 9 + day - 730530);
}

SolarSistem *SolarSistem::generateSolarSystemFromJson(QJsonObject data,
                                                      Qt3DCore::QEntity* parent)
{
    SolarSistem * solarsys = new SolarSistem(parent);
    solarsys->setName(data["name"].toString());
    solarsys->setPosition(QVector3D(data["x"].toDouble(),
                                    data["y"].toDouble(),
                                    data["z"].toDouble()));
    solarsys->generateFromJson(data);

    return solarsys;
}

QString SolarSistem::name() const
{
    return Name;
}

void SolarSistem::setName(QString name)
{
    Name = name;
}

QVector3D SolarSistem::position() const
{
    return Transform->translation();
}

void SolarSistem::setPosition(QVector3D position)
{
    if(!qFuzzyCompare(position,Transform->translation())){
        Transform->setTranslation(position);
        emit positionChanged();
    }
}

Qt3DCore::QTransform *SolarSistem::transform() const
{
    return Transform;
}

void SolarSistem::setTransform(Qt3DCore::QTransform *transform)
{
    if(Transform != transform){
        Transform = transform;
        addComponent(Transform);
        emit transformChanged();
    }
}

void SolarSistem::generateFromJson(QJsonObject solarSystemData)
{
    stars.append(star::generateStarFromJson(solarSystemData,this));
}

float SolarSistem::getPlanetRadius(QString name) const
{
    Q_FOREACH(star*sun,stars){
        Q_FOREACH(planet*planeto,sun->getPlanets()){
            if(planeto->name()==name)
                return planeto->mesh()->radius();
        }
    }
    qCritical() << name << " not found"<<endl;
    return 0.0;
}

QVector3D SolarSistem::getPlanetPos(QString name) const
{
    Q_FOREACH(star*sun,stars){
        Q_FOREACH(planet*planeto,sun->getPlanets()){
            if(planeto->name()==name)
                return planeto->transform()->translation();
        }
    }
    qCritical() << name << " not found"<<endl;
    return QVector3D(0.0,0.0,0.0);
}

void SolarSistem::animate(float day)
{
    Q_FOREACH(star* sun,stars){
        sun->animate(day);
    }
}






#ifndef SPACESTATION_H
#define SPACESTATION_H

#include <QObject>
#include <QDebug>

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include <Qt3DAnimation>

class spacestation : public Qt3DCore::QEntity
{
    Q_OBJECT
public:
    explicit spacestation(Qt3DCore::QEntity *parent = nullptr);
    static spacestation* generateFromJson(QJsonObject data,Qt3DCore::QEntity* parent);
signals:

public slots:
};

#endif // SPACESTATION_H

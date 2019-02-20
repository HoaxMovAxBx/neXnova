#include "spacestation.h"

spacestation::spacestation(Qt3DCore::QEntity *parent) : Qt3DCore::QEntity(parent)
{

}

spacestation *spacestation::generateFromJson(QJsonObject data, Qt3DCore::QEntity *parent)
{
    //!TODO implement
    //!
    //!
    qDebug() << "Spacesttion  "<<data<<endl;
    return nullptr;
}

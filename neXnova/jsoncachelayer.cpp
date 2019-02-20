#include "jsoncachelayer.h"

JsonCacheLayer::JsonCacheLayer(connection *conn, QObject *parent)
    : QThread(parent),conn(conn)
{
    planetBarIds.clear();
}

void JsonCacheLayer::processUniverseData(QJsonObject unidata)
{
    Q_FOREACH(QJsonValue sun,unidata["stars"].toArray()){

        QJsonObject sundata = sun.toObject();

        QString recivedid = sundata["id"].toString();
        qInfo() << "Cached item "<<recivedid<<endl;
        if(!cache.keys().contains(recivedid)){
            cache.insert(recivedid,sundata);
            checkPlanetBarInfo(sundata);
            emit dataToRender(sundata);
            return;
        }
        if(cache.values().contains(sundata))
            return;
        cache[recivedid] = sundata;
        checkPlanetBarInfo(sundata);
        emit changedItem(recivedid,sundata);
        }
    }

void JsonCacheLayer::requestSolarSystem(QString id)
{
    if(cache.contains(id)){
        qDebug() << "loading "<<id<<" from cache "<<endl;
        emit dataToRender(cache[id]);
        return;
    }
    qDebug() << "loading "<<id<<" from server "<<endl;
    QMetaObject::invokeMethod(conn,"requestSolarSystem",
                              Qt::QueuedConnection,Q_ARG(QString,id));

}


void JsonCacheLayer::run()
{
    qDebug()<<"Cache Initialized "<<endl;
    connect(conn,&connection::universeData,
            this,&JsonCacheLayer::processUniverseData,
            Qt::QueuedConnection);
    exec();
}

void JsonCacheLayer::checkPlanetBarInfo(QJsonObject ssdata)
{
    QString user;
    qDebug() << "Requesting username"<<endl;
    QMetaObject::invokeMethod(conn,"loggedUser",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QString,user));

    qDebug() << "Recived username "<<user<<endl;

    if(user.isEmpty()){
        qCritical() << "Empty user for planet bar generation "<<endl;
        return;
    }
    QJsonArray planetos = ssdata["planets"].toArray();
    Q_FOREACH(QJsonValue planeto,planetos){
        QJsonObject planetoobj = planeto.toObject();
        if(planetoobj["user"].toString() == user){
            if(planetBarIds.keys().contains(planetoobj["id"].toString())){
                if(planetBarIds[planetoobj["id"].toString()]!=planetoobj["type"].toInt()){
                    qDebug() << "Planet "<<planetoobj["name"].toString()
                             <<" changed his type from "
                             <<planetBarIds[planetoobj["id"].toString()]
                             <<" to "
                             <<planetoobj["type"].toInt()
                             <<endl;
                    planetBarIds[planetoobj["id"].toString()] = planetoobj["type"].toInt();
                    emit planetBarData(planetoobj);
                }
            }
            else{
               planetBarIds.insert(planetoobj["id"].toString(),planetoobj["type"].toInt());
               qDebug() << "Sending "<<planetoobj["name"].toString() <<" to planetbar"<<endl;
               emit planetBarData(planetoobj);
            }
        }
    }

}

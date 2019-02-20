#ifndef JSONCACHELAYER_H
#define JSONCACHELAYER_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QSettings>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "connection.h"
#include "qmlengine.h"

class JsonCacheLayer : public QThread
{
    Q_OBJECT
public:
    explicit JsonCacheLayer(connection * conn,QObject *parent = nullptr);

signals:
    void changedItem(QString id, QJsonObject newvalue);
    void dataToRender(QJsonObject data);
    void planetBarData(QJsonObject data);

public slots:
    void processUniverseData(QJsonObject unidata);
    void requestSolarSystem(QString id);

private:
    void run() override;
    void checkPlanetBarInfo(QJsonObject ssdata);

    QHash<QString,QJsonObject>cache;
    QHash<QString,int>planetBarIds;
    connection * conn;
};

#endif // JSONCACHELAYER_H

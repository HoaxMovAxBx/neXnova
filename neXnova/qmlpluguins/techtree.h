#ifndef TECHTREE_H
#define TECHTREE_H

#include <QObject>
#include <QQuickItem>
#include <QQuickPaintedItem>
#include <QPainter>
#include <QPainterPath>
#include <QImage>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <QDebug>
#include <QFile>

class techtree : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(int itemside READ itemside WRITE setItemside NOTIFY itemsideChanged)
public:
    explicit techtree(QQuickItem *parent = nullptr);
    int itemside();
    void setItemside(int side);

protected:
    void paint(QPainter * painter);

signals:
    void itemsideChanged();

public slots:
    void setTechData(QJsonObject tdata);

private:
    QJsonObject data;
    int ItemSide;

};

#endif // TECHTREE_H

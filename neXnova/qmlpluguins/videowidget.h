#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QObject>
#include <QQuickItem>
#include <QQuickPaintedItem>
#include <QPainter>

#include "videochat.h"

class videowidget : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(videochat* framesource READ framesource WRITE setFramesource NOTIFY framesourceChanged)

public:
    explicit videowidget(QQuickItem *parent = nullptr);
    ~videowidget();

    videochat* framesource() const;
    void setFramesource(videochat * source);

protected:
    void paint(QPainter *painter);

private:

    QImage current_frame;
    videochat * Framesource;

signals:
    void framesourceChanged();

public slots:
    void newframe(QImage frame);
};

#endif // VIDEOWIDGET_H

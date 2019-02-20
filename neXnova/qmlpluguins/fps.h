#ifndef FPS_H
#define FPS_H

#include <QQuickPaintedItem>
#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <QElapsedTimer>
#include <QString>
#include <QTime>

#define PI 3.1415926535

class fps : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal framespersecond READ framespersecond NOTIFY framespersecondChanged)
public:
    explicit fps(QObject *parent = nullptr);


    void newFrame();
    qreal framespersecond();

signals:
    void framespersecondChanged();


private:
    QElapsedTimer frameRenderingTime;
    qreal Fps;
};

class fpswidget : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(fps* fpsCalculator READ fpsCalculator WRITE setFpsCalculator NOTIFY fpsCalculatorChanged)
public:
    explicit fpswidget(QQuickItem * parent = nullptr);
    ~fpswidget();

    void setFpsCalculator(fps * grabber);
    fps * fpsCalculator() const;
public slots:
    void newfps();

signals:
    void fpsCalculatorChanged();

protected:
    void paint(QPainter* painter);    
    void drawThresholdLine(QPainter* painter);
    void drawCrown(QPainter* painter);
    void drawUnits(QPainter* painter);
    void drawLabel(QPainter* painter);
    void drawCircularBar(QPainter* painter);
    void drawTicks(QPainter* painter);
    void drawCoverGlass(QPainter* painter);

private:
    fps * fpsgrabber;
};

#endif // FPS_H






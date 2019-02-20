#include "fps.h"

fps::fps(QObject *parent) : QObject(parent)
{
    Fps = 0.0;
}

void fps::newFrame()
{
    quint64 time = frameRenderingTime.restart();
    qreal tempfps = 1000.0/time;
    if(tempfps!=Fps){
        Fps = tempfps;
        emit framespersecondChanged();
    }
}

qreal fps::framespersecond()
{
    return Fps;
}


fpswidget::fpswidget(QQuickItem *parent):QQuickPaintedItem(parent)
{
    fpsgrabber = nullptr;
}

fpswidget::~fpswidget()
{

}

void fpswidget::setFpsCalculator(fps *grabber)
{
    if(fpsgrabber!=grabber){
        fpsgrabber = grabber;
        connect(fpsgrabber,&fps::framespersecondChanged,this,&fpswidget::newfps,Qt::DirectConnection);
        emit fpsCalculatorChanged();
    }
}

fps *fpswidget::fpsCalculator() const
{
    return fpsgrabber;
}

void fpswidget::newfps()
{
    update();
}

void fpswidget::paint(QPainter *painter)
{

    painter->setRenderHints(QPainter::Antialiasing);

    int side = qMin(width(), height());
    painter->setViewport((width() - side) / 2, (height() - side) / 2,side, side);
    painter->setWindow(-50, -50, 100, 100);

    drawCircularBar(painter);
    drawTicks(painter);
    drawCoverGlass(painter);
    drawLabel(painter);
    drawUnits(painter);
    drawCrown(painter);
    //drawThresholdLine(painter);

}




void fpswidget::drawCrown(QPainter *painter)
{
    painter->save();
    QRectF rectangle(-47, -47, 94, 94);//! NOTE rev
    int startAngle = 30 * 16;
    int spanAngle = 390 * 16;
    QLinearGradient linearGrad(QPointF(-47, -47), QPointF(94, 94));
    linearGrad.setColorAt(0, Qt::white);
    linearGrad.setColorAt(0.3, QColor(60,60,60,250));
    QPen pen;
    QBrush brush=QBrush(linearGrad);
    pen.setBrush(brush);
    pen.setWidth(3);
    painter->setPen(pen);
    painter->drawArc(rectangle, startAngle, spanAngle);
    painter->restore();
}


void fpswidget::drawCoverGlass(QPainter *painter)
{
    painter->save();
    QLinearGradient linearGrad(QPointF(-15, -45), QPointF(90, 75));
    linearGrad.setColorAt(0, QColor(255,255,255,30));
    linearGrad.setColorAt(0.3, QColor(120,120,120,20));
    QBrush brush=QBrush(linearGrad);
    QPen pen;
    pen = QColor(0,0,0,0);
    pen.setBrush(brush);
    pen.setWidth(0);
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawEllipse(-45,-45,90,90);//! NOTE rev
    painter->restore();
}



void fpswidget::drawTicks(QPainter *painter)
{
    painter->save();
    QPen pen=QPen(QColor(0,0,0,225));
    pen.setWidth(2);
    painter->setPen(pen);
    int nSteps=60;
    painter->rotate(30) ;
    double angleStep= 330.0 /60.0;
    for ( int i=0; i<=nSteps;i++ )
    {
        painter->drawLine(30,0,40,0 );
        painter->rotate( angleStep );
    }
    painter->restore();

    painter->save();
    pen.setWidth(1);
    painter->setPen(pen);
    double spanAngle= (60.0/90.0)*((fpsgrabber == nullptr ? 0.0 : fpsgrabber->framespersecond())*5.0+30.0) + 30.0;
    painter->drawArc(-28,-28,56,56,30*16, spanAngle*16);
    painter->restore();

}

void fpswidget::drawThresholdLine(QPainter *painter)
{
    QPen pen;

    double thresholdAngle = (60.0/90.0)*((fpsgrabber == nullptr ? 0.0 : fpsgrabber->framespersecond())*5.0+30.0);
    pen.setWidth(2);
    pen.setColor(Qt::red);

    painter->setPen(pen);
    painter->drawArc(-40,-40,80,80,(int)thresholdAngle*16,(int)(-thresholdAngle+360)*16);

}

void fpswidget::drawCircularBar(QPainter *painter)
{
    painter->save();
    double valueAngle = (60.0/90.0)*((fpsgrabber == nullptr ? 0.0 : fpsgrabber->framespersecond())*5.0+30.0);
    QRadialGradient haloGradientw(0, 0, 49, 0, 0);
    QColor color(20,200,50,220);
    haloGradientw.setColorAt(0.7, color);
    haloGradientw.setColorAt(0.9, Qt::black);
    QBrush brush=QBrush(haloGradientw);
    QPen pen = QPen(brush,5);
    painter->setPen(pen);
    painter->drawArc(-35,-35,70,70,(int)(30.0*16),(int)((valueAngle-30.0)*16));
    painter->restore();

}


void fpswidget::drawLabel(QPainter *painter)
{
    painter->save();

    QRectF labelRect=QRectF(-15,10,35,20);

    painter->setBrush(QColor(20,30,200,200));
    painter->setPen(QColor(20,30,200,200));

    QString no;
    no = QString("%1").arg(fpsgrabber == nullptr ? 0.0 : fpsgrabber->framespersecond(), 0, 'f', 2);

    painter->drawText(labelRect,Qt::AlignCenter, no);
    painter->restore();
}


void fpswidget::drawUnits(QPainter *painter)
{
    painter->save();

    QRectF unitsRect=QRectF(-20,-30,40,20);
    painter->setBrush(QColor(20,30,200,200));
    painter->setPen(QColor(20,30,200,200));
    painter->drawText(unitsRect,Qt::AlignCenter,"fps");
    painter->restore();
}


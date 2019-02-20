#include "videowidget.h"

videowidget::videowidget(QQuickItem * parent):QQuickPaintedItem(parent)
{
    Framesource = nullptr;
}

videowidget::~videowidget()
{

}

videochat *videowidget::framesource() const
{
    return Framesource;
}

void videowidget::setFramesource(videochat *source)
{
    if(Framesource!=source){
        Framesource = source;
        connect(Framesource,SIGNAL(newFrame(QImage)),this,SLOT(newframe(QImage)));
        emit framesourceChanged();
        qDebug()<<"framesource"<<endl;
    }
}

void videowidget::paint(QPainter *painter)
{
    qDebug() << "rendering"<<endl;
    current_frame = current_frame.scaled(width(),
                                         height(),
                                         Qt::KeepAspectRatio,
                                         Qt::SmoothTransformation);

    painter->setRenderHints(QPainter::Antialiasing);
    painter->setRenderHints(QPainter::SmoothPixmapTransform);
    painter->drawImage(0, 0,current_frame);
}

void videowidget::newframe(QImage frame)
{
    qDebug() << "New Frame"<<endl;
    current_frame = frame;
    if(frame.isNull())
        qCritical() << "Null frame "<<endl;
    else
        qDebug() <<"Frame size "<<frame.sizeInBytes()<<endl;
    update();
}

// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include "circleimage.h"

CircleImage::CircleImage(QQuickItem* parent)
  : QQuickPaintedItem(parent), 
    color(QColor(0,0,0,0)),imageSource("")
{
    Disabled = false;
}

CircleImage::~CircleImage() 
{

}

QColor CircleImage::backgroundcolor() const
{
    return color;
}

void CircleImage::setBackgroundcolor(const QColor &backgroundcolor)
{
    if (backgroundcolor != color) {
        color = backgroundcolor;
        emit backgroundcolorChanged();
        update();
    }
}

QString CircleImage::source() const
{
    return imageSource;
}

void CircleImage::setSource(const QString &source)
{

    QUrl sourceurl = QUrl::fromUserInput(source);
    QString corrected_source = sourceurl.path();
    if(!corrected_source.startsWith(':'))
        corrected_source.prepend(':');

    if(imageSource != corrected_source) {
        imageSource = corrected_source;
        emit sourceChanged();
        update();
    }

}

bool CircleImage::disabled() const
{
    return Disabled;
}

void CircleImage::setDisabled(bool disabled)
{
    if(Disabled!=disabled){
        Disabled = disabled;
        update();
        emit disabledChanged();
    }
}


void CircleImage::paint(QPainter* painter) 
{
    QFile file(imageSource);
    if (!file.exists()){
        qCritical() << "Error opening round image file "<<imageSource<<endl;
        return;
    }
    QImage image(imageSource);
    if (image.isNull()){
        qCritical() <<"error loading image "<<imageSource<<" make sure the format is "
                                                           "suported "<<endl;
        return;
    }

    image = image.scaled(width()+10.0, height()+10.0,Qt::KeepAspectRatio,Qt::SmoothTransformation);

    if(Disabled){
        qDebug() << "Using grayscale "<<endl;
        image=image.convertToFormat(QImage::Format_Grayscale8);
    }

    QPainterPath path;
    path.addEllipse(0, 0, width(), height());
    painter->setRenderHints(QPainter::Antialiasing);
    painter->setRenderHints(QPainter::SmoothPixmapTransform);
	painter->setClipping(true);
	painter->setClipPath(path);
    painter->drawImage(0, 0,image,5.0,5.0);
	painter->clipPath();

}

#include "techtree.h"

techtree::techtree(QQuickItem *parent) : QQuickPaintedItem(parent)
{

}

int techtree::itemside()
{
    return ItemSide;
}

void techtree::setItemside(int side)
{
    if(side!=ItemSide){
        ItemSide = side;
        emit itemsideChanged();
    }
}

void techtree::paint(QPainter *painter)
{
    QJsonArray techs = data["tech"].toArray();

    painter->setRenderHints(QPainter::Antialiasing);
    painter->setRenderHints(QPainter::SmoothPixmapTransform);


    Q_FOREACH(auto tech,techs){

        QString imageSource = tech["icon"].toString();

        QFile file(imageSource);
        if (!file.exists()){
            qCritical() << "Error opening round image file "<<imageSource<<endl;
            continue;
        }
        QImage image(imageSource);
        if (image.isNull()){
            qCritical() <<"error loading image "<<imageSource<<" make sure the format is "
                                                               "suported "<<endl;
            continue;
        }

        image = image.scaled(width()+10.0, height()+10.0,Qt::KeepAspectRatio,Qt::SmoothTransformation);

        if(tech["level"].toInt() == 0){
            qDebug() << "Using grayscale "<<endl;
            image=image.convertToFormat(QImage::Format_Grayscale8);
        }

        // Image
        QPainterPath path;
        path.addEllipse(tech["x"].toInt(), tech["y"].toInt(), ItemSide, ItemSide);

        painter->setClipping(true);
        painter->setClipPath(path);
        painter->drawImage(0, 0,image,5.0,5.0);
        painter->clipPath();

        if(tech["developing"].toBool()){
            QPainterPath devarc;
            devarc.arcTo(tech["x"].toInt(), tech["y"].toInt(), ItemSide, ItemSide,0.0,
                    devarc.angleAtPercent(tech["devprogress"].toDouble()));

            painter->save();
            painter->drawPath(devarc);
            painter->restore();
        }
        else{//NOTE  probe
            for(int i = 0; i < tech["level"].toInt() * 2; i++){
                if(i%2==1){
                    painter->drawLine(tech["x"].toInt() - 10 + ItemSide / 2,tech["y"].toInt() + + ItemSide / 2,
                            tech["x"].toInt()+ ItemSide / 2 ,tech["y"].toInt() + ItemSide / 2 );
                    painter->rotate(360.0*i/50.0);
                }
            }
        //TODO draw path betwen items and setup pens and broushes

        }

        QJsonArray deps = tech["dependencies"].toArray();
        Q_FOREACH(auto dep,deps){
            Q_FOREACH(auto deptech,techs){
                if(deptech["name"].toString() == dep["name"].toString()){
                    painter->drawLine(tech["x"].toInt() + ItemSide / 2,tech["y"].toInt() + ItemSide / 2,
                            deptech["x"].toInt() + ItemSide / 2,deptech["x"].toInt() + ItemSide / 2);
                }
            }
        }

    }

}
void techtree::setTechData(QJsonObject tdata)
{
    data = tdata;
    update();
}

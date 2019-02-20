#include "imageprovider.h"

AsyncImageResponse::AsyncImageResponse(const QString &id, const QSize &requestedSize, AsyncImageProvider *parent)
    : id(id), requestedSize(requestedSize),parent(parent)
{
   setAutoDelete(false);
}

QQuickTextureFactory * AsyncImageResponse::textureFactory() const
{
    return QQuickTextureFactory::textureFactoryForImage(image);
}

void AsyncImageResponse::run(){

    QMetaObject::invokeMethod(parent,"getImageFromCahe",
                              Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QImage,image),
                              Q_ARG(QString,id));
    char i = 0;
    if(image.isNull())
        QMetaObject::invokeMethod(parent,"requestToServer",Q_ARG(QString,id));
    while(image.isNull() && i < 12){
        qWarning() << "Waiting for image "<<id<<endl;
        QThread::sleep(10);
        QMetaObject::invokeMethod(parent,"getImageFromCahe",
                                  Qt::BlockingQueuedConnection,
                                  Q_RETURN_ARG(QImage,image),
                                  Q_ARG(QString,id));
    }
    if(image.isNull()){
        qFatal("Image timeout \n");
        emit finished();
        return;
    }

    if (requestedSize.isValid())
        image = image.scaled(requestedSize);

    emit finished();
}

AsyncImageProvider::AsyncImageProvider()
{
    pool = new QThreadPool;
}

QQuickImageResponse * AsyncImageProvider::requestImageResponse(const QString &id,
                                                               const QSize &requestedSize){
    AsyncImageResponse *response = new AsyncImageResponse(id, requestedSize, this);
    pool->start(response);
    return response;
}

QImage AsyncImageProvider::getImageFromCahe(QString key) const
{
    if(cache.keys().contains(key))
        return cache[key];
    return QImage();
}

void AsyncImageProvider::requestToServer(QString id)
{
    //! TODO implement
    qInfo() << "Requesting image "<<id<<" to server "<<endl;
}

void AsyncImageProvider::avatarLoadFromMsg(QJsonObject msg)
{
    if(!msg.contains("sender") || !msg.contains("avatar"))
        return;

    QByteArray imgdata = msg["avatar"].toString().toLatin1();

    qInfo() <<"Img size "<<imgdata.size()<<endl;

    const QByteArray decodeddata = QByteArray::fromBase64(imgdata);
    imgdata.clear();

    qDebug() <<"decoded size "<<decodeddata.size()<<endl;

    QPixmap*map = new QPixmap();
    if(!map->loadFromData(decodeddata))
        qCritical() << "Error loading avatar "<<decodeddata.size()<<" Bytes"<<endl;;


    QImage img = map->toImage();

    cache.insert(msg["sender"].toString(),img);

    qDebug() << "Avatar for user " << msg["sender"].toString()
             << " loaded in cache "<< map->size() << endl;

}

void AsyncImageProvider::avatarLoadFromUsersInfo(QJsonObject usersinfo)
{
    QJsonArray users = usersinfo["users"].toArray();
    Q_FOREACH(QJsonValue user,users){
       QJsonObject info = user.toObject();
       if(info.contains("sender") && info.contains("avatar")){

           QByteArray imgdata = info["avatar"].toString().toLatin1();

           qInfo() <<"Img size "<<imgdata.size()<<endl;

           const QByteArray decodeddata = QByteArray::fromBase64(imgdata);
           imgdata.clear();

           qDebug() <<"decoded size "<<decodeddata.size()<<endl;

           QPixmap*map = new QPixmap();
           if(!map->loadFromData(decodeddata))
               qCritical() << "Error loading avatar "<<decodeddata.size()<<" Bytes"<<endl;;

           QImage img = map->toImage();

           cache.insert(info["sender"].toString(),img);
       }
    }
}















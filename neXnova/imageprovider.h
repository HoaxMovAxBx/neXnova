#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QObject>
#include <QQuickImageProvider>
#include <QMetaObject>

#include <QDebug>
#include <QImage>
#include <QThreadPool>
#include <QMutex>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

class AsyncImageProvider;

class AsyncImageResponse : public QQuickImageResponse, public QRunnable
{
    Q_OBJECT
public:
    AsyncImageResponse(const QString &id, const QSize &requestedSize,AsyncImageProvider*parent);

    QQuickTextureFactory * textureFactory() const override;
    void run() override;


private:
    QString id;
    QSize requestedSize;
    QImage image;
    AsyncImageProvider * parent;
};

class AsyncImageProvider : public QObject , public QQuickAsyncImageProvider
{
    Q_OBJECT
public:
    AsyncImageProvider();
    QQuickImageResponse *requestImageResponse(const QString &id, const QSize &requestedSize) override;

public slots:
    QImage getImageFromCahe(QString key) const;
    void requestToServer(QString id);
    void avatarLoadFromMsg(QJsonObject msg);
    void avatarLoadFromUsersInfo(QJsonObject usersinfo);
private:
    QThreadPool * pool;
    QHash<QString,QImage>cache;
};


#endif // IMAGEPROVIDER_H

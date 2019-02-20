#ifndef HTTP_H
#define HTTP_H

#include <QObject>
#include <QThread>

#include <qhttpserver/qhttpserverfwd.h>
#include <qhttpserver/qhttpserver.h>
#include <qhttpserver/qhttpconnection.h>
#include <qhttpserver/qhttpresponse.h>
#include <qhttpserver/qhttprequest.h>

class http : public QThread
{
    Q_OBJECT
public:
    explicit http(QObject *parent = nullptr);

private:
    void run() override;

    QHttpServer * httpserver;
signals:

private slots:
    void ResposeRequest(QHttpRequest *request, QHttpResponse *response);
    void incommingData(QHttpConnection *conn, QString data);
    void connectionClosed(QHttpConnection *conn);
};

#endif // HTTP_H

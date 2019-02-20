#include "http.h"

http::http(QObject *parent) : QThread(parent)
{

}

void http::run()
{
    httpserver = new QHttpServer;
    httpserver->listen(8080);

    connect(httpserver,&QHttpServer::newRequest,
            this,&http::ResposeRequest,Qt::DirectConnection);
    connect(httpserver,&QHttpServer::webSocketDataReady,
            this,&http::incommingData,Qt::DirectConnection);
    connect(httpserver,&QHttpServer::webSocketConnectionClose,
            this,&http::connectionClosed,Qt::DirectConnection);

    exec();
}

void http::ResposeRequest(QHttpRequest *request, QHttpResponse *response)
{
    qDebug() << "Request recived "<<request->path() <<endl;
}

void http::incommingData(QHttpConnection *conn, QString data)
{
    qDebug() << "inndata recived " <<endl;
}

void http::connectionClosed(QHttpConnection *conn)
{
    qDebug() << "connect closed " <<endl;
}

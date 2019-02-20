#include <QCoreApplication>
#include <QFile>
#include <QDateTime>
#include <QDebug>

#include <QSqlDatabase>
#include <QSqlError>
#include <QString>

#include "server.h"
#include "http.h"

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

#define LOGFILE "neXnova.log"

static QFile logf(LOGFILE);

void msgHandler(QtMsgType type,const QMessageLogContext &context,
                    const QString &localMsg);



int main(int argc, char *argv[])
{
    qRegisterMetaType<client_thread>("client_thread");

    qInstallMessageHandler(msgHandler);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("neXnova");
    db.setUserName("root");
    db.setPassword("");
    bool ok = db.open();

    if(!ok)
        qCritical() << "Error opening db "<<db.lastError().text() << endl;
    else qDebug() <<"Ok db " << endl;



    QCoreApplication a(argc, argv);
    http s;
    s.start();
    server x;

    return a.exec();
}

void msgHandler(QtMsgType type,const QMessageLogContext &context,
                    const QString &localMsg)
{
    if(!logf.isOpen())
        logf.open(QIODevice::ReadWrite);
    switch (type) {
    case QtDebugMsg:
        qDebug() << GRN << "[Debug] : "<< WHT << QDateTime::currentDateTime().toString().toLatin1().data()
                 << QString("%1 (%2:%3, %4)\n").arg(QString(localMsg).remove("\n"))
                    .arg(context.file).arg(context.line).arg(context.function)
                    .toLatin1().data()
                 << RESET;
        logf.write(QString("[Debug] : %5 %1 (%2:%3, %4) \n").arg(localMsg)
                   .arg(context.file).arg(context.line).arg(context.function)
                   .arg(QDateTime::currentDateTime().toString())
                   .toLatin1().data());
        break;
    case QtWarningMsg:
        qDebug() << YEL << "[Warning] : "<< WHT << QDateTime::currentDateTime().toString().toLatin1().data()
                 << QString("%1 (%2:%3, %4)\n").arg(QString(localMsg).remove("\n"))
                    .arg(context.file).arg(context.line).arg(context.function)
                    .arg(QDateTime::currentDateTime().toString())
                    .toLatin1().data()
                    << RESET;

        logf.write(QString("[Warning] : %5 %1 (%2:%3, %4)\n").arg(localMsg)
                   .arg(context.file).arg(context.line).arg(context.function)
                   .arg(QDateTime::currentDateTime().toString())
                   .toLatin1().data());
        break;
    case QtCriticalMsg:
        qDebug() << BLU << "[Critical] "<<WHT << QDateTime::currentDateTime().toString().toLatin1().data()
                 << QString("%1 (%2:%3, %4)\n").arg(QString(localMsg).remove("\n"))
                    .arg(context.file).arg(context.line).arg(context.function)
                    .toLatin1().data()
                    << RESET;

        logf.write(QString("[Critical] : %5 %1 (%2:%3, %4)\n").arg(localMsg)
                   .arg(context.file).arg(context.line).arg(context.function)
                   .arg(QDateTime::currentDateTime().toString())
                   .toLatin1().data());
        break;
    case QtFatalMsg:
        qDebug() << RED << "[Fatal] "<<WHT<< QDateTime::currentDateTime().toString().toLatin1().data()
                 << QString("%1 (%2:%3, %4)\n").arg(QString(localMsg).remove("\n"))
                    .arg(context.file).arg(context.line).arg(context.function)
                    .arg(QDateTime::currentDateTime().toString())
                    .toLatin1().data()
                    << RESET;

        logf.write(QString("[Fatal] : %5 %1 (%2:%3, %4)\n").arg(localMsg)
                   .arg(context.file).arg(context.line).arg(context.function)
                   .toLatin1().data());
        break;
    case QtInfoMsg:
        qDebug() << WHT << "[Info] " << QDateTime::currentDateTime().toString().toLatin1().data()
                 << QString("%1 (%2:%3, %4)\n").arg(QString(localMsg).remove("\n"))
                    .arg(context.file).arg(context.line).arg(context.function)
                    .toLatin1().data()
                    << RESET;

        logf.write(QString("[Info] : %5  %1 (%2:%3, %4)\n").arg(localMsg)
                   .arg(context.file).arg(context.line).arg(context.function)
                   .arg(QDateTime::currentDateTime().toString())
                   .toLatin1().data());
        break;
    }
}

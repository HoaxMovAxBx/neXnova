#include "qmlengine.h"
#include "audioengine.h"

#include <QApplication>
#include <QGuiApplication>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QString>


#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

#define LOGFILE "neXnova.log"

static QFile logfile(LOGFILE);

void msgHandler(QtMsgType type,const QMessageLogContext &context,
                    const QString &localMsg);


int main(int argc, char *argv[])
{

    qInstallMessageHandler(msgHandler);

    QGuiApplication::setApplicationName("neXnova");
    QGuiApplication::setOrganizationName("H0ax-tech");
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication a(argc, argv);

    qmlengine xeng;
    return a.exec();
}

void msgHandler(QtMsgType type,const QMessageLogContext &context,
                    const QString &localMsg)
{
    if(!logfile.isOpen())
        logfile.open(QIODevice::ReadWrite);
    switch (type) {
    case QtDebugMsg:
        qDebug() << GRN << "[Debug] : "<< WHT << QDateTime::currentDateTime().toString().toLatin1().data()
                 << QString("%1 (%2:%3, %4)\n").arg(QString(localMsg).remove("\n"))
                    .arg(context.file).arg(context.line).arg(context.function)
                    .toLatin1().data()
                 << RESET;
        logfile.write(QString("[Debug] : %5 %1 (%2:%3, %4) \n").arg(localMsg)
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

        logfile.write(QString("[Warning] : %5 %1 (%2:%3, %4)\n").arg(localMsg)
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

        logfile.write(QString("[Critical] : %5 %1 (%2:%3, %4)\n").arg(localMsg)
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

        logfile.write(QString("[Fatal] : %5 %1 (%2:%3, %4)\n").arg(localMsg)
                   .arg(context.file).arg(context.line).arg(context.function)
                   .toLatin1().data());
        break;
    case QtInfoMsg:
        qDebug() << WHT << "[Info] " << QDateTime::currentDateTime().toString().toLatin1().data()
                 << QString("%1 (%2:%3, %4)\n").arg(QString(localMsg).remove("\n"))
                    .arg(context.file).arg(context.line).arg(context.function)
                    .toLatin1().data()
                    << RESET;

        logfile.write(QString("[Info] : %5  %1 (%2:%3, %4)\n").arg(localMsg)
                   .arg(context.file).arg(context.line).arg(context.function)
                   .arg(QDateTime::currentDateTime().toString())
                   .toLatin1().data());
        break;
    }
}

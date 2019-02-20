#include "logger.h"


void msgHandler(QtMsgType type,const QMessageLogContext &context,
                    const QString &localMsg)
{
    if(!logf.isOpen())
        logf.open(QIODevice::WriteOnly);
    switch (type) {
    case QtDebugMsg:
        qDebug() << GRN << "[Debug] "<<RESET
                 << QString("%1 (%2:%3, %4)\n").arg(QString(localMsg).remove("\n"))
                    .arg(context.file).arg(context.line).arg(context.function)
                    .toLatin1().data();
        logf.write(QString("[Debug] : %1 (%2:%3, %4)\n").arg(localMsg.constData())
                   .arg(context.file).arg(context.line).arg(context.function)
                   .toLatin1().data());
        break;
    case QtWarningMsg:
        qDebug() << YEL << "[Warning] "<<RESET
                 << QString("%1 (%2:%3, %4)\n").arg(QString(localMsg).remove("\n"))
                    .arg(context.file).arg(context.line).arg(context.function)
                    .toLatin1().data();

        logf.write(QString("[Warning] : %1 (%2:%3, %4)\n").arg(localMsg.constData())
                   .arg(context.file).arg(context.line).arg(context.function)
                   .toLatin1().data());
        break;
    case QtCriticalMsg:
        qDebug() << BLU << "[Critical] "<<RESET
                 << QString("%1 (%2:%3, %4)\n").arg(QString(localMsg).remove("\n"))
                    .arg(context.file).arg(context.line).arg(context.function)
                    .toLatin1().data();

        logf.write(QString("[Critical] : %1 (%2:%3, %4)\n").arg(localMsg.constData())
                   .arg(context.file).arg(context.line).arg(context.function)
                   .toLatin1().data());
        break;
    case QtFatalMsg:
        qDebug() << RED << "[Fatal] "<<RESET
                 << QString("%1 (%2:%3, %4)\n").arg(QString(localMsg).remove("\n"))
                    .arg(context.file).arg(context.line).arg(context.function)
                    .toLatin1().data();

        logf.write(QString("[Fatal] : %1 (%2:%3, %4)\n").arg(localMsg.constData())
                   .arg(context.file).arg(context.line).arg(context.function)
                   .toLatin1().data());
        break;
    }
}

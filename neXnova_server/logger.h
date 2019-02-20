#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QDebug>
#include <QFile>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

#define LOGFILE "flavor.log"

static QFile logf(LOGFILE);

void msgHandler(QtMsgType type,const QMessageLogContext &context,
                    const QString &localMsg);


#endif // LOGGER_H

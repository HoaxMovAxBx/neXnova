#ifndef TANOSGAMEENGINE_H
#define TANOSGAMEENGINE_H

#include <QObject>
#include <QThread>

class tanosGameEngine : public QThread
{
    Q_OBJECT
public:
    explicit tanosGameEngine(QObject *parent = nullptr);

private:
    void run()override;

signals:

public slots:
};

#endif // TANOSGAMEENGINE_H

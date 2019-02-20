#ifndef RAVENBATTLEENGINE_H
#define RAVENBATTLEENGINE_H

#include <QObject>
#include <QThread>

class ravenBattleEngine : public QThread
{
    Q_OBJECT
public:
    explicit ravenBattleEngine(QObject *parent = nullptr);


private:
    void run() override;

signals:

public slots:
};

#endif // RAVENBATTLEENGINE_H

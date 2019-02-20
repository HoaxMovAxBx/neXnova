#ifndef RAZESELECTOR_H
#define RAZESELECTOR_H

#include <QObject>
#include <QAbstractListModel>
#include <QUrl>

class Raze
{

public:
    Raze(const QString & name = "Voltra", const QString & backgroundImage = QString("qrc:/"),
         const QString & logo = QString("qrc:/"),const QString & qmlfile = QString("qrc:/"));

    QString name() const;
    QString backgroundImage() const;
    QString logo() const;
    QString qmlfile() const;

private:
    QString namex;
    QString backgroundImagex;
    QString logox;
    QString qmlfilex;

};


class RazeSelector : public QAbstractListModel
{
    Q_OBJECT

    enum RazeRoles {
        NameRole = Qt::UserRole + 1,
        BackgroundRole,
        LogoRole,
        QmlFileRole
    };


public:
    explicit RazeSelector(QObject *parent = nullptr);
    void razeGenerator();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<Raze> razes;

signals:

public slots:


};

#endif // RAZESELECTOR_H

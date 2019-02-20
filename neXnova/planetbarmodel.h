#ifndef PLANETBARMODEL_H
#define PLANETBARMODEL_H

#include <QObject>
#include <QPixmap>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <QAbstractListModel>

class planetbarmodel : public QAbstractListModel
{
    enum PlanetsModelRoles {
        NameRole = Qt::UserRole + 1,
        PlanetIconRole
    };

    Q_OBJECT
public:
    explicit planetbarmodel(QObject *parent = nullptr);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QJsonArray planetsData;
signals:

public slots:
    void insertData(QJsonObject planeto);

};

#endif // PLANETBARMODEL_H

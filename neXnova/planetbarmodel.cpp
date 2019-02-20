#include "planetbarmodel.h"

planetbarmodel::planetbarmodel(QObject *parent) : QAbstractListModel(parent)
{

}

int planetbarmodel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return planetsData.count();
}

QVariant planetbarmodel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= planetsData.count())
        return QVariant();

    const QJsonObject & planetData = planetsData[index.row()].toObject();
    if (role == NameRole)
        return planetData["name"].toString();

    else if(role == PlanetIconRole){
        //! TODO implement correct
        QString path("qrc:/assets/images/earth.png");
        return path;
    }
    return QVariant();

}

QHash<int, QByteArray> planetbarmodel::roleNames() const
{
    QHash<int, QByteArray> rolenames;
    rolenames[NameRole] = "name";
    rolenames[PlanetIconRole]="planeticon";
    //! TODO descrition
    return rolenames;
}

void planetbarmodel::insertData(QJsonObject planeto)
{
    beginInsertRows(QModelIndex(),rowCount(),rowCount());
    planetsData.append(planeto);
    endInsertRows();
}


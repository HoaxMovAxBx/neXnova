#include "razeselector.h"

RazeSelector::RazeSelector(QObject *parent) : QAbstractListModel(parent)
{

}

void RazeSelector::razeGenerator()
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    razes << Raze("Voltra",QString("qrc:/assets/tlo.jpg"),
                  QString("qrc:/gui/razes/voltra.jpg"),
                  QString("qrc:/razesqml/voltra.qml"));

    razes << Raze("Gultra",QString("qrc:/assets/tlo.jpg"),
                  QString("qrc:/gui/razes/gultra.jpg"),
                  QString("qrc:/razesqml/gultra.qml"));


    endInsertRows();

}

int RazeSelector::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return razes.count();
}

QVariant RazeSelector::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= razes.count())
        return QVariant();

    const Raze & race = razes[index.row()];
    if (role == NameRole)
        return race.name();
    else if (role == LogoRole)
        return race.logo();
    else if(role == BackgroundRole)
        return race.backgroundImage();
    else if(role == QmlFileRole)
        return race.qmlfile();
    return QVariant();
}

QHash<int, QByteArray> RazeSelector::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[BackgroundRole] = "backgroundImage";
    roles[LogoRole]= "logo";
    roles[QmlFileRole] = "qmlfile";
    return roles;
}

Raze::Raze(const QString &name, const QString &backgroundImage, const QString &logo, const QString &qmlfile)
    :namex(name),backgroundImagex(backgroundImage),logox(logo),qmlfilex(qmlfile)
{

}

QString Raze::name() const
{
    return namex;
}

QString Raze::backgroundImage() const
{
    return backgroundImagex;
}

QString Raze::logo() const
{
    return logox;
}

QString Raze::qmlfile() const
{
    return qmlfilex;
}

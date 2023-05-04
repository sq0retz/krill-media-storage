

#include "TableModel.h"

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    v.resize(500, std::vector<QString>(500));
}

int TableModel::rowCount(const QModelIndex & /*parent*/) const
{
    return 500;
}

int TableModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 5;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
        return v[index.row()][index.column()];

    return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role ) const
{
    if (role == Qt::DisplayRole)
        if(orientation == Qt::Horizontal)
            switch(section){
            case 0: return QString("Название");
            case 1: return QString("Размер в байтах");
            case 2: return QString("Формат");
            case 3: return QString("Продолжительность");
            case 4: return QString("Путь");
            default:  return QVariant();
            }
    return QVariant();
}

bool TableModel::setData (const QModelIndex &index, const QVariant &value, int role )
{
    if (role == Qt::EditRole)
    {
        v[index.row()][index.column()] = QVariant(value).toString();
        emit dataChanged( index,index);// для обновления
        return true;

    }
    return false;

}

void TableModel::clear()
{
    int i = 0,j=0;
    for (;i<500;i++)
    {
        for (;j<500;j++){
            v[i][j] = "";}
    }


}



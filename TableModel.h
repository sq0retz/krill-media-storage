
#ifndef TABLEMODEL_H
#define TABLEMODEL_H
#include <QAbstractTableModel>
class TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    std::vector <std::vector <QString>> v ;


    explicit TableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role ) const override;
    bool setData (const QModelIndex &index, const QVariant &value, int role ) override ;
    void clear();
};
#endif // TABLEMODEL_H

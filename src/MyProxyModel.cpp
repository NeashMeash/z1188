#include "MyProxyModel.h"
#include "HomeUsersModel.h"

MyProxyModel::MyProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{

}

MyProxyModel::~MyProxyModel()
{

}

QVariant MyProxyModel::data(const QModelIndex &index, int role) const
{
    if ( role == Qt::DisplayRole && index.column() == 0 ) {
        return index.row()+1;
    }
    return QSortFilterProxyModel::data(index, role);
}

bool MyProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if ( left.column() == HomeUsersModel::kNameColumn ) {

    }
    QVariant rightData = sourceModel()->data(right);

    if ( rightData.isNull()  ) {
        return true;
    }

    QVariant leftData = sourceModel()->data(left);
    if ( leftData.isNull() ) {
        return false;
    }
    return QSortFilterProxyModel::lessThan(left, right);
}


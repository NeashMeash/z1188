#include "BlankItemSqlQueryModel.h"
#include <QColor>
#include <QFont>
#include <QDebug>

QBlankItemSqlQueryModel::QBlankItemSqlQueryModel(QObject *parent): QSqlQueryModel(parent)
{

}

QBlankItemSqlQueryModel::~QBlankItemSqlQueryModel()
{

}

QVariant QBlankItemSqlQueryModel::data(const QModelIndex& item, int role) const
{
    if(item.row() == 0)
        return QVariant();

    QModelIndex fakeItem = this->QSqlQueryModel::index(item.row()-1, item.column());

    if ( item.row() == 1 && role == Qt::FontRole ) {
        QVariant v = QSqlQueryModel::data(fakeItem, role);
        QFont f = qvariant_cast<QFont>(v);
        f.setBold(true);
        return f;
        //return QColor(Qt::red);
    }

    if ( role == Qt::DisplayRole && item.column() == 1 ) {
        QString s = QSqlQueryModel::data(fakeItem, role).toString();
        return " " + s;
        //return QVariant(QString(""));
    }


    return this->QSqlQueryModel::data(fakeItem, role);
    //return this->QSqlQueryModel::data(item, role);
}

int QBlankItemSqlQueryModel::rowCount(const QModelIndex& parent) const
{
    return this->QSqlQueryModel::rowCount(parent) + 1;
}

#ifndef QBLANKITEMSQLQUERYMODEL_H
#define QBLANKITEMSQLQUERYMODEL_H

#include <QSqlQueryModel>

class QBlankItemSqlQueryModel  : public QSqlQueryModel
{
    Q_OBJECT

public:
    QBlankItemSqlQueryModel(QObject *parent = 0);
    ~QBlankItemSqlQueryModel();
    virtual QVariant data(const QModelIndex& item, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
};

#endif // QBLANKITEMSQLQUERYMODEL_H

#ifndef MYPROXYMODEL_H
#define MYPROXYMODEL_H

#include <QSortFilterProxyModel>
class MyProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit MyProxyModel(QObject *parent = 0);
    ~MyProxyModel();
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
     bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
signals:

public slots:
};

#endif // MYPROXYMODEL_H

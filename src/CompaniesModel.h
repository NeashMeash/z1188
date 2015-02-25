#ifndef COMPANIESMODEL_H
#define COMPANIESMODEL_H

#include <QObject>

#include <QSqlQueryModel>
#include <QSettings>
#include "HomeUsersModel.h"

class CompaniesModel : public  HomeUsersModel
{
    Q_OBJECT
public:
    explicit CompaniesModel(QObject *parent = 0);
    ~CompaniesModel();
    virtual QVariant data(const QModelIndex& item, int role = Qt::DisplayRole) const;

signals:

public slots:

protected:
    virtual void configureHeader();
};

#endif // COMPANIESMODEL_H

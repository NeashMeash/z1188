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
    /*enum {
        kNrColumn = 0,
        kPrefixColumn = 1,
        kPhoneColumn = 2,
        kNameColumn = 3,
        kStreetColumn = 4,
        kHouseColumn = 5,
        kApartmentColumn = 6,
        kSectorName = 7,
        kSubdivisionName = 8,
        kLocationColumn = 9,
        kIsCompanyColumn = 10,
        kLatColumn = 11,
        kLonColumn = 12,
        kStreetIdColumn = 13,
        kAddressUpdatedColumn = 14,
        kStreetLocationId = 15,
        kLocationId = 16,
        kUpdatedColumn = 17,
        kRegionName = 18,
        kExactBlock = 19,
        kLocationTypeColumn = 20,
         kStreetTypeColumn = 21,
        kLastNameColumn  =22,
        kFirstNameColumn = 23,
        kMiddleNameColumn = 24,
        kAcutesColumn = 25,

        SortingRole = 500
     };*/
    explicit CompaniesModel(QObject *parent = 0);
    ~CompaniesModel();

signals:

public slots:

protected:
    virtual void configureHeader();
};

#endif // COMPANIESMODEL_H

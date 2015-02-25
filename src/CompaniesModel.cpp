#include "CompaniesModel.h"
#include <QDebug>
#include <QSettings>
#include <QDate>
#include <QColor>
#include "Utils.h"

CompaniesModel::CompaniesModel(QObject *parent) : HomeUsersModel(parent)
{

}

CompaniesModel::~CompaniesModel()
{

}

QVariant CompaniesModel::data(const QModelIndex &item, int role) const
{
    int column = item.column();
    if (role == Qt::DisplayRole  ) {
         bool isCompany = QSqlQueryModel::data(index(item.row(), kIsCompanyColumn)).toBool();
         if (!isCompany ) {
            if ( column == kNameColumn) {

                QString companyName = QSqlQueryModel::data(index(item.row(), kCompanyName)).toString();
                if (!companyName.isEmpty()) {
                    return companyName;
                }
            } else if (column == kEmployee ) {
                QString fullName = QSqlQueryModel::data(index(item.row(), kNameColumn)).toString();
                QString other = index(item.row(), kOtherColumn).data().toString();
                QVariant acutes = index(item.row(), kAcutesColumn).data();
                if ( !acutes.isNull()) {
                    if ( nameCache[item.row()].isEmpty() ) {
                        QString name = index(item.row(), kLastNameColumn).data().toString();
                        QString firstName = index(item.row(), kFirstNameColumn).data().toString();
                        QString middleName = index(item.row(), kMiddleNameColumn).data().toString();
                        Utils::restoreNormalizedStr(name, firstName, middleName,acutes.toByteArray());
                        fullName = name + " " + firstName + " " + middleName;
                        const_cast<QVector<QString>&>(nameCache)[item.row()] = fullName;
                    } else {
                        fullName= nameCache[item.row()];
                    }
                }

                if ( !other.isEmpty() ) {
                    fullName += "(" + other + ")";
                }
                return fullName;
            }
         }
    }
    return HomeUsersModel::data(item, role);
}

void CompaniesModel::configureHeader()
{
    setHeaderData(kNrColumn, Qt::Horizontal, tr("Nr"));
    setHeaderData(kNrColumn, Qt::Horizontal, Qt::AlignCenter, Qt::TextAlignmentRole);
    setHeaderData(kPrefixColumn, Qt::Horizontal, tr("Сode"));
    setHeaderData(kPrefixColumn, Qt::Horizontal, Qt::AlignCenter, Qt::TextAlignmentRole);

    setHeaderData(kPhoneColumn, Qt::Horizontal, tr("Phone"));
    setHeaderData(kPhoneColumn, Qt::Horizontal, Qt::AlignCenter, Qt::TextAlignmentRole);

    setHeaderData(kNameColumn, Qt::Horizontal, tr("Name"));
    setHeaderData(kNameColumn, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);

    setHeaderData(kStreetColumn, Qt::Horizontal, tr("Street"));
    setHeaderData(kStreetColumn, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);

    setHeaderData(kHouseColumn, Qt::Horizontal, tr("House"));
    setHeaderData(kHouseColumn, Qt::Horizontal, Qt::AlignCenter, Qt::TextAlignmentRole);
    setHeaderData(kApartmentColumn, Qt::Horizontal, tr("Of."));
    setHeaderData(kApartmentColumn, Qt::Horizontal, Qt::AlignCenter, Qt::TextAlignmentRole);
    setHeaderData(kLocationColumn, Qt::Horizontal, tr("Location"));
    setHeaderData(kLocationColumn, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);

    setHeaderData(kSectorName, Qt::Horizontal, tr("Sector"));
    setHeaderData(kSectorName, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    setHeaderData(kSubdivisionName, Qt::Horizontal, tr("Subdivision"));
    setHeaderData(kSubdivisionName, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);

    setHeaderData(kEmployee, Qt::Horizontal, tr("Employee"));

}




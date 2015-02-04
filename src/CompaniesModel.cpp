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
    setHeaderData(kApartmentColumn, Qt::Horizontal, tr("Ap."));
    setHeaderData(kApartmentColumn, Qt::Horizontal, Qt::AlignCenter, Qt::TextAlignmentRole);
    setHeaderData(kLocationColumn, Qt::Horizontal, tr("Location"));
    setHeaderData(kLocationColumn, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);

    setHeaderData(kSectorName, Qt::Horizontal, tr("Sector"));
    setHeaderData(kSectorName, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    setHeaderData(kSubdivisionName, Qt::Horizontal, tr("Subdivision"));
    setHeaderData(kSubdivisionName, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);

}




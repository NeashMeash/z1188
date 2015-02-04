#include "HomeUsersModel.h"
#include "Utils.h"
#include <QDate>
#include <QColor>
#include <QDebug>
#include <QVector>
#include <QSize>
#include <QFont>
#include <QFontMetrics>
#include <QTreeView>
#include <QSysInfo>

HomeUsersModel::HomeUsersModel(QObject *parent) : QSqlQueryModel(parent)
{
    showOldStreetNames = false;
    isWindowsXP = false;
    nameCache.resize(500);
    rowsCount = -1;
    useSystemStyle = false;
#ifdef Q_OS_WIN32
    isWindowsXP = QSysInfo::windowsVersion() == QSysInfo::WV_XP;
#endif
}

QVariant HomeUsersModel::data(const QModelIndex &item, int role) const
{

     if (!item.isValid()) return QVariant() ;
    int column = item.column();
    if ( role == Qt::TextAlignmentRole && (  column == 0 ||
                                             column == kPrefixColumn || column == kPhoneColumn
                                             || column == kHouseColumn || column == kApartmentColumn

                                             ) ) {
        return Qt::AlignCenter;
    } else if ( role == Qt::DisplayRole && column == 0 ) {
        return item.row()+1;
    }else if (role == Qt::DisplayRole && column == kStreetColumn && showOldStreetNames ) {
        if ( language != "ru" ) {
            //qDebug() << "language"<<language;
            QString street = QSqlQueryModel::data(item, role).toString();
            if ( language == "ro") {
                 street = Utils::transliterateRuToRo(street);
            } else  {
                street = Utils::transliterateRuToEn(street);
            }
            return street;
        }
    }  else if (role == Qt::DisplayRole && column == kNameColumn ) {
        //QDateTime m = index(item.row(), kUpdatedColumn).data();
        QVariant acutes = index(item.row(), kAcutesColumn).data();
        if ( !acutes.isNull()) {
            if ( nameCache[item.row()].isEmpty() ) {
                QString name = index(item.row(), kLastNameColumn).data().toString();
                QString firstName = index(item.row(), kFirstNameColumn).data().toString();
                QString middleName = index(item.row(), kMiddleNameColumn).data().toString();
                Utils::restoreNormalizedStr(name, firstName, middleName,acutes.toByteArray());
                QString res = name + " " + firstName + " " + middleName;
                const_cast<QVector<QString>&>(nameCache)[item.row()] = res;
                 return res;
            } else {
                return nameCache[item.row()];
            }
        }

    }

    else if ( role == SortingRole ) {
        if ( column == kHouseColumn ) {
            QString house = data(index(item.row(), item.column())).toString();
            QStringList tokens = house.split("/");
            int houseNumber = tokens[0].toInt();
            QString houseBlock;
            if ( tokens.size() > 1 ) {
                houseBlock = tokens[1];
            }
            QString res;
            res.sprintf("%03d/%s",houseNumber,houseBlock.toUtf8().data());

            return res;
        }
         return QSqlQueryModel::data(item, Qt::DisplayRole);
    } else if ( role == Qt::TextColorRole ) {
        int  updated = data(index(item.row(), kUpdatedColumn)).toInt();
        //qDebug() << "Updated: "<<updated;
        if ( updated &&  updated < 2005 ) {
             return QColor(Qt::gray);
        }
         //return QColor(Qt::gray);
        if ( column == kStreetColumn || column == kHouseColumn || column == kApartmentColumn ) {
            int streetLocationId = data(index(item.row(),kStreetLocationId)).toInt();
            int locationId = data(index(item.row(),kLocationId)).toInt();
            if ( streetLocationId && streetLocationId!=locationId  ) {
                  return QColor(Qt::gray);
            }
            QString street = data(index(item.row(), kStreetColumn)).toString();
            if ( ! street.isEmpty() ) {
                int date = data(index(item.row(), kAddressUpdatedColumn)).toInt();
                if ( date && date < 2008 ) {
                    return QColor(Qt::gray);
                }
            }
            if ( column == kHouseColumn ) {
                bool exactBlock = data(index(item.row(), kExactBlock)).toBool();
                if (!exactBlock && data(index(item.row(),kHouseColumn)).toString().contains("/")) {
                    return QColor(Qt::gray);
                }
            }

        }
        return useSystemStyle ? QSqlQueryModel::data(item, role) : QColor(Qt::black);
    }else if ( role == Qt::ToolTipRole ) {
        int updated = data(index(item.row(), kUpdatedColumn)).toInt();
        if ( updated &&  updated < 2005 ) {
            return tr("The line is grayed out because phone number probably has been removed (information corresponds to the year of %1)").arg(updated);

        }
        if ( column == kStreetColumn || column == kHouseColumn || column == kApartmentColumn ) {
            int streetLocationId = data(index(item.row(),kStreetLocationId)).toInt();
            int locationId = data(index(item.row(),kLocationId)).toInt();
            if ( streetLocationId && streetLocationId!=locationId  ) {
                return tr("The address is grayed out because the address does not match the phone number's city (the side effect of database merging)");

            } else {
                QString street = data(index(item.row(), kStreetColumn)).toString();
                if ( ! street.isEmpty() ) {
                    int date = data(index(item.row(), kAddressUpdatedColumn)).toInt();
                    if ( date && date < 2009) {
                        return tr("The address is grayed out because the data is obsolete (%1)").arg(date);
                    }
                }
            }
            if ( column == kHouseColumn ) {
                 bool exactBlock = data(index(item.row(), kExactBlock)).toBool();
                 if (!exactBlock  && data(index(item.row(),kHouseColumn)).toString().contains("/")) {
                     return tr("The house number is grayed out because the block number might not be accurate");

                 }
            }
        } else if ( column == kLocationColumn ) {
            QString regionName = data(index(item.row(),kRegionName)).toString();
            return tr("Region: %1").arg(regionName);
        }
    } else if ( isWindowsXP &&  role == Qt::SizeHintRole && column != kLocationColumn ) {
        QTreeView * treeview = qobject_cast<QTreeView*>(parent());
        QFontMetrics fm = treeview->viewport()->fontMetrics();
        QString text = data(item, Qt::DisplayRole).toString();
        return   QSize(fm.width(text)+3, fm.height()+6);
    }
    return QSqlQueryModel::data(item, role);
}

void HomeUsersModel::setQuery(const QSqlQuery &query)
{
    rowsCount = -1;
    QSqlQueryModel::setQuery(query);
    configureHeader();
}

void HomeUsersModel::setQuery(const QString &query, const QSqlDatabase &db)
{
    rowsCount = -1;
    QSqlQueryModel::setQuery(query,db);
    configureHeader();
}

HomeUsersModel::~HomeUsersModel()
{

}

void HomeUsersModel::setAppLanguage(const QString &language)
{
    this->language = language;
}

void HomeUsersModel::setShowOldStreetNames(bool show)
{
    showOldStreetNames = show;
}

int HomeUsersModel::rowCount(const QModelIndex &parent) const
{
    return (rowsCount == -1) ? QSqlQueryModel::rowCount(parent) : 0;
}

void HomeUsersModel::clear()
{
    // QSqlQueryModel::clear();
    //nameCache.clear();
    removeRows(0,rowCount());
    nameCache.resize(500);
   // emit rowsRemoved(index(0,0),0,QSqlQueryModel::rowCount());
//    emit

}

bool HomeUsersModel::removeRows(int row, int count, const QModelIndex & parent) {
    beginRemoveRows(parent,row, row+count);
     rowsCount = 0;
    endRemoveRows();
    return true;
}

void HomeUsersModel::setUseSystemStyle(bool use)
{
    bool useSystemStyle = use;
}

void HomeUsersModel::configureHeader()
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



    //setHeaderData(kLocationColumn, Qt::Horizontal, Qt::AlignCenter, Qt::TextAlignmentRole);
}


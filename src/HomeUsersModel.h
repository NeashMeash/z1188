#ifndef HOMEUSERSMODEL_H
#define HOMEUSERSMODEL_H

#include <QObject>
#include <QSqlQueryModel>
#include <QVector>
#include <QPixmap>

class HomeUsersModel : public  QSqlQueryModel
{
    Q_OBJECT
public:
    enum {
        kNrColumn = 0,
        kPrefixColumn,
        kPhoneColumn,
        kNameColumn,
        kStreetColumn,
        kHouseColumn,
        kApartmentColumn,
        kSectorName,
        kSubdivisionName,
        kEmployee,
        kCompanyName,
        kCompanyId,
        kLocationColumn,
        kIsCompanyColumn,
        kLatColumn,
        kLonColumn,
        kStreetIdColumn,
        kAddressUpdatedColumn,
        kStreetLocationId,
        kLocationId,
        kUpdatedColumn,
        kRegionName,
        kExactBlock,
        kLocationTypeColumn,
        kStreetTypeColumn,
        kLastNameColumn,
        kFirstNameColumn,
        kMiddleNameColumn,
        kAcutesColumn,
        kOperatorColumn,
        kOtherColumn,
        kApSuffix,
        SortingRole = 500
     };
    explicit HomeUsersModel(QObject *parent = 0);
    virtual QVariant data(const QModelIndex& item, int role = Qt::DisplayRole) const;
    void setQuery(const QSqlQuery &query);
    void setQuery(const QString &query, const QSqlDatabase &db = QSqlDatabase());
    ~HomeUsersModel();
    void setAppLanguage(const QString& language);
    void setShowOldStreetNames(bool show);
    virtual int	rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual void clear();
    bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
    void setUseSystemStyle(bool use);
signals:

public slots:
protected:
    virtual void configureHeader();

protected:

    QString language;
    QPixmap mobileIcon;
    bool showOldStreetNames;
    QVector<QString> nameCache;
    int rowsCount;
    bool useSystemStyle;
    bool isWindowsXP;
};

#endif // HOMEUSERSMODEL_H

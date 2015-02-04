#ifndef HOMEUSERSMODEL_H
#define HOMEUSERSMODEL_H

#include <QObject>
#include <QSqlQueryModel>
#include <QVector>

class HomeUsersModel : public  QSqlQueryModel
{
    Q_OBJECT
public:
    enum {
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

private:

    QString language;
    bool showOldStreetNames;
    QVector<QString> nameCache;
    int rowsCount;
    bool useSystemStyle;
    bool isWindowsXP;
};

#endif // HOMEUSERSMODEL_H

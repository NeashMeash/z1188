#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QSqlQuery>

#include <QSettings>

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker(QSettings * settings);
    ~Worker();
    void setDbLanguage(const QString& language);
    void setShowOldStreetNames(bool);
public slots:
    void doWork( int regionId, int locationId, QString phone, const QString& name,
                 const QString& street,  int houseNumber,    const QString& houseBlock,
                 int apartment, bool searchCompanies);

signals:
    void resultReady(const QSqlQuery &result, bool isCompany, const QString& errorString);
private:
    QString language;
    bool showOldStreetNames;
    QSettings * settings;
};

#endif // WORKER_H

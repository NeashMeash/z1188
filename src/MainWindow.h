#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QTreeView>
#include <QThread>
#include <QKeyEvent>
#include "HomeUsersModel.h"
#include "CompaniesModel.h"
#include "Worker.h"
#include <QSettings>
#include <QMovie>
#include "MyProxyModel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QSettings *settings, QWidget *parent = 0);
    ~MainWindow();

    bool hasFailed();
    enum MapType { GoogleMaps, PointMdMap, OpenStreetMap};

public slots:
    void handleResults(const QSqlQuery &result,bool isCompany,const QString& errorString);
private slots:
    void on_regionCombo_currentIndexChanged(int index);

    void on_homeUsersButton_clicked();

    void on_companiesButton_clicked();

    void on_searchButton_clicked();

    void on_settingsButton_clicked();
    void on_homeUsersTable_customContextMenuRequested(const QPoint &pos);

    void on_companiesTable_customContextMenuRequested(const QPoint &pos);

    void on_saveButton_clicked();

    void on_phoneEdit_textEdited(const QString &arg1);

    void on_locationCombo_currentIndexChanged(int index);

signals:
    void operate( int regionId, int locationId, QString phone, const QString& name,
                  const QString& street,  int houseNumber,    const QString& houseBlock,
                  int apartment,  bool searchCompanies);

protected:
    void writeSettings();
    void readSettings();
    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
private:
    Ui::MainWindow *ui;
    QSqlQueryModel* locationModel;
    QString dbLanguage;
    QThread workerThread;
    Worker * worker;
    HomeUsersModel * homeUsersModel;
    CompaniesModel * companiesModel;
    QSettings *settings;
    QMovie* loaderAnimation;
    MyProxyModel *sqlproxy;
    MyProxyModel *companiesSqlProxy;
    QSqlQueryModel *streetAutoCompleteModel;
    virtual void keyPressEvent(QKeyEvent *);
    void initComboBoxes();


    void fixLocationTreeView();
    void selectCompanies(bool select);
    void setupButtonsCursor();
    void showOnMap(bool companies, int row, MapType mapType);

    bool findHouseCoordinates(int streetId, QString house, double * lat, double *lon);
    bool failed;
    void showLoaderAnimation(bool show);
    QString getColumnValueForExport(QTreeView* treeview, int row, int column, bool forClipboard);
    void copyRowToClipboard(QTreeView* treeview, int row);
    void treeviewContextMenu(bool companies, const QPoint &pos);
    void updateStreetAutoComplete();

};

#endif // MAINWINDOW_H

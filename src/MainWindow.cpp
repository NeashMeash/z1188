#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QCompleter>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QDebug>
#include <QSqlQuery>
#include <QListView>
#include <QSizePolicy>
#include <QMessageBox>
#include "SettingsDialog.h"
#include "BlankItemSqlQueryModel.h"
#include "Worker.h"
#include <QSortFilterProxyModel>
#include "HomeUsersModel.h"
#include <QShortcut>
#include <QDesktopWidget>
#include <QMenu>
#include <QDesktopServices>
#include <QSqlRecord>
#include <QSqlField>
#include <QUrl>
#include <QMovie>
#include <QClipboard>
#include <QFileDialog>
#ifdef Z1188_CURSOR_HACK
    #include "MyHeaderView.h"
#endif
#include "MyProxyModel.h"
#include "Utils.h"

MainWindow::MainWindow(QSettings *settings, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    failed = false;
    locationModel = 0;
    ui->setupUi(this );
    ui->homeUsersTable->installEventFilter(this);
    ui->companiesTable->installEventFilter(this);
#ifdef Q_OS_WIN32
    ui->regionCombo->setMinimumHeight(22);
    ui->locationCombo->setMaximumHeight(22);
#endif

    loaderAnimation = new QMovie(":/icons/loader.mng",QByteArray(), this);
    foreach ( QObject* child, findChildren<QLineEdit *>() ) {
        child->installEventFilter(this);
    }
    foreach ( QObject* child, findChildren<QComboBox *>() ) {
        child->installEventFilter(this);
    }

    //loaderAnimation->show();
    this->settings = settings;
    readSettings();


    QString language =  QApplication::instance()->property("currentLanguage").toString();
    if ( language == "ru" ) {
        dbLanguage = "ru";
    } else {
        dbLanguage = "ro";
    }
    //ui->regionCombo->setModel();
    ui->companiesTable->hide();
    qRegisterMetaType<QSqlQuery>("QSqlQuery");
#ifdef Z1188_CURSOR_HACK
    ui->homeUsersTable->setHeader(new MyHeaderView(Qt::Horizontal,ui->homeUsersTable));
#endif
    ui->homeUsersTable->header()->setSectionsClickable(true);
    ui->homeUsersTable->setRootIsDecorated(false);


    streetAutoCompleteModel = new QSqlQueryModel();
   /* QSqlQuery q("SELECT DISTINCT COALESCE(name_"+dbLanguage+",name_ro) as name from streets order by name");
    q.exec();
    streetAutoCompleteModel->setQuery(q);*/
    QCompleter *completer = new QCompleter(this);
    completer->setModel(streetAutoCompleteModel);
    completer->setFilterMode(Qt::MatchStartsWith);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    // completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);

    initComboBoxes();
    setupButtonsCursor();





    ui->streetEdit->setCompleter(completer);

    homeUsersModel = new HomeUsersModel(ui->homeUsersTable);
    sqlproxy = new MyProxyModel(this);
    sqlproxy->setSourceModel(homeUsersModel);
    sqlproxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    sqlproxy->setSortRole(HomeUsersModel::SortingRole);
    ui->homeUsersTable->setModel(sqlproxy);
    ui->homeUsersTable->header()->setSortIndicator(HomeUsersModel::kNameColumn, Qt::AscendingOrder ) ;
    ui->homeUsersTable->setSortingEnabled(true);
    ui->homeUsersTable->setContextMenuPolicy(Qt::CustomContextMenu);
    //connect(ui->homeUsersTable, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

    companiesModel = new CompaniesModel(ui->companiesTable);
    companiesSqlProxy = new MyProxyModel(this);
    companiesSqlProxy->setSourceModel(companiesModel);
    companiesSqlProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    sqlproxy->setSortRole(CompaniesModel::SortingRole);
#ifdef Z1188_CURSOR_HACK
    ui->companiesTable->setHeader(new MyHeaderView(Qt::Horizontal,ui->companiesTable));
#endif
    ui->companiesTable->setModel(companiesSqlProxy);
    ui->companiesTable->header()->setSortIndicator(CompaniesModel::kNameColumn, Qt::AscendingOrder ) ;
    ui->companiesTable->setSortingEnabled(true);
    ui->companiesTable->setRootIsDecorated(false);
    ui->companiesTable->setContextMenuPolicy(Qt::CustomContextMenu);



    // ui->homeUsersTable->setC

    worker = new Worker(settings);
    worker->setDbLanguage(dbLanguage);
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &MainWindow::operate, worker, &Worker::doWork);
    //connect(&workerThread, &QThread::start,)
    //c/onnect(this, &Controller::operate, worker, &Worker::doWork);
    connect(worker, &Worker::resultReady, this, &MainWindow::handleResults);
    workerThread.start();

    ui->phoneEdit->setFocus();
    /*QShortcut *returnShortcut = new QShortcut(QKeySequence("Return"), ui->groupBox);
    QObject::connect(returnShortcut, SIGNAL(activated()), ui->pushButton_Calculate, SLOT(click()));*/
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleResults(const QSqlQuery &result,bool isCompany,const QString& errorString)
{
    QString language = QApplication::instance()->property("currentLanguage").toString();
    bool showOldStreetNames = settings->value("showOldStreetNames", false).toBool();
    bool useSystemStyle =  settings->value("useSystemDefaultTheme", false).toBool();
    if ( !isCompany ) {
        QHeaderView * header = ui->homeUsersTable->header();
        //header->show();
        homeUsersModel->setAppLanguage(language);
        homeUsersModel->setShowOldStreetNames(showOldStreetNames);
        homeUsersModel->setUseSystemStyle(useSystemStyle);
        homeUsersModel->setQuery(result);


        ui->homeUsersTable->setColumnHidden(HomeUsersModel::kSectorName, true);
        ui->homeUsersTable->setColumnHidden(HomeUsersModel::kSubdivisionName, true);
        ui->homeUsersTable->setColumnHidden(HomeUsersModel::kIsCompanyColumn, true);
        ui->homeUsersTable->setColumnHidden(HomeUsersModel::kLatColumn, true);
        ui->homeUsersTable->setColumnHidden(HomeUsersModel::kLonColumn, true);
        ui->homeUsersTable->setColumnHidden(HomeUsersModel::kStreetIdColumn, true);
        ui->homeUsersTable->setColumnHidden(HomeUsersModel::kLocationId, true);
        ui->homeUsersTable->setColumnHidden(HomeUsersModel::kUpdatedColumn, true);
        ui->homeUsersTable->setColumnHidden(HomeUsersModel::kAddressUpdatedColumn, true);
        ui->homeUsersTable->setColumnHidden(HomeUsersModel::kStreetLocationId, true);
        ui->homeUsersTable->setColumnHidden(HomeUsersModel::kRegionName, true);
        ui->homeUsersTable->setColumnHidden(HomeUsersModel::kExactBlock, true);
        ui->homeUsersTable->setColumnHidden(HomeUsersModel::kStreetTypeColumn, true);
        ui->homeUsersTable->setColumnHidden(HomeUsersModel::kLocationTypeColumn, true);
        ui->homeUsersTable->setColumnHidden(HomeUsersModel::kLastNameColumn, true);
        ui->homeUsersTable->setColumnHidden(HomeUsersModel::kFirstNameColumn, true);
        ui->homeUsersTable->setColumnHidden(HomeUsersModel::kMiddleNameColumn, true);
        ui->homeUsersTable->setColumnHidden(HomeUsersModel::kAcutesColumn, true);
        ui->homeUsersButton->setChecked(true);
        ui->companiesButton->setChecked(false);
         //ui->homeUsersTable->header()->setSectionResizeMode(HomeUsersModel::kNameColumn, QHeaderView::Stretch);
        if ( homeUsersModel->rowCount() ) {
            for ( int i =0 ; i<homeUsersModel->columnCount(); i++) {
                if ( i == CompaniesModel::kPhoneColumn ) {
                    ui->homeUsersTable->setColumnWidth(i,70);
                } else {
                    ui->homeUsersTable->resizeColumnToContents(i);
                }

            }
        }

        ui->homeUsersTable->header()->show();

        selectCompanies(false);
    } else {
        companiesModel->setAppLanguage(language);
        companiesModel->setShowOldStreetNames(showOldStreetNames);
        companiesModel->setQuery(result);
        companiesModel->setUseSystemStyle(useSystemStyle);
        ui->companiesTable->setColumnHidden(CompaniesModel::kIsCompanyColumn, true);
        ui->companiesTable->setColumnHidden(CompaniesModel::kLatColumn, true);
        ui->companiesTable->setColumnHidden(CompaniesModel::kLonColumn, true);
        ui->companiesTable->setColumnHidden(CompaniesModel::kStreetIdColumn, true);
        ui->companiesTable->setColumnHidden(CompaniesModel::kLocationId, true);
        ui->companiesTable->setColumnHidden(CompaniesModel::kUpdatedColumn, true);
        ui->companiesTable->setColumnHidden(CompaniesModel::kAddressUpdatedColumn, true);
        ui->companiesTable->setColumnHidden(CompaniesModel::kStreetLocationId, true);
        ui->companiesTable->setColumnHidden(CompaniesModel::kRegionName, true);
        ui->companiesTable->setColumnHidden(CompaniesModel::kExactBlock, true);
        ui->companiesTable->setColumnHidden(CompaniesModel::kStreetTypeColumn, true);
        ui->companiesTable->setColumnHidden(CompaniesModel::kLocationTypeColumn, true);
        ui->companiesTable->setColumnHidden(CompaniesModel::kLastNameColumn, true);
        ui->companiesTable->setColumnHidden(CompaniesModel::kFirstNameColumn, true);
        ui->companiesTable->setColumnHidden(CompaniesModel::kMiddleNameColumn, true);
        ui->companiesTable->setColumnHidden(CompaniesModel::kAcutesColumn, true);

        if ( companiesModel->rowCount() ) {
            for ( int i =0 ; i<companiesModel->columnCount(); i++) {
                if ( i == CompaniesModel::kNameColumn) {
                    ui->companiesTable->setColumnWidth(i,200);
                } else if ( i == CompaniesModel::kSectorName) {
                    ui->companiesTable->setColumnWidth(i,180);
                }else if  ( i == CompaniesModel::kSubdivisionName) {
                    ui->companiesTable->setColumnWidth(i,150);
                }
                else {
                    ui->companiesTable->resizeColumnToContents(i);
                }
            }
        }


        ui->companiesTable->header()->show();
        selectCompanies(true);
    }
    showLoaderAnimation(false);
    ui->searchButton->setEnabled(true);
    if ( !errorString.isEmpty()) {
        QMessageBox::warning(this,tr("Error"),errorString);
    }
}

void MainWindow::on_regionCombo_currentIndexChanged(int index)
{
    QModelIndex id = ui->regionCombo->model()->index(index, 3);
    int regionId = id.data().toInt();

    if ( regionId ) {
        QSqlQuery query;
        query.prepare("SELECT COALESCE(locations.name_"+dbLanguage+",locations.name_ro)||' '|| location_types.name_"+dbLanguage+",COALESCE(locations.name_"+dbLanguage+",locations.name_ro),location_types.name_"+dbLanguage +",locations.id FROM locations inner join location_types on locations.location_type=location_types.id WHERE region_id=:regionId and location_type!=3 order by locations.id !=3,location_type!=1,COALESCE(locations.name_"+dbLanguage+",locations.name_ro)");
        query.bindValue(":regionId", regionId);
        query.exec();

        locationModel->setQuery(query);
        fixLocationTreeView();
    } else {
        if ( locationModel ) {
            locationModel->clear();
        }
    }
    updateStreetAutoComplete();
}

void MainWindow::on_homeUsersButton_clicked()
{
    selectCompanies(false);
}

void MainWindow::on_companiesButton_clicked()
{
    selectCompanies(true);
}

void MainWindow::fixLocationTreeView()
{
    QTreeView * treeView = qobject_cast<QTreeView*>(ui->locationCombo->view());

    treeView->setRootIsDecorated(false);
    treeView->setColumnHidden(0, true);
    treeView->setColumnHidden(3, true);
    treeView->setColumnWidth(2,20);
    QSizePolicy sp = treeView->sizePolicy();
    sp.setVerticalPolicy(QSizePolicy::MinimumExpanding);
    treeView->setSizePolicy(sp);


}

void MainWindow::selectCompanies(bool select)
{
    if ( select ) {
        ui->homeUsersButton->setChecked(false);
        ui->companiesButton->setChecked(true);
        ui->homeUsersTable->hide();
        ui->companiesTable->show();
        ui->nameLabel->setText(tr("Name:","Company name"));
        ui->apartmentLabel->setText(tr("Of."));
    } else {
        ui->homeUsersButton->setChecked(true);
        ui->companiesButton->setChecked(false);
        ui->homeUsersTable->show();
        ui->companiesTable->hide();
        ui->nameLabel->setText(tr("Name:","Person name"));
        ui->apartmentLabel->setText(tr("Ap."));
    }
}

void MainWindow::on_searchButton_clicked()
{
    bool showOldStreetNames = settings->value("showOldStreetNames", true).toBool();
    worker->setShowOldStreetNames(showOldStreetNames);
    int index = ui->regionCombo->currentIndex();
    int regionId = ui->regionCombo->model()->index(index, 3).data().toInt();
    index = ui->locationCombo->currentIndex();
    int locationId =  ui->locationCombo->model()->index(index, 3).data().toInt();

    QString phone = ui->phoneEdit->text().trimmed();
    phone.replace("-","");
    phone.replace("(","");
    phone.replace(")","");
    phone.replace(" ","");
    QString name = ui->nameEdit->text().trimmed();

    QString lastName;
    QString firstName;
    QString middleName;

    name.replace(QRegExp("\\s+")," ");
    name.replace("."," ");

    QString street = ui->streetEdit->text().trimmed();
    int houseNumber = ui->houseEdit->text().trimmed().toInt();
    QString houseBlock = ui->houseBlockEdit->text().trimmed();
    int apartment = ui->apartmentEdit->text().trimmed().toInt();
    ui->searchButton->setEnabled(false);
    bool searchCompanies = ui->companiesButton->isChecked();
    //ui->homeUsersTable-
    //ui->homeUsersTable->setModel(0);
    homeUsersModel->removeRows(0,homeUsersModel->rowCount());
    companiesModel->removeRows(0, companiesModel->rowCount());
    sqlproxy->removeRows(0, sqlproxy->rowCount());
    companiesSqlProxy->removeRows(0, companiesSqlProxy->rowCount());

    //sqlproxy->clear();
    //ui->homeUsersTable->setC
    //companiesModel->clear();
    //companiesSqlProxy->clear();
    // ui->homeUsersTable->header()->setModel(0);
    //  ui->companiesTable->header()->setModel(0);
    //ui->homeUsersTable->header()->hide();
    // ui->companiesTable->header()->hide();
    //ui->companiesTable->setModel(0);
    showLoaderAnimation(true);
    emit operate(regionId, locationId, phone, name, street,houseNumber, houseBlock, apartment, searchCompanies);
}

void MainWindow::on_settingsButton_clicked()
{
    SettingsDialog *dlg = new SettingsDialog(settings, this);
    if ( dlg->exec() == QDialog::Accepted ) {
        setupButtonsCursor();

        QString language =  QApplication::instance()->property("currentLanguage").toString();

        worker->setDbLanguage(language == "ru" ?language:"ro");
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if ( event->key() == Qt::Key_Return && event->modifiers() == Qt::NoModifier ) {
        ui->searchButton->click();
        event->accept();
    }
}

void MainWindow::initComboBoxes()
{
    QSqlQueryModel *regionsModel = new QBlankItemSqlQueryModel;
    QString sql = "SELECT case when code then name_"+dbLanguage+
            "|| '\t('||code||')' else name_"+dbLanguage +" end, name_"+dbLanguage+", case when code = 0 then null else ('('||code||')') end, id,code FROM regions order by id !=2, name_"+dbLanguage;
    regionsModel->setQuery(sql);

    ui->regionCombo->setModel(regionsModel);

    QTreeView *regionComboView =new QTreeView(this);
    ui->regionCombo->setView(regionComboView);

    regionComboView->setRootIsDecorated(false);
    regionComboView->setColumnHidden(0,true);
    regionComboView->setColumnHidden(3,true);
    regionComboView->setColumnHidden(4,true);
    regionComboView->setColumnWidth(2,30);


    QSizePolicy sp = regionComboView->sizePolicy();
    sp.setVerticalPolicy(QSizePolicy::MinimumExpanding);
    regionComboView->setMinimumHeight(300);
    regionComboView->setSizePolicy(sp);
    regionComboView->header()->hide();


    locationModel = new QBlankItemSqlQueryModel;
    ui->locationCombo->setModel(locationModel);
    QTreeView *treeView = new QTreeView(this);
    ui->locationCombo->setView(treeView);
    treeView->setMinimumHeight(300);
    treeView->header()->hide();
    fixLocationTreeView();
    QModelIndexList index = regionsModel->match(regionsModel->index(0,3),Qt::DisplayRole,2);
    if ( !index.isEmpty() &&  index.first().isValid() ) {
        ui->regionCombo->setCurrentIndex( index.first().row());
    }
}

bool MainWindow::hasFailed() {
    return failed;
}

void MainWindow::writeSettings()
{
    //QSettings settings();

    settings->beginGroup("MainWindow");
    settings->setValue("size", size());
    settings->endGroup();
}

void MainWindow::readSettings()
{
    settings->beginGroup("MainWindow");
    resize(settings->value("size", QSize(750, 450)).toSize());
    settings->endGroup();
    QDesktopWidget *desktop = QApplication::desktop();

    int screenWidth = desktop->width();
    int screenHeight = desktop->height();
    int x = (screenWidth - width()) / 2;
    int y = (screenHeight - height()) / 2;
    move(x,y);

}

void MainWindow::closeEvent(QCloseEvent *event)
{

    workerThread.quit();
    workerThread.wait();
    writeSettings();
    event->accept();
}

void MainWindow::setupButtonsCursor() {
    bool useSystemDefaultTheme = settings->value("useSystemDefaultTheme", false).toBool();
    QCursor cursor = useSystemDefaultTheme ? Qt::ArrowCursor : Qt::PointingHandCursor;
    ui->searchButton->setCursor(cursor);
    ui->saveButton->setCursor(cursor);
    ui->closeButton->setCursor(cursor);
    /*ui->homeUsersButton->setCursor(cursor);
    ui->companiesButton->setCursor(cursor);*/
}

void MainWindow::showOnMap(bool companies, int row, MainWindow::MapType mapType)
{
    QTreeView * treeview = companies ? ui->companiesTable : ui->homeUsersTable;
    QAbstractItemModel * model = treeview->model();
    bool ok = true;
    QString lat1 = model->index(row,companies?CompaniesModel::kLatColumn: HomeUsersModel::kLatColumn).data().toString();
    double lat = model->index(row,companies?CompaniesModel::kLatColumn: HomeUsersModel::kLatColumn).data().toDouble(&ok);
    double lon = model->index(row,companies?CompaniesModel::kLonColumn: HomeUsersModel::kLonColumn).data().toDouble(&ok);
    if ( !ok ) {
        return;
    }
    QString house = treeview->model()->index(row,companies?CompaniesModel::kHouseColumn: HomeUsersModel::kHouseColumn).data().toString();
    int streetId = treeview->model()->index(row,companies?CompaniesModel::kStreetIdColumn: HomeUsersModel::kStreetIdColumn).data().toInt();
    QString streetName =  treeview->model()->index(row,companies?CompaniesModel::kStreetColumn: HomeUsersModel::kStreetColumn).data().toString();

    QString url;
    QString label = streetName;
    if ( findHouseCoordinates(streetId,house,&lat,&lon) ) {
        label+= " "+house;
    }


    switch(mapType) {
    case GoogleMaps:
        url = "https://maps.google.com/maps?z=12&q=loc:"+QString::number(lat,'g',15)+","+QString::number(lon,'g',15);
        break;
    case PointMdMap:
        url =  "http://point.md/ru/map/#x"
                +QUrl::toPercentEncoding("="+QString::number(lat,'g',15)+"&y="+QString::number(lon,'g',15)+"&z=18&sq=&m="+
                                         QUrl::toPercentEncoding("[["+QString::number(lat,'g',15)+","+QString::number(lon,'g',15)+",\""+label+"\"]]"));
        break;
    case OpenStreetMap:
        url = "http://www.openstreetmap.org/?mlat="+QString::number(lat,'g',15)+"&mlon="+ QString::number(lon,'g',15) +"#map=17/"+QString::number(lat,'g',15)+"/"+QString::number(lon,'g',15);

        break;
    }


    QDesktopServices::openUrl(url);
}


bool MainWindow::findHouseCoordinates(int streetId, QString house, double *lat, double *lon)
{
    QStringList tokens = house.split("/");
    int houseNumber = tokens[0].toInt();
    QString houseBlock;
    if ( tokens.size() > 1 ) {
        houseBlock = tokens[1];
    }
    houseBlock = houseBlock.toLower();
    QString initialRussianBlock;
    QRegularExpression russianRegexp(QString::fromUtf8("^[а-яА-Я]+$"));
    if ( houseBlock.contains(russianRegexp) ) {
        initialRussianBlock = houseBlock;
        houseBlock = Utils::transliterateRuToEn(houseBlock);
    }

    QSqlQuery query;
    QString sql = "SELECT lat,lon from houses where lat is not null and lon is not null and street_id=:streetId and house_number=:houseNumber";
    if ( !houseBlock.isEmpty()) {
        sql += " AND house_block=:houseBlock";
    } else {
        sql += " AND house_block is null";
    }
    query.prepare(sql);
    query.bindValue(":streetId", streetId);
    query.bindValue(":houseNumber", houseNumber);
    query.bindValue(":houseBlock", houseBlock);
    query.exec();

    if( query.size()<=0 && !initialRussianBlock.isEmpty()) {
        if ( QString::compare(initialRussianBlock,"в", Qt::CaseInsensitive) ) {
            houseBlock = "b";
            query.bindValue(":houseBlock", houseBlock);
            query.exec();
        }
    }
    if( query.size() ) {
        if ( query.first() ) {
            QSqlRecord rec = query.record();
            // int latCol = rec.indexOf("lat");
            // int lonCol = rec.indexOf("lon");
            *lat = rec.field("lat").value().toDouble();
            *lon = rec.field("lon").value().toDouble();
            return true;
        }
    }
    return false;

}

void MainWindow::showLoaderAnimation(bool show)
{
    if ( show ) {
        loaderAnimation->start();
        ui->animationLabel->setMovie(loaderAnimation);


    } else {
        ui->animationLabel->setMovie(0);
        ui->animationLabel->repaint();
    }
}

QString MainWindow::getColumnValueForExport(QTreeView *treeview, int row, int column,bool forClipboard)
{
    QString columnValue = treeview->model()->index(row,column).data().toString();
    if ( columnValue.isEmpty() || columnValue == "0"){
        return QString();
    }
    if ( column == HomeUsersModel::kPrefixColumn && forClipboard ) {
        return QString();
    }
    if ( column == HomeUsersModel::kPhoneColumn && forClipboard ) {
        QString code = treeview->model()->index(row,HomeUsersModel::kPrefixColumn).data().toString();
        if ( !code.isEmpty() ) {
            columnValue = "(0"+code+") "+columnValue;
        }
    }
    else if ( column == HomeUsersModel::kApartmentColumn ) {
        columnValue=tr("ap.")+" "+columnValue;
    } else if ( column == HomeUsersModel::kStreetColumn ) {
        QString streetType = treeview->model()->index(row,HomeUsersModel::kStreetTypeColumn).data().toString();
        columnValue.remove(streetType);
        columnValue = streetType + " " + columnValue.trimmed();
    } else if ( column == HomeUsersModel::kStreetColumn ) {

    } else if ( column == HomeUsersModel::kLocationColumn ) {
        QString locationType = treeview->model()->index(row,HomeUsersModel::kLocationTypeColumn).data().toString();
        return locationType + " " + columnValue;
    }
    return columnValue;
}

void MainWindow::copyRowToClipboard(QTreeView *treeview, int row)
{
    QString result;
    for ( int i =1; i < treeview->model()->columnCount(); i++) {
        if (!treeview->isColumnHidden(i) ) {
            QString columnValue = getColumnValueForExport(treeview,row,i, true);
            if ( i!=1 && !columnValue.isEmpty()) {
                result+=" ";
            }
            result+=columnValue;

        }
    }
    QApplication::clipboard()->setText(result.trimmed());
}

void MainWindow::treeviewContextMenu(bool companies, const QPoint &pos)
{

    QTreeView * treeview = companies ? ui->companiesTable : ui->homeUsersTable;
    QModelIndex index = treeview->indexAt(pos);

    if (index.isValid() /*&& index.row() % 2 == 0*/) {
        int row = index.row();
        bool ok;
        double lat = treeview->model()->index(row,companies?CompaniesModel::kLatColumn: HomeUsersModel::kLatColumn).data().toDouble();
        double lon = treeview->model()->index(row,companies?CompaniesModel::kLonColumn: HomeUsersModel::kLonColumn).data().toDouble(&ok);

        QMenu *contextMenu = new QMenu();
        //ui->homeUsersTable->setContextMenuPolicy(Qt::ActionsContextMenu);
        QAction * action;



        QString cellValue = treeview->model()->data(index).toString().trimmed();
        if (!cellValue.isEmpty() && cellValue != "0" ) {
            action = new QAction(tr("Copy \"%1\"").arg(cellValue),contextMenu);
            connect(action, &QAction::triggered, [cellValue]{
                QApplication::clipboard()->setText(cellValue);
            });
            contextMenu->addAction(action);
        }

        action = new QAction(tr("Copy selected row")+"\tCtrl+C",contextMenu);
        connect(action, &QAction::triggered, [this,row,treeview]{
            copyRowToClipboard( treeview,row);
        });
        contextMenu->addAction(action);

        if ( ok && lat && lon ) {
            contextMenu->addSeparator();
            action = new QAction(QIcon(":/icons/google_maps_icon.png"),tr("Show on Google Maps"),contextMenu);
            connect(action, &QAction::triggered, [this,companies,index]{ showOnMap(companies, index.row(),GoogleMaps); });
            contextMenu->addAction(action);

            action = new QAction(QIcon(":/icons/point_md_icon.ico"),tr("Show on Point.md map"),contextMenu);
            connect(action, &QAction::triggered, [this,companies,index]{ showOnMap(companies, index.row(),PointMdMap); });
            contextMenu->addAction(action);

            action = new QAction(QIcon(":/icons/openstreetmap_icon.ico"),tr("Show on OpenStreetMap"),contextMenu);
            connect(action, &QAction::triggered, [this,companies,index]{ showOnMap(companies, index.row(),OpenStreetMap); });
            contextMenu->addAction(action);
        }



        contextMenu->exec(treeview->viewport()->mapToGlobal(pos));
    }
}

void MainWindow::updateStreetAutoComplete()
{

    int index = ui->regionCombo->currentIndex();
    int regionId = ui->regionCombo->model()->index(index, 3).data().toInt();
    index = ui->locationCombo->currentIndex();
    int locationId =  ui->locationCombo->model()->index(index, 3).data().toInt();
    QString sql = "SELECT DISTINCT COALESCE(streets.name_"+dbLanguage+",streets.name_ro) as name from streets  "
                                                                      " left join locations on location_id=locations.id "
                                                                      "where 1=1 ";

    if ( regionId ) {
        sql += " and region_id = "+QString::number(regionId);
    }
    if ( locationId ) {
        sql += " and location_id = "+QString::number(locationId);
    }
    sql+= " order by  SUBSTR(name,1,1)='\"',name";
    QSqlQuery q(sql);
    q.exec();
    streetAutoCompleteModel->setQuery(q);
    //ui->streetEdit->completer()->setM
}

void MainWindow::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        QString language = QApplication::instance()->property("currentLanguage").toString();
        if ( language == "ru" ) {
            dbLanguage = "ru";
        } else {
            dbLanguage = "ro";
        }
        // retranslate designer form
        ui->retranslateUi(this);
        initComboBoxes();

        // retranslate other widgets which weren't added in designer
        //retranslate();
    }

    // remember to call base class implementation
    QWidget::changeEvent(event);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if ( qobject_cast<QTreeView*>(obj) && keyEvent->key() == Qt::Key_C && keyEvent->modifiers()==Qt::ControlModifier){
            QTreeView * treeview = qobject_cast<QTreeView*>(obj);
            if ( treeview ) {
                QModelIndex currentIndex = treeview->currentIndex();
                if ( currentIndex.isValid())
                    copyRowToClipboard(treeview,currentIndex.row());
            }
            return true;
        } else if ( (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) && keyEvent->modifiers() == Qt::NoModifier ) {
            ui->searchButton->click();
            return true;
        }

    }
    return QObject::eventFilter(obj, event);
}

void MainWindow::on_homeUsersTable_customContextMenuRequested(const QPoint &pos)
{
    treeviewContextMenu(false,pos);

}

void MainWindow::on_companiesTable_customContextMenuRequested(const QPoint &pos)
{
    treeviewContextMenu(true,pos);
}

void MainWindow::on_saveButton_clicked()
{
    QString selectedFilter;
    QString initialDir = settings->value("savingDir",QString()).toString();

 // #ifdef Q_OS_WIN32
    initialDir+= "result.html";
//#endif

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    /*"results.html"*/initialDir,
                                                    tr("HTML files")+" (*.html);;"+
                                                    tr("Text files")+" (*.txt);;"+
                                                    tr("CSV files")+" (*.csv)",&selectedFilter
                                                    );

    if ( fileName.isEmpty() ) {
        return ;
    }
    settings->setValue("savingDir",QFileInfo(fileName).absoluteDir().path()+"/");
    bool companies = ui->companiesButton->isChecked();
    QTreeView *treeview = companies? ui->companiesTable : ui->homeUsersTable;
    int columnCount = treeview->model()->columnCount();
    int rowCount = treeview->model()->rowCount();



    QString ext;
    QString fileExt = QFileInfo(fileName).suffix();
    if ( !selectedFilter.isEmpty() ) {
        int startPos = selectedFilter.lastIndexOf("(*.")+3;
        ext = selectedFilter.mid(startPos,selectedFilter.lastIndexOf(")")-startPos);
        if ( fileExt .isEmpty() && !ext.isEmpty() ) {
            fileName+= "." + ext;
        }
    }

    if ( ext.isEmpty() ) {
       ext = fileExt;
    }

    if ( ext.toLower() ==  "html" ) {
        QFile f (":/export_template.html");
        f.open(QIODevice::ReadOnly);
        QString htmlTemplate = f.readAll();
        htmlTemplate.replace("{$title}", tr("Search results – z1188  –  Phone Book of Moldova"));
        QString tableHeader;
        QString tableRows;

        for ( int i =0; i<columnCount; i++ ) {
            if ( treeview->isColumnHidden(i) ) {
                continue;
            }
            tableHeader += "<th>" + treeview->model()->headerData(i, Qt::Horizontal).toString() + "</th>";
        }
        for ( int i =0; i<rowCount; i++ ) {
            tableRows += "<tr>";
            for ( int j =0; j<columnCount; j++ ) {
                if ( treeview->isColumnHidden(j) ) {
                    continue;
                }
                QString cellValue = treeview->model()->index(i,j).data().toString();
                if ( j == 0 ) {
                    cellValue = QString::number(i+1);
                }
                tableRows += "<td>" +  (cellValue == "0" ? "":cellValue) + "</td>";
            }
            tableRows += "</tr>\r\n";
        }
        htmlTemplate.replace("{$tableHeader}", tableHeader);
        htmlTemplate.replace("{$tableRows}", tableRows);
        QFile outFile(fileName);
        if ( outFile.open(QIODevice::WriteOnly) ) {
            QTextStream stream( &outFile );
            stream.setGenerateByteOrderMark(true);
            stream.setCodec("UTF-8");
            stream << htmlTemplate;
            f.close();
        } else {
            QMessageBox::critical(this, tr("Error"),tr("Cannot create file \"%1\"").arg(fileName));
        }

    } else if ( ext.toLower() == "csv") {
        QFile outFile(fileName);
        if ( outFile.open(QIODevice::WriteOnly|QIODevice::Text) ) {
            QTextStream stream( &outFile );
            stream.setGenerateByteOrderMark(true);
            stream.setCodec("UTF-8");
            for ( int i =0; i<rowCount; i++ ) {

                for ( int j =0; j<columnCount; j++ ) {
                    if ( treeview->isColumnHidden(j) ) {
                        continue;
                    }
                    QString cellValue;
                    if ( j == 0 ) {
                        cellValue = QString::number(i+1);
                    } else {
                        cellValue = treeview->model()->index(i,j).data().toString();
                        if ( j == HomeUsersModel::kHouseColumn ) {
                            cellValue="=\""+cellValue+"\"";
                        }
                    }

                    stream << (cellValue == "0" ? "":cellValue) + ";";
                }
                stream << "\n";
            }


            outFile.close();
        } else {
            QMessageBox::critical(this, tr("Error"),tr("Cannot create file \"%1\"").arg(fileName));
        }
    } else {
        QFile outFile(fileName);
        if ( outFile.open(QIODevice::WriteOnly|QIODevice::Text) ) {
            QTextStream stream( &outFile );
            stream.setGenerateByteOrderMark(true);
            stream.setCodec("UTF-8");
            QVector<int> columnSizes;
            columnSizes.resize(columnCount);
            for ( int i =0; i<rowCount; i++ ) {

                for ( int j =0; j<columnCount; j++ ) {
                    if ( treeview->isColumnHidden(j) ) {
                        continue;
                    }
                    QString cellValue = getColumnValueForExport(treeview,i,j, true);
                    if ( cellValue.length() > columnSizes[j]) {
                        columnSizes[j] = cellValue.length();
                    }
                }

            }

            for ( int i =0; i<rowCount; i++ ) {

                for ( int j =0; j<columnCount; j++ ) {
                    if ( treeview->isColumnHidden(j) ) {
                        continue;
                    }
                    QString cellValue;
                    /*if ( j == 0 ) {
                        cellValue.sprintf("%03d",i+1);
                    } else*/ {

                        cellValue = getColumnValueForExport(treeview,i,j, true);
                        if ( j == 0 ) {
                            cellValue = cellValue.rightJustified(columnSizes[j],' ');
                        } else {
                            cellValue = cellValue.leftJustified(columnSizes[j],' ');
                        }
                    }
                    stream << (cellValue == "0" ? "":cellValue) + " ";
                }
                stream << "\n";
            }


            outFile.close();
        } else {
            QMessageBox::critical(this, tr("Error"),tr("Cannot create file \"%1\"").arg(fileName));
        }
    }



}

void MainWindow::on_phoneEdit_textEdited(const QString &str1)
{
    QString str = str1;
    str.replace("-","");
    str.replace("(","");
    str.replace(")","");
    str.replace(" ","");
    QString numberWithCode;
    //int number;
    int code = 0;

    if(str.length() ) {
        if(str.startsWith("0") ) {
            numberWithCode = str.mid(1);
        } else if ( str.startsWith("+373") ) {
            numberWithCode = str.mid(4);
        }
    }
    if ( numberWithCode.startsWith("22") ) {
        code = 22;
    } else if ( numberWithCode.length() >=3 ) {
        code = numberWithCode.left(3).toInt();

    }
    if ( code ) {
        QModelIndexList  list = ui->regionCombo->model()->match(ui->regionCombo->model()->index(0,4),Qt::DisplayRole, code);
        if (!list.isEmpty() ) {
            ui->regionCombo->setCurrentIndex(list.first().row());
        }
    }
}


void MainWindow::on_locationCombo_currentIndexChanged(int index)
{
    updateStreetAutoComplete();
}

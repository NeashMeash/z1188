/**
   (c) 2015   neash.meash@gmail.com
*/
#include "MainWindow.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QTranslator>
#include <QLibraryInfo>
#include <QByteArray>

#include "Utils.h"
#include <QSqlDriver>
#include <QDesktopServices>
#include <QStandardPaths>
#include <QDir>
#include "Settings.h"
#include <QSqlError>

#ifdef Q_OS_WIN32
#pragma comment(lib,"Shell32.lib")
#include "shlobj.h"
#endif
#include "AppProperties.h"
#include <iostream>

void normalizeDb() {
     QSqlDatabase db = QSqlDatabase::database();
    if ( !db.isOpen() ) {
        QMessageBox::critical(0,"z1188",QApplication::tr("Could not connect to database"));
        return;
    }
    QSqlQuery qq("PRAGMA synchronous = OFF;");
    QSqlQuery qqq("PRAGMA journal_mode = MEMORY;");

    QSqlQuery q("SELECT  id,name, first_name, middle_name from phones where acutes is null");

    int i =0;
    while (q.next() ) {

            QByteArray acutes;
            QString name = q.value("name").toString();
            Utils::normalizeStr(name,0,acutes);

            QString firstName = q.value("first_name").toString();
            Utils::normalizeStr(firstName,1,acutes);
            QString middleName = q.value("middle_name").toString();
            Utils:: normalizeStr(middleName,2,acutes);
            if ( acutes.size() ) {
                QString sql = "UPDATE phones "
                              " set  name=:name, "
                              " first_name=:firstName, "
                              " middle_name=:middleName,"
                              " acutes=:acutes "
                              " where id=:id";

                QSqlQuery q2;
                q2.prepare(sql);
                q2.bindValue(":name", name);
                q2.bindValue(":firstName",  firstName.isEmpty()?QVariant():firstName);
                q2.bindValue(":middleName", middleName.isEmpty()? QVariant() : middleName);
                q2.bindValue(":id", q.value("id").toInt());
                q2.bindValue(":acutes", acutes,QSql::In|QSql::Binary);
                if ( !q2.exec() ) {
                   std::cout<<"Last error "<< q2.lastError().databaseText().toLocal8Bit().data()<<" "<<q2.size();
                } else {
                    i++;
                }
            }



        }
    std::cout<<"Updated "<<i<<" records"<<std::endl;
}

int main(int argc, char *argv[])
{


#ifdef Q_OS_WIN32
    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(QFileInfo(argv[0]).dir().path());
    paths.append("imageformats");
    paths.append("platforms");
    paths.append("sqldrivers");
    QCoreApplication::setLibraryPaths(paths);
#endif

    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Neash-Meash");
    // QCoreApplication::setOrganizationDomain("mysoft.com");
    QCoreApplication::setApplicationName("z1188");
    Settings settings;
    bool useSystemStyle = settings.value(Settings::UseSystemDefaultTheme).toBool();

    if (!useSystemStyle) {
        QFile styleFile( ":/style.css" );
        styleFile.open( QFile::ReadOnly );

        a.setStyleSheet( styleFile.readAll() );
        styleFile.close();


    }

    QApplication::setEffectEnabled(Qt::UI_AnimateCombo, false);
    QApplication::setEffectEnabled(Qt::UI_AnimateTooltip, false);


    QString languagesPath = QApplication::applicationDirPath() + "/languages/";
#ifdef Q_OS_OSX
    QDir langDir = QDir(QApplication::applicationDirPath());
    langDir.cdUp();
    langDir.cd("Resources");
    langDir.cd("Languages");
    languagesPath = langDir.absolutePath();
 #endif
#ifndef Q_OS_WIN32
    if ( ! QDir(languagesPath).exists() ) {
        languagesPath = "/usr/share/z1188/languages/";
    }
#endif

    QTranslator qtTranslator;
    QString qtTransPath = languagesPath+"qt";

    QString language = settings.value(Settings::Language).toString();
    AppProperties* appProps = AppProperties::instance();
    appProps->setCurrentLanguage(language);
    appProps->setLanguagesPath(languagesPath);
    appProps->setQtLanguagesPath(qtTransPath);


    qtTranslator.load("qt_" +language ,qtTransPath);
    a.installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    appProps->setTranslator(&myappTranslator);
    appProps->setQtTranslator(&qtTranslator);

    myappTranslator.load("z1188_" + language, languagesPath);
    a.installTranslator(&myappTranslator);

    bool showOldStreetNames = settings.value(Settings::ShowOldStreetNames).toBool();
    settings.setValue(Settings::ShowOldStreetNames, showOldStreetNames);
    QString commonDataPath;
#ifdef Q_OS_WIN32
    wchar_t szPath[MAX_PATH];
    // Get path for each computer, non-user specific and non-roaming data.
    if ( SUCCEEDED( SHGetFolderPath( NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPath ) ) )
    {
        commonDataPath = QString::fromUtf16((const ushort*)szPath)+"\\Neash-Meash\\z1188\\";
    }

#else
    commonDataPath = "/usr/share/z1188/";
#endif

    QString dbFileName =  "/db.db";
    QString dbPath = QApplication::applicationDirPath() ;
#ifdef Q_OS_OSX
    QDir d = QDir(dbPath);
    d.cdUp();
    d.cd("Resources");
    dbPath = d.absolutePath();
 #endif
            dbPath += dbFileName ;
            bool doNormalize = false;
            bool fileFromArg = false;

    if ( argc > 1 ) {

        for ( int i=0; i < argc; i++ ) {
            if ( !strcmp(argv[i],"-f") && i < argc-1) {

                dbPath = QString::fromLocal8Bit(argv[++i]);
                fileFromArg = true;

            } else if ( !strcmp(argv[i],"-n")) {
                doNormalize = true;

            }
        }
    }

    if ( !QFile(dbPath).exists()) {
        dbPath = commonDataPath + dbFileName;
        if ( fileFromArg || !QFile(dbPath).exists()) {
            QMessageBox::critical(0,"z1188",QApplication::tr("Cannot find database file %1").arg("db.db"));
            return 1;
        }
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    // faster load
    if (!doNormalize) {
        db.setConnectOptions(QLatin1String("QSQLITE_OPEN_READONLY"));
    }
    db.setDatabaseName(dbPath);

    if ( doNormalize ) {
        normalizeDb();
        return 0;
    }

    MainWindow w(&settings);
    if ( w.hasFailed() ) {
        return 1;
    }
    w.show();

    return a.exec();
}

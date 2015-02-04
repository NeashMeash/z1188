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

#ifdef Q_OS_WIN32
#pragma comment(lib,"Shell32.lib")
#include "shlobj.h"
#endif


void normalizeDb() {
    QSqlQuery qq("PRAGMA synchronous = OFF;");
    qq.exec();
    QSqlQuery qqq("PRAGMA journal_mode = MEMORY;");
    qqq.exec();

    QSqlQuery q("SELECT  id,name, first_name, middle_name from phones where acutes is null");
    q.exec();
    if ( q.size() ) {
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

                qDebug() << name << firstName << middleName << acutes.length()<<q.value("id").toInt()<<"\r\n";

            }


        }
    }
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
    QSettings settings;
    bool useSystemStyle = settings.value("useSystemDefaultTheme", false).toBool();

    if (!useSystemStyle) {
        QFile styleFile( ":/style.css" );
        styleFile.open( QFile::ReadOnly );

        a.setStyleSheet( styleFile.readAll() );
        styleFile.close();

    }

    QApplication::setEffectEnabled(Qt::UI_AnimateCombo, false);
    QApplication::setEffectEnabled(Qt::UI_AnimateTooltip, false);


    QString languagesPath = QApplication::applicationDirPath() + "/languages/";
#ifndef Q_OS_WIN32
    if ( ! QDir(languagesPath).exists() ) {
        languagesPath = "/usr/share/z1188/languages/";
    }
#endif

    QTranslator qtTranslator;
    QString qtTransPath = languagesPath+"qt";
    QString defaultLocale = QLocale::system().name();
    defaultLocale.truncate(defaultLocale.lastIndexOf('_'));
    QString language = settings.value("language",defaultLocale).toString();
    a.setProperty("currentLanguage", language);
    a.setProperty("languagesPath", languagesPath);
    a.setProperty("qtLanguagesPath", qtTransPath);


    qtTranslator.load("qt_" +language ,qtTransPath);
    a.installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    a.setProperty("translator",qVariantFromValue((void*)&myappTranslator));
    a.setProperty("qtTranslator",qVariantFromValue((void*)&qtTranslator));

    myappTranslator.load("z1188_" + language, languagesPath);
    a.installTranslator(&myappTranslator);

    bool showOldStreetNames = settings.value("showOldStreetNames", /*language == "ru"*/false).toBool();
    settings.setValue("showOldStreetNames", showOldStreetNames);
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
    QString dbPath = QApplication::applicationDirPath()  + dbFileName ;
    if ( !QFile(dbPath).exists()) {
        dbPath = commonDataPath + dbFileName;
        if ( !QFile(dbPath).exists()) {
            QMessageBox::critical(0,"z1188",QApplication::tr("Cannot find database file %1").arg("db.db"));
            return 1;
        }
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(dbPath);
    bool ok = db.open();
    if (!ok ) {
        QMessageBox::critical(0,"z1188",QApplication::tr("Could not connect to database"));
        return 1;
    }
    //normalizeDb();

    MainWindow w(&settings);
    if ( w.hasFailed() ) {
        return 1;
    }
    w.show();

    return a.exec();
}

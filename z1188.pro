#-------------------------------------------------
#
# Project created by QtCreator 2015-01-30T14:32:24
#
#-------------------------------------------------

QT       += core gui sql concurrent widgets

win32 {
QT       += core-private
QMAKE_CXXFLAGS += /Ox /Ob2 /Oi /Oy
}


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = z1188
TEMPLATE = app
win32{
RC_FILE = src/z1188.rc
}
CONFIG += c++11
unix{

}
arch = $$QMAKE_TARGET.arch
linux-g++:arch = $$QMAKE_HOST.arch
linux-g++-32:arch = x86
linux-g++-64:arch = x86_64
contains(arch, i686) {
arch=i386
}

contains(arch, x86_64) {
arch=amd64
}

win32:OUTDIR = build/windows
unix:OUTDIR = build/linux/$$arch
macx:OUTDIR = build/mac

debug:DESTDIR = $$OUTDIR/debug/executable
debug:OBJECTS_DIR = $$OUTDIR/debug/temp
debug:MOC_DIR = $$OUTDIR/debug/temp
debug:RCC_DIR = $$OUTDIR/debug/temp
debug:UI_DIR = $$OUTDIR/debug/temp

release:DESTDIR = $$OUTDIR/release/executable
release:OBJECTS_DIR = $$OUTDIR/release/temp
release:MOC_DIR = $$OUTDIR/release/temp
release:RCC_DIR = $$OUTDIR/release/temp
release:UI_DIR = $$OUTDIR/release/temp


SOURCES += src/main.cpp\
    src/SettingsDialog.cpp \
    src/Worker.cpp \
    src/HomeUsersModel.cpp \
    src/BlankItemSqlQueryModel.cpp \
    src/MainWindow.cpp \
    src/CompaniesModel.cpp \
    src/Utils.cpp \
    src/MyProxyModel.cpp \
    src/Settings.cpp \
    src/AppProperties.cpp \
    src/MyTreeView.cpp


HEADERS  += \
    src/SettingsDialog.h \
    src/Worker.h \
    src/HomeUsersModel.h \
    src/BlankItemSqlQueryModel.h \
    src/MainWindow.h \
    src/CompaniesModel.h \
    src/Utils.h \
    src/MyProxyModel.h \
    src/Settings.h \
    src/AppProperties.h \
    src/MyTreeView.h

FORMS    += src/mainwindow.ui \
    src/settingsdialog.ui

RESOURCES += \
    src/resources.qrc

TRANSLATIONS = languages/z1188_ru.ts languages/z1188_ro.ts

win32 {
    QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01
    QMAKE_LFLAGS_CONSOLE = /SUBSYSTEM:CONSOLE,5.01

    DEFINES += _ATL_XP_TARGETING
    QMAKE_CFLAGS += /D _USING_V110_SDK71_
    QMAKE_CXXFLAGS += /D _USING_V110_SDK71_
    LIBS *= -L"%ProgramFiles(x86)%/Microsoft SDKs/Windows/7.1A/Lib"
    INCLUDEPATH += "%ProgramFiles(x86)%/Microsoft SDKs/Windows/7.1A/Include"
    INCLUDEPATH += "c:/Qt/Qt5.4.0/5.4/msvc2013/src/qt-everywhere-opensource-src-5.4.0/qtbase/Include/QtWidgets/5.4.0/QtWidgets/"
}

win32{
 DEFINES += Z1188_CURSOR_HACK
 SOURCES += src/MyHeaderView.cpp
 HEADERS  +=   src/MyHeaderView.h
}

ICON = src/icons/z1188.icns

macx{
languages.files =  $$files(languages/*.qm)
languages.path = Contents/Resources/languages
languages_qt.files =  $$files(languages/qt/*.qm)
languages_qt.path = Contents/Resources/languages/qt

APP_DB_FILES.files = database/db.db
APP_DB_FILES.path = Contents/Resources
QMAKE_BUNDLE_DATA += APP_DB_FILES languages languages_qt
QMAKE_TARGET_BUNDLE_PREFIX = "neash-meash"
}

INCLUDEPATH += src/





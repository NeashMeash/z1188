#include "AppProperties.h"
#include <QMetaEnum>
#include <QCoreApplication>


AppProperties::AppProperties()
{
    translator_ = nullptr;
    qtTranslator_ = nullptr;
    totalPhoneCount_ = 0;
    atexit(&cleanUp);
}

void AppProperties::cleanUp()
{
    delete self;
    self = nullptr;
}


QString AppProperties::currentLanguage() const
{
    return currentLanguage_;
}

void AppProperties::setCurrentLanguage(const QString &language)
{
    currentLanguage_ = language;
}

QString AppProperties::languagesPath()  const
{
    return languagesPath_;
}

void AppProperties::setLanguagesPath(const QString& path)
{
    languagesPath_ = path;
}

QString AppProperties::qtLanguagesPath() const
{
    return qtLanguagesPath_;
}

void AppProperties::setQtLanguagesPath(const QString& path)
{
    qtLanguagesPath_ = path;
}

QTranslator* AppProperties::translator() const
{
    return translator_;
}

void AppProperties::setTranslator(QTranslator* translator)
{
    translator_ = translator;
}

QTranslator* AppProperties::qtTranslator() const
{
    return qtTranslator_;
}
void AppProperties::setQtTranslator(QTranslator* translator)
{
    qtTranslator_ = translator;
}

int AppProperties::totalPhoneCount() const
{
    return totalPhoneCount_;
}

void AppProperties::setTotalPhoneCount(int count)
{
    totalPhoneCount_ = count;
}

AppProperties* AppProperties::self = 0;

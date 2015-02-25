#include "Settings.h"
#include <QMap>
#include <QMetaEnum>
#include <QLocale>

Settings::Settings()
{
    QString defaultLocale = QLocale::system().name();
    defaultLocale.truncate(defaultLocale.lastIndexOf('_'));
    int index = staticMetaObject.indexOfEnumerator("SettingsEnum");
    metaEnum = staticMetaObject.enumerator(index);
}

QVariant Settings::defaultSettingValue(Settings::SettingsEnum key) const
{
    QMap<SettingsEnum, QVariant> defaultSettings ({
        {ShowOldStreetNames, false},
        {UseSystemDefaultTheme, false},
        {ShowStreetNamesInRomanian, false},
        {Language, defaultLocale}
    });
    return defaultSettings[key];
}

void Settings::setValue(const Settings::SettingsEnum& key, const QVariant &value)
{
    setValue(enumToStr(key),value);
}

QVariant Settings::value(const Settings::SettingsEnum& key) const
{
    return value(enumToStr(key), defaultSettingValue(key));
}

QString Settings::enumToStr(SettingsEnum key) const
{
    QString name = metaEnum.valueToKey(key);
    name[0] = name[0].toLower();
    return name;
}

void Settings::setValue(const QString &key, const QVariant &value){
    QSettings::setValue(key,value);
}

QVariant Settings::value(const QString &key, const QVariant &defaultValue) const {
    return QSettings::value(key,defaultValue);
}

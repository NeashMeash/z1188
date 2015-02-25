#ifndef DEFAULTSETTINGS_H
#define DEFAULTSETTINGS_H

#include <QSettings>
#include <QMetaEnum>
class Settings: public QSettings
{
    Q_OBJECT
    Q_ENUMS(SettingsEnum)

public:
    explicit Settings();
    enum SettingsEnum {
        ShowOldStreetNames,
        UseSystemDefaultTheme,
        ShowStreetNamesInRomanian,
        Language,
        SavingDir
    };

    QVariant defaultSettingValue(SettingsEnum key) const;

    void setValue(const SettingsEnum& key,const QVariant &value);
    QVariant value(const SettingsEnum& key) const;
    QString enumToStr(SettingsEnum key) const;
    void setValue(const QString &key, const QVariant &value);
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
protected:
    QString defaultLocale;
    QMetaEnum metaEnum;

};

#endif // DEFAULTSETTINGS_H

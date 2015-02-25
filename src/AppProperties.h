#ifndef APPPROPERTIES_H
#define APPPROPERTIES_H

#include <QString>

class QTranslator;

class AppProperties
{
public:
    static inline AppProperties *instance() { if(!self) { self = new AppProperties();} return self; }

    QString currentLanguage() const;
    void setCurrentLanguage(const QString &language);
    QString languagesPath() const;
    void setLanguagesPath(const QString& path);
    QString qtLanguagesPath() const;
    void setQtLanguagesPath(const QString& path);
    QTranslator* translator() const;
    void setTranslator(QTranslator* translator);
    QTranslator* qtTranslator() const;
    void setQtTranslator(QTranslator* translator);
    int totalPhoneCount() const;
    void setTotalPhoneCount(int count);
private:
    AppProperties();
    Q_DISABLE_COPY(AppProperties)
    QString currentLanguage_;
    QString languagesPath_;
    QString qtLanguagesPath_;
    QTranslator* translator_;
    QTranslator* qtTranslator_;
    int totalPhoneCount_;
    static AppProperties* self;
    static void cleanUp();
};




#endif // APPPROPERTIES_H

#ifndef UTILS_H
#define UTILS_H

#include <QString>

class Utils
{
public:
    Utils();
    ~Utils();
    static QString transliterateRuToEn(const QString& name);
    static QString transliterateRuToRo(const QString &name);
    static QString capitalize(const QString &name);
    static QString& convertRoToEn( QString &res);
    static void normalizeStr(QString &str, uchar stringNumber,QByteArray& acutes);
    static void restoreNormalizedStr(QString &name, QString &firstName, QString & middleName, const QByteArray&);
};

#endif // UTILS_H

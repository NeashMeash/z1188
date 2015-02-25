#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QMetaObject>

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
    template <typename Func>
    inline static void runLater(Func func) {
        QTimer *t = new QTimer();
        t->moveToThread(qApp->thread());
        t->setSingleShot(true);
        QObject::connect(t, &QTimer::timeout, [=]() {
            func();
            t->deleteLater();
        });
        QMetaObject::invokeMethod(t, "start", Qt::QueuedConnection, Q_ARG(int, 0));
    }
};

#endif // UTILS_H

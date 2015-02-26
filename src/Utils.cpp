#include "Utils.h"

#include <QMap>
#include <QByteArray>
#include <QDebug>
#include <QWidgetList>
#include <QApplication>
#include <QWidget>

Utils::Utils()
{

}

Utils::~Utils()
{

}

QString Utils::transliterateRuToEn(const QString &name) {
    QString res;
    static QMap<QString, QString> toTranslit;
    toTranslit["а"] = "a";
    toTranslit["б"] = "b";
    toTranslit["в"] = "v";
    toTranslit["г"] = "g";
    toTranslit["д"] = "d";
    toTranslit["е"] = "e";
    toTranslit["ё"] = "jo";
    toTranslit["ж"] = "zh";
    toTranslit["з"] = "z";
    toTranslit["и"] = "i";
    toTranslit["й"] = "j";
    toTranslit["к"] = "k";
    toTranslit["л"] = "l";
    toTranslit["м"] = "m";
    toTranslit["н"] = "n";
    toTranslit["о"] = "o";
    toTranslit["п"] = "p";
    toTranslit["р"] = "r";
    toTranslit["с"] = "s";
    toTranslit["т"] = "t";
    toTranslit["у"] = "u";
    toTranslit["ф"] = "f";
    toTranslit["х"] = "h";
    toTranslit["ц"] = "c";
    toTranslit["ч"] = "ch";
    toTranslit["ш"] = "sh";
    toTranslit["щ"] = "w";
    toTranslit["ъ"] = "";
    toTranslit["ы"] = "y";
    toTranslit["ь"] = "'";
    toTranslit["э"] = "e";
    toTranslit["ю"] = "ju";
    toTranslit["я"] = "ja";
    toTranslit["є"] = "je";
    toTranslit["і"] = "i";
    toTranslit["ї"] = "i";

    for (int i = 0; i < name.length(); ++i) {
        const QString ch = name.at(i);
        const QString lowerCh = ch.toLower();
        if (toTranslit.contains(lowerCh)) {
            QString tmp = ch == lowerCh ? toTranslit[lowerCh] : capitalize(toTranslit[lowerCh]);
            res += tmp;
        } else
            res += ch;
    }

    return res;
}

static QList<QPair<QString, QString>> toTranslit;

void initToTranslit() {
    toTranslit << QPair<QString,QString>("джа","gea");
    toTranslit << QPair<QString,QString>("джи" , "gi");
    toTranslit << QPair<QString,QString>("дже" , "ge");
    toTranslit << QPair<QString,QString>("чо" , "cio");
    toTranslit << QPair<QString,QString>("че" , "ce");
    toTranslit << QPair<QString,QString>("чи" , "ci");
    toTranslit << QPair<QString,QString>("ча" , "cea");
    toTranslit << QPair<QString,QString>("жи" , "gi");
    toTranslit << QPair<QString,QString>("ге" , "ghe");
    toTranslit << QPair<QString,QString>("ги" , "ghi");
    toTranslit << QPair<QString,QString>("ке" , "che");
    toTranslit << QPair<QString,QString>("ки" , "chi");
    toTranslit << QPair<QString,QString>("кс" , "x");
    toTranslit << QPair<QString,QString>("ья" , "ia");
    toTranslit << QPair<QString,QString>("аэ" , "aе");
    toTranslit << QPair<QString,QString>("ия" , "ia");
    toTranslit << QPair<QString,QString>("э" , "a");
    toTranslit << QPair<QString,QString>("ш" , "s");
    toTranslit << QPair<QString,QString>("ь" , "i");
    toTranslit << QPair<QString,QString>("ы" , "i");
    toTranslit << QPair<QString,QString>("й" , "i");
    toTranslit << QPair<QString,QString>("я" , "ea");
    toTranslit << QPair<QString,QString>("ц" , "t");
    toTranslit << QPair<QString,QString>("ж" , "j");
    toTranslit << QPair<QString,QString>("г", "g");
    toTranslit << QPair<QString,QString>("к" , "c");
    toTranslit << QPair<QString,QString>("ю" , "iu");
    toTranslit << QPair<QString,QString>("а","a");
    toTranslit << QPair<QString,QString>("б","b");
    toTranslit << QPair<QString,QString>("в","v");
    toTranslit << QPair<QString,QString>("д","d");
    toTranslit << QPair<QString,QString>("е","e");
    toTranslit << QPair<QString,QString>("е","io");
    toTranslit << QPair<QString,QString>("ж","j");
    toTranslit << QPair<QString,QString>("з","z");
    toTranslit << QPair<QString,QString>("и","i");
    toTranslit << QPair<QString,QString>("л","l");
    toTranslit << QPair<QString,QString>("м","m");
    toTranslit << QPair<QString,QString>("н","n");
    toTranslit << QPair<QString,QString>("о","o");
    toTranslit << QPair<QString,QString>("п","p");
    toTranslit << QPair<QString,QString>("р","r");
    toTranslit << QPair<QString,QString>("с","s");
    toTranslit << QPair<QString,QString>("т","t");
    toTranslit << QPair<QString,QString>("у","u");
    toTranslit << QPair<QString,QString>("ф","f");
    toTranslit << QPair<QString,QString>("х","h");
    toTranslit << QPair<QString,QString>("ч","ci");
    toTranslit << QPair<QString,QString>("щ","sc");
    toTranslit << QPair<QString,QString>("ъ","");

}

QString Utils::transliterateRuToRo(const QString &name) {
    if ( toTranslit.isEmpty() ) {
        initToTranslit();
    }
QPair<QString,QString> pair;
QString res = name;
foreach ( pair, toTranslit ) {
    QString from = pair.first;
    QString to = pair.second;
    int pos;
    int startPos = 0;
    while ( (pos = res.indexOf(from,startPos,Qt::CaseInsensitive)) != -1 ) {
        QString to1 = to;
        if ( name.at(pos).isUpper() ){
             to1[0] = to1[0].toUpper();
        }
        res.replace(pos, from.length(), to1);
        startPos = pos+to1.length();
    }
}

return res;

}

QString Utils::capitalize(const QString &name)
{
    if ( !name.length()) {
        return name;
    }
    QString tmp = name;
    tmp = tmp.toLower();
    tmp[0] = tmp[0].toUpper();
    return tmp;
}

QString& Utils::convertRoToEn( QString &res) {

    for( int i =0 ;i< res.length(); i++) {
        switch( res[i].unicode()) {
        case 258:
        res[i] = 65;
        break;
        case 194:
        res[i] = 65;
        break;
        case 206:
        res[i] = 73;
        break;
        case 536:
        res[i] = 83;
        break;
        case 538:
        res[i] = 84;
        break;
        case 259:
        res[i] = 97;
        break;
        case 226:
        res[i] = 97;
        break;
        case 238:
        res[i] = 105;
        break;
        case 537:
        res[i] = 115;
        break;
        case 539:
        res[i] = 116;
        break;
        case 350: // Ş неправильная
            res[i] = 'S';

        break;
       case 351: // ş неправильная
            res[i] = 's';

        break;
        case 354: // Ţ неправильная
              res[i] = 'T';

             break;
         case 355: // ţ неправильная
              res[i] = 't';
             break;

        }

    }

    return res;
}

void Utils::normalizeStr(QString &str, uchar stringNumber,QByteArray& acutes) {
    int len = str.length();
    for ( int i =0 ; i< len; i++) {
        QChar c = str[i];
        uchar offset = 0xff;
        QChar to;
        switch( c.unicode()) {
            case 258: // Ă
                to = 65;
                offset = 0;
                break;
            case 259: // ă
                to = 97;
                offset = 1;
                break;
            case 194: // Â
                to = 65;
                offset = 2;
                break;
            case 226: // â
                to = 97;
                offset = 3;
            break;
            case 206: // Î
                to = 73;
                offset = 4;
                break;
            case 238: // î
                to = 105;
                offset = 5;
                break;
            case 536: // Ș
                to = 83;
                offset = 6;
                break;
            case 537: // ș
                to = 115;
                offset = 7;
                break;
            case 350: // Ş неправильная
                to = 'S';
                offset = 6;
            break;
                case 351: // ş неправильная
                to = 's';
                offset = 7;
            break;
            case 538:  // Ț
                to = 84;
                offset = 8;
                break;
            case 539: // ț
                to = 116;
                offset = 9;
                break;
           case 354: // Ţ неправильная
                to = 'T';
                offset = 8;
                break;
            case 355: // ţ неправильная
                to = 't';
                offset = 9;
                break;
            }
        if ( offset != 0xff ) {
            str[i] = to;
            uchar code = offset | (stringNumber<<4);
            acutes.append(code);
            acutes.append(i);
        }
    }
}

void Utils::restoreNormalizedStr(QString &name, QString &firstName, QString &middleName, const QByteArray &acutes )
{
    int i =0;
    int len = acutes.length();

    while ( i < len ) {
        uchar code = acutes.at(i);
        uchar stringNumber = code >> 4;
        uchar c = ((uchar)( code << 4)) >> 4;

        QChar to;

        switch( c) {
            case 0: // Ă
                to = 258;
                break;
            case 1: // ă
                to = 259;
                break;
            case 2: // Â
                to = 194;
                break;
            case 3: // â
                to = 226;
            break;
            case 4: // Î
                to = 206;
                break;
            case 5: // î
                to = 238;
                break;
            case 6: // Ș
                to = 536;
                break;
            case 7: // ș
                to = 537;
                break;

            case 8:  // Ț
                to = 538;

                break;
            case 9: // ț
                to = 539;
                break;
            }
        i++;
        if ( i == len ) {
            break;
        }
        int pos = acutes.at(i);
        switch ( stringNumber ) {
            case 0:
                name[pos] = QChar(to);
                break;
            case 1:
                firstName[pos] = QChar(to);
                break;
            case 2:
                middleName[pos] = QChar(to);
                break;
        }
        i++;

    }
}

void Utils::fixMacOsFocusRect(bool enable)
{
    QWidgetList widgets = /*m*yTopLevelWidget->findChildren<QWidget*>();*/
            qobject_cast<QApplication*>(QCoreApplication::instance())->allWidgets();
    foreach(QWidget* widget, widgets){
        widget->setAttribute( Qt::WA_MacShowFocusRect, enable);
    }
}


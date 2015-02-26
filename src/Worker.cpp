#include "Worker.h"
#include <QString>
#include <QVariant>
#include <QDebug>
#include <QSqlError>
#include <QRegularExpression>
#include "Settings.h"
#include "Utils.h"

Worker::Worker(Settings * settings)
{
    showOldStreetNames = true;
    this->settings = settings;
}

Worker::~Worker()
{

}

void Worker::setDbLanguage(const QString &language)
{
    this->language = language;
}

void Worker::setShowOldStreetNames(bool show )
{
    showOldStreetNames = show;
}

void Worker::doWork( int regionId, int locationId, QString phone, const QString& name,
                     const QString& street,  int houseNumber,    const QString& houseBlock, int apartment,  bool searchCompanies) {

    QString numberWithCode;
    //int number;
    int code = 0;

    if(phone.length() ) {
        if(phone.startsWith("0") ) {
            numberWithCode = phone.mid(1);

        } else if ( phone.startsWith("+373") ) {
            numberWithCode = phone.mid(4);
        } else if ( phone.length() > 6 ) {
            numberWithCode = phone;
        }
    }
    bool isMobile = false;
    if ( numberWithCode.startsWith("22") ) {
        code = 22;
        phone = numberWithCode.mid(2);
    } else if ( numberWithCode.length() >=3 ) {
        if( numberWithCode.length() == 8 && ( numberWithCode.startsWith("7")
                || numberWithCode.startsWith("6")
                || numberWithCode.startsWith("8")
                  || numberWithCode.startsWith("9")
                 || numberWithCode.startsWith("5"))
                ){
            phone = numberWithCode;
            isMobile = true;
        } else {
            code = numberWithCode.left(3).toInt();
             phone = numberWithCode.mid(3);
        }
    }

    bool showStreetNamesInRomanian = settings->value(Settings::ShowStreetNamesInRomanian).toBool();

    QString errorString;
    QSqlQuery query;
    QString firstName;
    QString middleName;
    QString lastName = (name);
    Utils::convertRoToEn(lastName);
    if ( !searchCompanies ) {
        QStringList names = lastName.split(" ");
        lastName = names.first();

        if ( names.count() > 1 ) {
            firstName = names[1];
        }
        if ( names.count() > 2 ) {
            middleName = names[2];
        }
    }
    QString sql = "SELECT '',NULLIF(NULLIF(phone_prefix,1),0) as prefix,phone,name||' '||COALESCE(first_name,'')||' '||COALESCE(middle_name,''),";

    if ( !showStreetNamesInRomanian && language == "ru" ) {
        sql += "(COALESCE(streets.name_"+language+",streets.name_ro)";
    } else {
        sql += "(streets.name_ro";
    }

    if (showOldStreetNames ) {
        sql+=      " || (case when old_name is not null then (' ("+tr("old")+" '||old_name||')') else '' end )";
    }

    sql+=        " ||' '||street_types.name_"+ ((showStreetNamesInRomanian && language == "ru")?"ro":language)+"),";

    sql+=
            " house_number || (case when house_block is null then '' else ('/'||house_block) end),"
            "apartment ";
    if ( searchCompanies || phone.length() ) {
        sql+= " , sector_name, subdivision_name,employee_last_name||' '||COALESCE(employee_first_name,'')||' '||COALESCE(employee_middle_name,''),company_name,company_id";
    } else {
        sql+= " ,'', '','','',''";
    }
    sql += ", COALESCE(locations.name_"+ language +",locations.name_ro),is_company,streets.lat,streets.lon,street_id,address_updated,streets.location_id as street_location_id,"
                                                   " locations.id as location_id,phones.updated,regions.name_"+language
            +", exact_block,location_types.name_"+language+", street_types.name_"+ ((showStreetNamesInRomanian && language == "ru")?"ro":language)+" , name, first_name, middle_name, acutes,op,other,ap_suffix";


    sql+=      " FROM phones "
               " left join streets on phones.street_id=streets.id  "
               " left join locations on coalesce(phones.location_id,streets.location_id)=locations.id "
               " left join regions on locations.region_id=regions.id"
               " left join location_types on locations.location_type=location_types.id"
               " left join street_types on street_type=street_types.id";
    if ( searchCompanies || phone.length() ) {
        sql += " LEFT JOIN companies on company_id = companies.id";
    }
    sql+=                 " WHERE 1=1  ";
    /* QString sql = "SELECT * from phones where 1=1 ";*/
    QMap<QString,QVariant> values;

    if ( phone.length() ) {
        sql += " AND phone= :phoneNumber ";

        //sql += " AND phones.phone="+QString::number(phone);
        //
    }

    if (!isMobile && code ) {
        sql += " AND phone_prefix= :code ";
    }

       QRegularExpression russianRegexp(QString::fromUtf8("^[а-яА-Я\\- ]+$"));
       QString lastNameRussian;
       if ( !lastName.isEmpty() ) {

           if(lastName.contains(russianRegexp) ) {
               lastNameRussian = Utils::transliterateRuToRo(lastName);
           }
           if (  searchCompanies || phone.length() ) {
                sql += " AND  ( phones.name like '%'||:lastName||'%' ";
                if ( !lastNameRussian.isEmpty() ) {
                    sql += " OR  phones.name like '%'||:lastNameRussian||'%'";
                }
                if ( searchCompanies ) {
                    sql += " OR  company_name like '%'||:lastName||'%' ";
                }
                 sql += ") ";

           } else {
                sql += " AND ((phones.name) like :lastName||'%' ";
                if ( !lastNameRussian.isEmpty() ) {
                    sql += " OR phones.name like :lastNameRussian||'%'";
                }
                 sql += ") ";
           }

       }

       QString firstNameRussian;
       QString middleNameRussian;
       if ( !firstName.isEmpty() ) {          
           sql += " AND (phones.first_name like :firstName||'%' ";
           if ( firstName.contains(russianRegexp) ) {
               firstNameRussian = Utils::transliterateRuToRo(firstName);
                sql += " OR phones.first_name like :firstNameRussian||'%'";
           }
           sql += ") ";
       }

       if ( !middleName.isEmpty() ) {
           sql += " AND ( phones.middle_name like :middleName||'%' ";
           if ( middleName.contains(russianRegexp) ) {
               middleNameRussian = Utils::transliterateRuToRo(middleName);
               sql += " OR phones.middle_name like :middleNameRussian||'%'";
           }
            sql += ") ";
       }

       if ( regionId ) {
           if ( regionId==99999 ) {
                sql += " AND ( phones.id >= 1252479  and op = 2 ) ";

           } else if ( phone.isEmpty() || phone.length() == 5  ){
               sql += " AND ( locations.region_id=:regionId ";
               if ( phone.length() == 6) {
                    sql +=  " OR phone_prefix=22 ";
               }
               sql += " ) ";
           }

       }
       if ( !street.isEmpty() ) {
           sql += " AND ( streets.name_ro like '%'||:street||'%' ";
           if ( language == "ru" ) {
               bool showOldStreetNames = settings->value(Settings::ShowOldStreetNames).toBool();
               sql+= " OR  streets.name_ru like '%'||:street||'%' ";
               if ( showOldStreetNames ) {
                sql+= " OR  streets.old_name like '%'||:street||'%' ";
               }

           }
           sql+= ")";
       }

       if ( houseNumber ) {
            sql += " AND phones.house_number=:houseNumber ";
       }
       if ( !houseBlock.isEmpty() ) {
            sql += " AND house_block=:houseBlock ";
       }
       if ( apartment ) {
           sql += " AND apartment=:apartment ";
       }

       if ( phone.isEmpty() ) {
             sql += " AND ( is_company=:isCompany ";
             if (searchCompanies) {
                 sql += " OR company_id is not null ";
             }
             sql+= ")";
       }

	//TODO: искать по дефолтному префиксу региона если location не задан 
       if ( locationId ) {
           sql += " AND COALESCE(phones.location_id,streets.location_id)=:locationId ";

       }
       if ( (regionId == 2 || regionId == 99999 )  && phone.isEmpty() && street.isEmpty() && !houseNumber && houseBlock.isEmpty() &&
            !apartment && lastName.isEmpty() && middleName.isEmpty() && firstName.isEmpty()) {
           if ( searchCompanies )  {
            sql+= " AND phone > 220000 ";
           } else {
                 sql+= " AND updated > 2004 and ( address_updated > 2004 or address_updated is null ) "
                       "AND street_id is not null and exact_block=1 and name!='' ";
           }
       }

      sql+= " LIMIT 100 ";
      bool isCompany = searchCompanies;

       query.prepare(sql );
       query.bindValue(":firstName", firstName);
       query.bindValue(":phoneNumber", phone.toInt());
       query.bindValue(":lastName", lastName.toLower());
       query.bindValue(":middleName", middleName);
       query.bindValue(":regionId", regionId);
       query.bindValue(":locationId", locationId);
       query.bindValue(":street",street);
       query.bindValue(":houseNumber",houseNumber);
       query.bindValue(":houseBlock",houseBlock);
       query.bindValue(":apartment",apartment);
       query.bindValue(":isCompany",(int)searchCompanies);
       query.bindValue(":lastNameRussian",lastNameRussian);
       query.bindValue(":firstNameRussian",firstNameRussian);
       query.bindValue(":middleNameRussian",middleNameRussian);
       query.bindValue(":code",code);


       bool res = query.exec();
       if ( res ) {
           int flag = -1;
              if ( phone.length() ) {
                   bool hasRows = false;
                  while(query.next())
                      {
                      hasRows = true;
                      // OMG
                        int companyId = query.value("company_id").toInt();


                          int isRowCompany = query.value("is_company").toInt();
                          if ( companyId && !isRowCompany) {
                              flag = -1;
                              break;
                          }
                          isRowCompany = isRowCompany || companyId;
                          if ( flag == -1 ) {
                            flag = (int)(isRowCompany);
                          } else if ( flag != isRowCompany  ) {
                               flag = -2;
                          }
                      }
                  /*if (!hasRows  && phone.at(0) == '8' && regionId ) {
                      QSqlQuery query2("SELECT code from regions where id="+QString::number(regionId));
                      query2.exec();
                          if ( query2.first() ) {
                              int code = query2.value("code").toInt();
                              QSqlQuery query3("SELECT * from providers where prefix="+QString::number(code));
                              query3.exec();
                              if ( query3.size() ) {
                                  while(query3.next()){
                                      int numberFrom = query3.value("from").toInt();
                                      int numberTo = query3.value("to").toInt();
                                      if ( phone.toInt() >= numberFrom && phone.toInt() <= numberTo ) {
                                          QString provider = query3.value("name").toString();
                                          errorString = tr("This number could not be found in the database because it belongs to operator %1.").arg(provider);
                                        break;
                                      }

                                  }
                              }
                      }
                  }*/
              }

              if ( flag >= 0 ) {
                  isCompany = (bool)flag;
              }



    }
       emit resultReady(query, isCompany, errorString,regionId == 99999);
}


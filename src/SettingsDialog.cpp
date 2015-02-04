#include "SettingsDialog.h"
#include "ui_settingsdialog.h"
#include <QAbstractButton>
#include <QDir>
#include <QTranslator>
#include "Utils.h"

SettingsDialog::SettingsDialog(QSettings* settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    this->settings = settings;
    ui->setupUi(this);
    setFixedSize(size());
    ui->showOldStreetNamesCheckbox->setChecked(settings->value("showOldStreetNames", false).toBool());
    initialUseSystemStyle = settings->value("useSystemDefaultTheme", false).toBool();
    ui->useSystemDefaultTheme->setChecked(initialUseSystemStyle);

    if ( !initialUseSystemStyle ) {
        foreach ( QAbstractButton* button, ui->buttonBox->buttons()) {
            button->setCursor(Qt::PointingHandCursor);
        }
        foreach ( QObject* child, children()) {
            QCheckBox* checkbox = qobject_cast<QCheckBox*>(child);
            if ( checkbox ) {
                checkbox->setCursor(QCursor(Qt::PointingHandCursor));
            }
        }
    }
    Qt::WindowFlags flags = windowFlags();
    flags = flags & (~Qt::WindowContextHelpButtonHint);
    setWindowFlags(flags);


  QString languagesPath =  QApplication::instance()->property("languagesPath").toString();
  QDir dir(languagesPath);
   QString language =  QApplication::instance()->property("currentLanguage").toString();
   initialLanguage = language;
ui->languageCombobox->addItem("English", "en");
  QStringList fileNames = dir.entryList(QStringList("z1188*.qm"));
   int selectedIndex = 0;
  for (int i = 0; i < fileNames.size(); ++i)
  {
      // get locale extracted by filename
      QString locale;
      locale = fileNames[i];                  // "TranslationExample_de.qm"
      locale.truncate(locale.lastIndexOf('.'));   // "TranslationExample_de"
      locale.remove(0, locale.indexOf('_') + 1);   // "de"

       if ( locale == language ) {
           selectedIndex = i+1;
       }
      QString lang = Utils::capitalize(QLocale(locale).nativeLanguageName());
      ui->languageCombobox->addItem(lang, locale);
  }
   ui->languageCombobox->setCurrentIndex(selectedIndex);
  //neashmeash@gmail.com
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_SettingsDialog_accepted()
{
    settings->setValue("showOldStreetNames", ui->showOldStreetNamesCheckbox->isChecked());
    bool useSystemDefaultTheme = ui->useSystemDefaultTheme->isChecked();
    settings->setValue("useSystemDefaultTheme", useSystemDefaultTheme);
    QApplication * app = qobject_cast<QApplication*>(QApplication::instance());
    QString language = ui->languageCombobox->currentData().toString();
    if ( initialLanguage != language ) {
        settings->setValue("language", language);
        app->setProperty("currentLanguage", language);
        QTranslator *translator = (QTranslator *) app->property("translator").value<void *>();
        QString languagesPath = app->property("languagesPath").toString();
        QString qtLanguagesPath = app->property("qtLanguagesPath").toString();
        QTranslator *qtTranslator = (QTranslator *) app->property("qtTranslator").value<void *>();
        app->removeTranslator(translator);
        app->removeTranslator(qtTranslator);
        if(translator->load("z1188_" + language, languagesPath)) {
            app->installTranslator(translator);
        }
        if (qtTranslator->load("qt_" +language ,qtLanguagesPath) ) {
             app->installTranslator(qtTranslator);
        }

    }

  // load the new translator

    if ( initialUseSystemStyle != useSystemDefaultTheme ) {
        if ( useSystemDefaultTheme ) {
            app->setStyleSheet("");
        } else {
            QFile styleFile( ":/style.css" );
            styleFile.open( QFile::ReadOnly );
            app->setStyleSheet( styleFile.readAll() );
             styleFile.close();
        }
    }
}

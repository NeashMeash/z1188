#include "SettingsDialog.h"
#include "ui_settingsdialog.h"
#include <QAbstractButton>
#include <QDir>
#include <QTranslator>
#include "Utils.h"
#include "AppProperties.h"
#include "Utils.h"
SettingsDialog::SettingsDialog(Settings* settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    this->settings = settings;
    ui->setupUi(this);
    setFixedSize(size());
    ui->showOldStreetNamesCheckbox->setChecked(settings->value(Settings::ShowOldStreetNames).toBool());
    initialUseSystemStyle = settings->value(Settings::UseSystemDefaultTheme).toBool();
    ui->useSystemDefaultTheme->setChecked(initialUseSystemStyle);
    ui->showStreetNamesInRomanianCheckbox->setChecked(settings->value(Settings::ShowStreetNamesInRomanian).toBool());

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


  QString languagesPath =  AppProperties::instance()->languagesPath();
  QDir dir(languagesPath);
   QString language =  AppProperties::instance()->currentLanguage();
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

   if ( language != "ru") {
       ui->showStreetNamesInRomanianCheckbox->hide();
   }

   AppProperties * appProps = AppProperties::instance();
   ui->phoneCountLabel->setText(tr("Total numbers in the database: %1").arg(appProps->totalPhoneCount()));

  //neashmeash@gmail.com
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_SettingsDialog_accepted()
{
    settings->setValue(Settings::ShowOldStreetNames, ui->showOldStreetNamesCheckbox->isChecked());
    bool useSystemDefaultTheme = ui->useSystemDefaultTheme->isChecked();
    settings->setValue(Settings::UseSystemDefaultTheme, useSystemDefaultTheme);
    settings->setValue(Settings::ShowStreetNamesInRomanian, ui->showStreetNamesInRomanianCheckbox->isChecked());
    AppProperties * appProps =  AppProperties::instance();
    QApplication* app = qobject_cast<QApplication*>(QCoreApplication::instance());
    QString language = ui->languageCombobox->currentData().toString();
    if ( initialLanguage != language ) {
        settings->setValue(Settings::Language, language);
        appProps->setCurrentLanguage(language);
        QTranslator *translator = appProps->translator();
        QString languagesPath = appProps->languagesPath();
        QString qtLanguagesPath = appProps->qtLanguagesPath();
        QTranslator *qtTranslator = appProps->qtTranslator();
        app->removeTranslator(translator);
        app->removeTranslator(qtTranslator);
        if(translator->load("z1188_" + language, languagesPath)) {
            app->installTranslator(translator);
            appProps->setTranslator(translator);
        }
        if (qtTranslator->load("qt_" +language ,qtLanguagesPath) ) {
             app->installTranslator(qtTranslator);
             appProps->setQtTranslator(qtTranslator);
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
#ifdef Q_OS_MACX
        Utils::fixMacOsFocusRect(useSystemDefaultTheme);
#endif
    }
}

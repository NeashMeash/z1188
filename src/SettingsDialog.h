#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "Settings.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(Settings *settings, QWidget *parent = 0);
    ~SettingsDialog();

private slots:
    void on_SettingsDialog_accepted();

private:
    Ui::SettingsDialog *ui;
    Settings *settings;
    bool initialUseSystemStyle;
    QString initialLanguage;
};

#endif // SETTINGSDIALOG_H

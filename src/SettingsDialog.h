#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QSettings* settings, QWidget *parent = 0);
    ~SettingsDialog();

private slots:
    void on_SettingsDialog_accepted();

private:
    Ui::SettingsDialog *ui;
    QSettings *settings;
    bool initialUseSystemStyle;
    QString initialLanguage;
};

#endif // SETTINGSDIALOG_H

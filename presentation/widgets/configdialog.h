#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QStringListModel>
#include <QSettings>
#include <QVariant>

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = nullptr);
    ~ConfigDialog();
    void accept();
signals:
    QString setconfig(QString);
protected slots:
    void doubleClickedSelect(const QModelIndex &index);

private:
    Ui::ConfigDialog *ui;
    QStringListModel *model;
    QSettings* settings;
};

#endif // CONFIGDIALOG_H

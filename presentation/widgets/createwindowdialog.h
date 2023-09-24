#ifndef CREATEWINDOWDIALOG_H
#define CREATEWINDOWDIALOG_H

#include <QDialog>

namespace Ui {
class CreateWindowDialog;
}

class CreateWindowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateWindowDialog(QWidget *parent = nullptr);
    ~CreateWindowDialog();
    void accept();
signals:

private:
    Ui::CreateWindowDialog *ui;
};

#endif // CREATEWINDOWDIALOG_H

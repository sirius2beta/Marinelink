#include "createwindowdialog.h"
#include "ui_createwindowdialog.h"

CreateWindowDialog::CreateWindowDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateWindowDialog)
{
    ui->setupUi(this);
    ui->dockTopRadialButton->toggle();
}

CreateWindowDialog::~CreateWindowDialog()
{
    delete ui;
}

void CreateWindowDialog::accept()
{

    QDialog::accept();
}

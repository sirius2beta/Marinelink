#include "configdialog.h"
#include "ui_configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);

    // initialize settings
    QCoreApplication::setOrganizationName("Ezosirius");
    QCoreApplication::setApplicationName("GPlayer_v1");
    settings = new QSettings(this);


    QList<QString> formatlist;
    for(int i=0; i<10; i++){
        if(settings->value(QString("configs/%1").arg(i)) == QVariant()){
            settings->setValue(QString("configs/%1").arg(i), QString("config%1").arg(i));
        }
        formatlist.append(settings->value(QString("configs/%1").arg(i)).toString());
    }

    setWindowTitle("load setting files");

    //configs << "a" << "b" << "c";
    model = new QStringListModel(this);
    model->setStringList(formatlist);
    ui->listView->setModel(model);

   connect(ui->listView,&QListView::doubleClicked,this,&ConfigDialog::doubleClickedSelect);
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::doubleClickedSelect(const QModelIndex &index)
{
    accept();
}

void ConfigDialog::accept()
{
    QList<QString> formatlist = model->stringList();
    for(int i=0; i<10; i++){
        settings->setValue(QString("configs/%1").arg(i), formatlist.at(i));
    }
    emit setconfig(QString::number(ui->listView->currentIndex().row()));
    QDialog::accept();
}

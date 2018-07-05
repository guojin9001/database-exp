#include "about.h"
#include "ui_about.h"

about::about(QString readerid,QString rname,QString sex,QString address,QString amount,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::about)
{
    ui->setupUi(this);
    ui->label_2->setText(readerid);
    ui->label_4->setText(rname);
    ui->label_6->setText(sex);
    ui->label_8->setText(address);
    ui->label_10->setText(amount);
}

about::~about()
{
    delete ui;
}

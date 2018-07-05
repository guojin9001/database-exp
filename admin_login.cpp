#include "admin_login.h"
#include "ui_admin_login.h"
#include<QDebug>
admin_login::admin_login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::admin_login)
{
    ui->setupUi(this);
    ui->pushButton->setToolTip("返回");
    ui->pushButton_2->setToolTip("登录");
    ui->label->setToolTip("username");
    ui->label_2->setToolTip("密码");
}

admin_login::~admin_login()
{
    delete ui;
}

void admin_login::on_pushButton_clicked()
{
    this->hide();
    emit retsig();
}

void admin_login::on_pushButton_2_clicked()
{
    QString id=ui->lineEdit->text();
    QString pw=ui->lineEdit_2->text();
    if(id==""||pw=="")
    {
        QMessageBox::information(NULL,"提示","please input username and password",QMessageBox::Yes,QMessageBox::No);
        return;
    }
    mydatabase data=mydatabase();
    data.openDatabase();
    QSqlDatabase db=data.getConnection();
    QSqlQuery query=QSqlQuery(db);
    query.prepare(QString("select dbo.ISadmin(%1,%2)").arg(id).arg(pw));
    if(query.exec())
    {
        while (query.next()) {
           if(0!=query.value(0))
            {     
                qDebug()<<"ok";
                admin *ad=new admin();
                QString str=QString("welcome admin %1").arg(id);
                ad->setWindowTitle(str);
                ad->show();
                this->hide();
                return;
            }
        }
        {
            QMessageBox::information(NULL,"提示","please input correct username and password",QMessageBox::Yes,QMessageBox::No);
            return;
        }
    }
}

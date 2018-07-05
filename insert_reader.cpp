#include "insert_reader.h"
#include "ui_insert_reader.h"

insert_reader::insert_reader(int type,int rid,QString password, QString rname,QString sex,QString address,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::insert_reader)
{
    this->type=type;
    ui->setupUi(this);
    if(type==0){
        ui->lineEdit->setText(QString::number(rid));
        ui->lineEdit->setFocusPolicy(Qt::NoFocus);
        ui->lineEdit_2->setText(password);
        ui->lineEdit_3->setText(rname);

        ui->lineEdit_5->setText(address);
    }
    this->setWindowTitle("用户管理");
}

insert_reader::~insert_reader()
{
    delete ui;
}

void insert_reader::on_pushButton_2_clicked()
{
    this->close();
}

void insert_reader::on_pushButton_clicked()
{
    int rid=ui->lineEdit->text().toInt();
    QString password=ui->lineEdit_2->text();
    QString rname=ui->lineEdit_3->text();
    QString sex=ui->comboBox->currentText();
    QString address=ui->lineEdit_5->text();
    mydatabase data=mydatabase();
    data.openDatabase();
    QSqlDatabase db=data.getConnection();
    QSqlQuery query(db);
    if(type==0)
    {
        QString str=QString("update reader set rname= '%1',password= '%2',sex= '%3',address= '%4' where readerid=%5 ").arg(rname).arg(password).arg(sex).arg(address).arg(rid);
        qDebug()<<str;
        query.prepare(str);
        if(!query.exec())
        {
            QMessageBox::information(NULL,"提示","修改失败",QMessageBox::Yes,QMessageBox::No);
            this->close();
        }
        else
        {
            QMessageBox::information(NULL,"提示","修改成功",QMessageBox::Yes,QMessageBox::No);
            this->close();
        }

    }
    else
    {
        QString str=QString("select readerid from reader where readerid= %1 ").arg(rid);
        qDebug()<<str;
        query.prepare(str);
        query.exec();
        if(query.next())
        {
            QMessageBox::information(NULL,"提示","the username already exsits",QMessageBox::Yes,QMessageBox::No);
            return;
        }
        str=QString("insert into reader values(%1,'%2','%3','%4','%5')").arg(rid).arg(password).arg(rname).arg(sex).arg(address);
        query.prepare(str);
        if(!query.exec())
        {
            QMessageBox::information(NULL,"提示","插入失败",QMessageBox::Yes,QMessageBox::No);
        }
        else
        {
            QMessageBox::information(NULL,"提示","插入成功",QMessageBox::Yes,QMessageBox::No);
            this->close();
        }
    }
}

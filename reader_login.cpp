#include "reader_login.h"
#include "ui_reader_login.h"

reader_login::reader_login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::reader_login)
{
    ui->setupUi(this);
}

reader_login::~reader_login()
{
    delete ui;
}

void reader_login::on_pushButton_clicked()
{
    this->hide();
    emit retsig();
}

void reader_login::on_pushButton_2_clicked()
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
    query.prepare("select readerid,password from reader");
    if(query.exec())
    {
        while (query.next()) {
           if(id==query.value(0)&&pw==query.value(1).toString())
            {
                reader *ad=new reader(id);
                QString str=QString("welcome reader %1").arg(id);
                ad->setWindowTitle(str);
                ad->show();
                this->hide();
                return;
            }
        }
    }
}

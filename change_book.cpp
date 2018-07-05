#include "change_book.h"
#include "ui_change_book.h"

change_book::change_book(QString bookid,QString book_name,QString author,QString publisher,QDate pubtime,QString cat, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::change_book)
{
    ui->setupUi(this);
    ui->label->setText(bookid);
    ui->lineEdit_2->setText(book_name);
    ui->lineEdit_3->setText(author);
    ui->lineEdit_4->setText(publisher);
    ui->lineEdit_5->setText(cat);
    ui->dateEdit->setDate(pubtime);
}

change_book::~change_book()
{
    delete ui;
}

void change_book::on_pushButton_2_clicked()
{
    this->close();
}

void change_book::on_pushButton_clicked()
{
    QString bookid=ui->label->text();
    QString book_name=ui->lineEdit_2->text();
    QString author=ui->lineEdit_3->text();
    QString publisher=ui->lineEdit_4->text();
    QString pubtime=ui->dateEdit->date().toString("yyyy-MM-dd");
    QString catogary=ui->lineEdit_5->text();


    if(!admin::check_catogary(catogary))
    {
        if(!admin::insert_catogary(catogary)) {
            QMessageBox::information(NULL,"提示","修改失败",QMessageBox::Yes,QMessageBox::No);
            return;
        };
    }
    int catogary_id=admin::find_catogary_id(catogary);

    mydatabase data=mydatabase();
    data.openDatabase();
    QSqlDatabase db=data.getConnection();
    db.transaction();


    QSqlQuery query=QSqlQuery(db);

    QString querystr=QString("update book set bname='%1',author='%2',publisher='%3',pubtime='%4' where bookid='%5'")
            .arg(book_name)
            .arg(author)
            .arg(publisher)
            .arg(pubtime)
            .arg(bookid);
    qDebug()<<querystr;


    if(!query.exec(querystr))
    {
        QMessageBox::information(NULL,"提示","修改失败",QMessageBox::Yes,QMessageBox::No);
        db.rollback();
        return;
    }
    else
    {
        QSqlQuery query1=QSqlQuery(db);
        QString str=QString("update book_cat set catogorid=%1 where bookid='%2'").arg(catogary_id).arg(bookid);
        if(!query1.exec(str))
        {
            db.rollback();
            QMessageBox::information(NULL,"提示","修改失败",QMessageBox::Yes,QMessageBox::No);
        }
        else {
            db.commit();
            QMessageBox::information(NULL,"提示","修改成功",QMessageBox::Yes,QMessageBox::No);
            this->close();
        }
    }

}

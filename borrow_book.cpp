#include "borrow_book.h"
#include "ui_borrow_book.h"

borrow_book::borrow_book(int bookid,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::borrow_book)
{
    this->book_id=bookid;

    ui->setupUi(this);
    ui->lineEdit->setText(QString::number(bookid));
}
borrow_book::borrow_book(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::borrow_book)
{
    ui->setupUi(this);
}
borrow_book::~borrow_book()
{
    delete ui;
}

void borrow_book::on_pushButton_clicked()
{

    QString book_id=ui->lineEdit->text();
    QString rid=ui->lineEdit_2->text();
    if(!checkReader(rid.toInt()))
    {
        QMessageBox::information(NULL,"提示","the rid doesn't exsit",QMessageBox::Yes,QMessageBox::No);
        return;
    }
    if(checkFine(rid.toInt()))
    {
        QMessageBox::information(NULL,"提示","this reader have unpaid fines",QMessageBox::Yes,QMessageBox::No);
        return;
    }
    mydatabase data=mydatabase();
    data.openDatabase();
    QSqlDatabase db=data.getConnection();
    QString querystr="select storeid,amount from book_store where";

    if(book_id==""|| rid=="")
    {
        QMessageBox::information(NULL,"提示","please input bookid and reader id",QMessageBox::Yes,QMessageBox::No);
        return;
    }


    QSqlQuery query3=QSqlQuery(db);
    QString sel_dup_book=QString("select * from borrow where bookid='%1' and readerid= '%2'").arg(book_id).arg(rid);
    query3.exec(sel_dup_book);
    if(query3.next())
    {
        QMessageBox::information(NULL,"提示","you have borrowed this book",QMessageBox::Yes,QMessageBox::No);
          return;
    }


    querystr=querystr+" bookid="+"'" +book_id+"'";
    QSqlQuery query=QSqlQuery(db);
    query.prepare(querystr);
    if(!query.exec())
    {
          QMessageBox::information(NULL,"提示","查找失败",QMessageBox::Yes,QMessageBox::No);
            return;
    }
    while(query.next())
    {
         int storeid=query.value(0).toInt();
         int amount=query.value(1).toInt();

         if(amount>0)
         {             
             QSqlQuery query1=QSqlQuery(db);

             db.transaction();
             query1.prepare("insert into borrow_history(readerid,bookid,btime,btype) values(?,?,?,?)");
             query1.bindValue(0,rid);
             query1.bindValue(1,book_id);
             query1.bindValue(2,QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
             query1.bindValue(3,1);
             if(!query1.exec())
             {
                 QMessageBox::information(NULL,"提示","插入失败",QMessageBox::Yes,QMessageBox::No);
                   return;
             }
             else
             {
                 QSqlQuery query2=QSqlQuery(db);
                 query2.prepare("update book_store set amount=amount-1 where storeid= ? and bookid=? ");
                 query2.addBindValue(storeid);
                 query2.addBindValue(book_id);
                 if(!query2.exec())
                 {
                    QMessageBox::information(NULL,"提示","更新失败",QMessageBox::Yes,QMessageBox::No);
                    db.rollback();
                    return;
                 }
                 else
                 {
                     QSqlQuery query4=QSqlQuery(db);
                     query4.prepare("insert into borrow(readerid,bookid,btime) values(?,?,?)");
                     query4.bindValue(0,rid);
                     query4.bindValue(1,book_id);
                     query4.bindValue(2,QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

                     if(!query4.exec())
                     {
                         QMessageBox::information(NULL,"提示","插入失败",QMessageBox::Yes,QMessageBox::No);
                         db.rollback();
                         return;
                     }

                     QMessageBox::information(NULL,"提示","借书成功",QMessageBox::Yes,QMessageBox::No);
                     db.commit();
                     this->close();
                     return;
                 }

             }
         }

    }
    QMessageBox::information(NULL,"警告","库存不足",QMessageBox::Yes,QMessageBox::No);
}

void borrow_book::on_pushButton_2_clicked()
{
    this->close();
}
bool borrow_book::checkReader(int rid)
{
    mydatabase data=mydatabase();
    data.openDatabase();
    QSqlDatabase db=data.getConnection();

    QString querystr=QString("select * from reader where readerid= %1").arg(rid);

    QSqlQuery query=QSqlQuery(db);
    query.exec(querystr);
    if(query.next())
    {
        return true;
    }
    else return false;
}
bool borrow_book::checkFine(int rid)
{
    mydatabase data=mydatabase();
    data.openDatabase();
    QSqlDatabase db=data.getConnection();

    QString querystr=QString("select * from fine where readerid= %1").arg(rid);

    QSqlQuery query=QSqlQuery(db);
    query.exec(querystr);
    if(query.next())
    {
        return true;
    }
    else return false;
}

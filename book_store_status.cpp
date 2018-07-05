#include "book_store_status.h"
#include "ui_book_store_status.h"

book_store_status::book_store_status(QString bookid,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::book_store_status)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(5);

    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"bookid"<<"bookname"<<"store id"<<"store name"<<"amount");
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    update_book(bookid);
}

book_store_status::~book_store_status()
{
    delete ui;
}
void book_store_status::update_book(QString rid)
{
    ui->tableWidget->setRowCount(0);
    mydatabase data=mydatabase();
    data.openDatabase();
    QSqlDatabase db=data.getConnection();
    QString querystr=QString("select book.bookid,bname,store.storid,sname,amount from book,book_store,store where store.storid=book_store.storeid and book_store.bookid=book.bookid and book.bookid =%1")
            .arg(rid);
    qDebug()<<querystr;
    int rowCount;
    QSqlQuery query=QSqlQuery(db);
    query.prepare(querystr);
    if(!query.exec())
    {
        QMessageBox::information(NULL,"提示","查找失败",QMessageBox::Yes,QMessageBox::No);
        return;
    }
    while(query.next())
    {
        rowCount=ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(rowCount);
        ui->tableWidget->setItem(rowCount,0,new QTableWidgetItem(query.value(0).toString()));
        ui->tableWidget->item(rowCount,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(rowCount,1,new QTableWidgetItem(query.value(1).toString()));
        ui->tableWidget->item(rowCount,1)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(rowCount,2,new QTableWidgetItem(query.value(2).toString()));
        ui->tableWidget->item(rowCount,2)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(rowCount,3,new QTableWidgetItem(query.value(3).toString()));
        ui->tableWidget->item(rowCount,3)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(rowCount,4,new QTableWidgetItem(query.value(4).toString()));
        ui->tableWidget->item(rowCount,4)->setTextAlignment(Qt::AlignCenter);
   }
    QString querystr1=QString("select dbo.hasStore(%1)")
            .arg(rid);
    query.prepare(querystr1);
    if(!query.exec())
    {
        QMessageBox::information(NULL,"提示","查找失败",QMessageBox::Yes,QMessageBox::No);
        return;
    }
    if(query.next())
    {
        ui->label_2->setText("total amount :"+query.value(0).toString());
    }


    QString querystr2=QString("select count(*) from borrow where bookid =%1")
            .arg(rid);
    query.prepare(querystr2);
    if(!query.exec())
    {
        QMessageBox::information(NULL,"提示","查找失败",QMessageBox::Yes,QMessageBox::No);
        return;
    }
    if(query.next())
    {
        ui->label_3->setText("borrowed amount :"+query.value(0).toString());
    }
}

#include "borrow_status.h"
#include "ui_borrow_status.h"

borrow_status::borrow_status(QString rid, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::borrow_status)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(6);

    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"编号"<<"读者id"<<"读者名"<<"bookid"<<"书名"<<"借阅时间");
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    rtn_book=new QAction("还书");

    update_book(rid);
   connect(rtn_book,SIGNAL(triggered(bool)),this,SLOT(return_book()));
}

borrow_status::~borrow_status()
{
    delete ui;
}
void borrow_status::update_book(QString rid)
{
    ui->tableWidget->setRowCount(0);
    mydatabase data=mydatabase();
    data.openDatabase();
    QSqlDatabase db=data.getConnection();
    QString querystr=QString("select bid,borrow.readerid,rname,borrow.bookid,bname,btime from book,borrow,reader where borrow.readerid=reader.readerid and borrow.bookid=book.bookid and borrow.readerid =%1")
            .arg(rid);

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
        ui->tableWidget->setItem(rowCount,5,new QTableWidgetItem(query.value(5).toString().replace('T',' ')));
        ui->tableWidget->item(rowCount,5)->setTextAlignment(Qt::AlignCenter);

   }
}
void borrow_status::return_book()
{
    int row=ui->tableWidget->currentRow();
    QString rid=ui->tableWidget->item(row,1)->text();
    QString bookid=ui->tableWidget->item(row,3)->text();

    mydatabase data=mydatabase();
    data.openDatabase();
    QSqlDatabase db=data.getConnection();
    QString querystr=QString("select storeid,amount from book_store where bookid='%1'").arg(bookid);
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

         if(amount<10)
         {
             QSqlQuery query1=QSqlQuery(db);

             db.transaction();
             query1.prepare("insert into borrow_history(readerid,bookid,btime,btype) values(?,?,?,?)");
             query1.bindValue(0,rid);
             query1.bindValue(1,bookid);
             query1.bindValue(2,QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
             query1.bindValue(3,0);
             if(!query1.exec())
             {
                 QMessageBox::information(NULL,"提示","插入失败",QMessageBox::Yes,QMessageBox::No);
                 db.rollback();
                 return;
             }
             else
             {
                 QSqlQuery query2=QSqlQuery(db);
                 query2.prepare("update book_store set amount=amount+1 where storeid= ? and bookid=? ");
                 query2.addBindValue(storeid);
                 query2.addBindValue(bookid);
                 if(!query2.exec())
                 {
                    QMessageBox::information(NULL,"提示","更新失败",QMessageBox::Yes,QMessageBox::No);
                    db.rollback();
                    return;
                 }
                 else
                 {


                     QSqlQuery query3=QSqlQuery(db);
                     QString delete_borrow=QString("delete from borrow where readerid=%1 and bookid = %2")
                             .arg(rid)
                             .arg(bookid);
                     if(!query3.exec(delete_borrow))
                     {
                         QMessageBox::information(NULL,"提示","删除失败",QMessageBox::Yes,QMessageBox::No);
                         db.rollback();
                         return;
                     }

                     QMessageBox::information(NULL,"提示","删除成功",QMessageBox::Yes,QMessageBox::No);
                     db.commit();
                     this->close();
                 }

             }
         }

    }
}

void borrow_status::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    QTableWidgetItem *currentItem = ui->tableWidget->itemAt( pos );

    if( currentItem == 0)
        return;
    QMenu *treeWidgeMenu = new QMenu(ui->tableWidget);
    treeWidgeMenu->addAction(rtn_book);
    treeWidgeMenu->addSeparator();
    treeWidgeMenu->exec(QCursor::pos());
}

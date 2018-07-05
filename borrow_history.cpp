#include "borrow_history.h"
#include "ui_borrow_history.h"

borrow_history::borrow_history(QString rid,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::borrow_history)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(5);

    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"编号"<<"读者id"<<"读者名"<<"书名"<<"借阅时间");
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableWidget->setColumnWidth(4,300);
    update_book(rid);
}

borrow_history::~borrow_history()
{
    delete ui;
}
void borrow_history::update_book(QString rid)
{
    ui->tableWidget->setRowCount(0);
    mydatabase data=mydatabase();
    data.openDatabase();
    QSqlDatabase db=data.getConnection();
    QString querystr=QString("select bid,borrow_history.readerid,rname,bname,btime from book,borrow_history,reader where borrow_history.readerid=reader.readerid and borrow_history.bookid=book.bookid and borrow_history.btype= 1 and borrow_history.readerid =%1")
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
        ui->tableWidget->setItem(rowCount,4,new QTableWidgetItem(query.value(4).toString().replace('T',' ')));
        ui->tableWidget->item(rowCount,4)->setTextAlignment(Qt::AlignCenter);
   }
}

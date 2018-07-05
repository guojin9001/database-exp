#include "admin.h"
#include "ui_admin.h"

admin::admin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::admin)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(6);
    //ui->tableWidget->setColumnWidth(0,100);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"bookid"<<"bname"<<"author"<<"publisher"<<"publish time"<<"类别");
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);



    ui->tableWidget_2->setColumnCount(5);
    //ui->tableWidget->setColumnWidth(0,100);
    ui->tableWidget_2->setHorizontalHeaderLabels(QStringList()<<"readerid"<<"rname"<<"sex"<<"address"<<"借书总量");
    ui->tableWidget_2->setShowGrid(false);
    ui->tableWidget_2->verticalHeader()->setVisible(false);
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setContextMenuPolicy(Qt::CustomContextMenu);



    ui->tableWidget_3->setColumnCount(5);

    ui->tableWidget_3->setHorizontalHeaderLabels(QStringList()<<"readerid"<<"rname"<<"bookid"<<"bookname"<<"time");
    ui->tableWidget_3->setShowGrid(false);
    ui->tableWidget_3->verticalHeader()->setVisible(false);
    ui->tableWidget_3->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_3->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableWidget_3->setColumnWidth(4,180);


    act=new QAction(" 借书");
    xiugai_rdr=new QAction(" 修改");
    insert_rdr=new QAction(" 插入");
    delete_reader=new QAction("删除");
    view_borrow=new QAction("查询当前借书");
    view_borrow_history=new QAction("查看借书历史");
    delete_book=new QAction("删除");
    xiugai_book=new QAction("修改");
    book_status=new QAction("库存详情");
    rtn_fine=new QAction("缴纳罚款");
    connect(act,SIGNAL(triggered(bool)),this,SLOT(xiugai()));
    connect(xiugai_rdr,SIGNAL(triggered(bool)),this,SLOT(xiugai_reader()));
    connect(insert_rdr,SIGNAL(triggered(bool)),this,SLOT(insert_rder()));
    connect(view_borrow,SIGNAL(triggered(bool)),this,SLOT(vw_bo()));
    connect(view_borrow_history,SIGNAL(triggered(bool)),this,SLOT(vw_bohis()));
    connect(delete_reader,SIGNAL(triggered(bool)),this,SLOT(delete_read()));
    connect(this,SIGNAL(update_reader()),this,SLOT(on_pushButton_4_clicked()));
    connect(this,SIGNAL(update_book()),this,SLOT(on_pushButton_clicked()));
    connect(delete_book,SIGNAL(triggered(bool)),this,SLOT(delete_bok()));
    connect(xiugai_book,SIGNAL(triggered(bool)),this,SLOT(xiugai_bok()));
    connect(book_status,SIGNAL(triggered(bool)),this,SLOT(show_book_status()));
    connect(rtn_fine,SIGNAL(triggered(bool)),this,SLOT(return_fine()));
    update_fine();
}

admin::~admin()
{
    delete ui;
}

void admin::on_pushButton_2_clicked()
{
    QString catogary=ui->lineEdit_5->text();
    if(!check_catogary(catogary))
    {
        if(!insert_catogary(catogary)) {
            QMessageBox::information(NULL,"提示","录入shibai",QMessageBox::Yes,QMessageBox::No);
            return;
        };
    }
    int catogary_id=find_catogary_id(catogary);

    mydatabase data=mydatabase();
    data.openDatabase();
    QSqlDatabase db=data.getConnection();
    db.transaction();


    QSqlQuery query=QSqlQuery(db);

    query.prepare("insert into book(bname,author,publisher,pubtime) values(?,?,?,?)");


    QString bookname=ui->lineEdit_4->text();
    if(bookname=="") {
        QMessageBox::information(NULL,"警告","please input book name",QMessageBox::Yes,QMessageBox::No);
        return;
    }
    query.bindValue(0,bookname);

    QString author=ui->lineEdit_7->text();
    if(author=="")
    {
        QMessageBox::information(NULL,"警告","请输入作者名",QMessageBox::Yes,QMessageBox::No);
        return;
    }
    query.bindValue(1,author);


    QString publisher=ui->lineEdit_3->text();
    if(publisher=="") query.bindValue(2,NULL);
    else query.bindValue(2,publisher);

    QDate pubtime=ui->dateEdit->date();

    query.bindValue(3,pubtime.toString("yyyy-MM-dd"));

    if(!query.exec())
    {
        QMessageBox::information(NULL,"提示","录入shibai",QMessageBox::Yes,QMessageBox::No);
        return;
    }
    else
    {
        int bookid=query.lastInsertId().toInt();
        qDebug()<<bookid;
        QSqlQuery query1=QSqlQuery(db);
        QString str=QString("insert into book_cat values(%1,%2)").arg(bookid).arg(catogary_id);
        if(!query1.exec(str))
        {
            db.rollback();
            QMessageBox::information(NULL,"提示","录入shibai",QMessageBox::Yes,QMessageBox::No);
        }
        else {
            db.commit();
            QMessageBox::information(NULL,"提示","录入成功",QMessageBox::Yes,QMessageBox::No);
        }
    }
}

void admin::on_pushButton_clicked()
{

    ui->tableWidget->setRowCount(0);
    mydatabase data=mydatabase();
    data.openDatabase();
    QSqlDatabase db=data.getConnection();
    QString querystr="select book.bookid,bname,author,publisher,pubtime,cname from (book left outer join book_cat on(book.bookid=book_cat.bookid))left outer join  catogary on(book_cat.catogorid=catogary.catogarid) where";
    QString book_name=ui->book_name->text();
    if(book_name!="") querystr=querystr+"and bname like"+"'" +book_name+"%'";
    QString book_id=ui->book_id->text();
    if(book_id!="") querystr=querystr+"and book.bookid="+"'" +book_id+"'";
    QString author=ui->author->text();
    if(author!="") querystr=querystr+"and author like"+"'" +author+"%'";
    int index=querystr.indexOf("and");
    int rowCount;

    {
        if (index!=-1)
        querystr.remove(index,3);
        else querystr.remove("where");
        QSqlQuery query=QSqlQuery(db);
        query.prepare(querystr);
        if(!query.exec())
        {
            QMessageBox::information(NULL,"提示","查找失败",QMessageBox::Yes,QMessageBox::No);
            return;
        }
        else if(query.size()==0)
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
            ui->tableWidget->setItem(rowCount,5,new QTableWidgetItem(query.value(5).toString()));
            ui->tableWidget->item(rowCount,5)->setTextAlignment(Qt::AlignCenter);
       }
    }
   // else
    //{
    //    QMessageBox::information(NULL,"提示","please input",QMessageBox::Yes,QMessageBox::No);
   // }
}
void admin::xiugai()
{
    int FstRow=ui->tableWidget->rowCount();
    int row=ui->tableWidget->currentRow();
    if(FstRow>0)
    {
        int book_id=ui->tableWidget->item(row,0)->text().toInt();
        borrow_book *borrowbook=new borrow_book(book_id);
        borrowbook->setWindowModality(Qt::ApplicationModal);
        borrowbook->show();
    }
}
void admin::xiugai_reader()
{
    int FstRow=ui->tableWidget_2->rowCount();
    int row=ui->tableWidget_2->currentRow();
    if(FstRow>0)
    {
        int reader_id=ui->tableWidget_2->item(row,0)->text().toInt();
        QString password;
        mydatabase data=mydatabase();
        data.openDatabase();
        QSqlDatabase db=data.getConnection();
        QString querystr=QString("select password from reader where readerid= %1").arg(reader_id);
        QSqlQuery query=QSqlQuery(db);
        query.exec(querystr);
        if(query.next())
        {
            password=query.value(0).toString();
        }

        QString rname=ui->tableWidget_2->item(row,1)->text();
        QString sex=ui->tableWidget_2->item(row,2)->text();
        QString address=ui->tableWidget_2->item(row,3)->text();
        insert_reader *insert=new insert_reader(0,reader_id,password,rname,sex,address);
        insert->setWindowModality(Qt::ApplicationModal);
        insert->show();
    }
}
void admin::insert_rder()
{
        insert_reader *insert=new insert_reader(1,0,NULL,NULL,NULL,NULL);
        insert->setWindowModality(Qt::ApplicationModal);
        insert->show();
}
void admin::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
            QTableWidgetItem *currentItem = ui->tableWidget->itemAt( pos );

            if( currentItem == 0)
                return;

            QMenu *treeWidgeMenu = new QMenu(ui->tableWidget);

            treeWidgeMenu->addAction(act);
            treeWidgeMenu->addAction(xiugai_book);
            treeWidgeMenu->addAction(delete_book);
            treeWidgeMenu->addAction(book_status);
            treeWidgeMenu->addSeparator();
            treeWidgeMenu->exec(QCursor::pos());
}

void admin::on_pushButton_3_clicked()
{
    borrow_book *borrowbook=new borrow_book();
    borrowbook->setWindowModality(Qt::ApplicationModal);
    borrowbook->show();
}

void admin::on_pushButton_4_clicked()
{
    ui->tableWidget_2->setRowCount(0);
    mydatabase data=mydatabase();
    data.openDatabase();
    QSqlDatabase db=data.getConnection();
    QString querystr="select readerid,rname,sex,address from reader where";
    QString reader_id=ui->lineEdit->text();
    if(reader_id!="") querystr=querystr+"and readerid="+"'" +reader_id+"'";
    QString rname=ui->lineEdit_2->text();
    if(rname!="") querystr=querystr+"and rname="+"'" +rname+"'";

    int index=querystr.indexOf("and");
    int rowCount;

    {
        if (index!=-1)
        querystr.remove(index,3);
        else querystr.remove("where");
        QSqlQuery query=QSqlQuery(db);
        query.prepare(querystr);
        if(!query.exec())
        {
            QMessageBox::information(NULL,"提示","查找失败",QMessageBox::Yes,QMessageBox::No);
            return;
        }
        else if(query.size()==0)
        {
            QMessageBox::information(NULL,"提示","查找失败",QMessageBox::Yes,QMessageBox::No);
            return;
        }
        while(query.next())
        {
            rowCount=ui->tableWidget_2->rowCount();
            ui->tableWidget_2->insertRow(rowCount);
            QString rdrid=query.value(0).toString();
            ui->tableWidget_2->setItem(rowCount,0,new QTableWidgetItem(rdrid));

            ui->tableWidget_2->item(rowCount,0)->setTextAlignment(Qt::AlignCenter);

            ui->tableWidget_2->setItem(rowCount,1,new QTableWidgetItem(query.value(1).toString()));
            ui->tableWidget_2->item(rowCount,1)->setTextAlignment(Qt::AlignCenter);

            ui->tableWidget_2->setItem(rowCount,2,new QTableWidgetItem(query.value(2).toString()));
            ui->tableWidget_2->item(rowCount,2)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget_2->setItem(rowCount,3,new QTableWidgetItem(query.value(3).toString()));
            ui->tableWidget_2->item(rowCount,3)->setTextAlignment(Qt::AlignCenter);
            QSqlQuery query2=QSqlQuery(db);
            QString sel_borrow_amount=QString("select count(*) from borrow where readerid =%1").arg(rdrid);
            qDebug()<<sel_borrow_amount;
            query2.exec(sel_borrow_amount);
            if(query2.next())
            {
                ui->tableWidget_2->setItem(rowCount,4,new QTableWidgetItem(query2.value(0).toString()));
                ui->tableWidget_2->item(rowCount,4)->setTextAlignment(Qt::AlignCenter);
            }

       }
    }
   // else
    //{
    //    QMessageBox::information(NULL,"提示","please input",QMessageBox::Yes,QMessageBox::No);
   // }
}

void admin::on_tableWidget_2_customContextMenuRequested(const QPoint &pos)
{
    QTableWidgetItem *currentItem = ui->tableWidget_2->itemAt( pos );

    if( currentItem == 0)
        return;

    QMenu *treeWidgeMenu = new QMenu(ui->tableWidget_2);
    treeWidgeMenu->addAction(xiugai_rdr);
    treeWidgeMenu->addAction(insert_rdr);
    treeWidgeMenu->addAction(delete_reader);
    treeWidgeMenu->addAction(view_borrow);
    treeWidgeMenu->addAction(view_borrow_history);
    treeWidgeMenu->addSeparator();
    treeWidgeMenu->exec(QCursor::pos());
}

void admin::on_pushButton_5_clicked()
{
    insert_reader *insert=new insert_reader(1,0,NULL,NULL,NULL,NULL);
    insert->setWindowModality(Qt::ApplicationModal);
    insert->show();
}
void admin::vw_bo()
{
     int row=ui->tableWidget_2->currentRow();
     QString rid=ui->tableWidget_2->item(row,0)->text();
     borrow_status *borrow=new borrow_status(rid);
     borrow->setWindowModality(Qt::ApplicationModal);
     borrow->show();
}
void admin::vw_bohis()
{
    int row=ui->tableWidget_2->currentRow();
    QString rid=ui->tableWidget_2->item(row,0)->text();
    borrow_history *borrow=new borrow_history(rid);
    borrow->setWindowModality(Qt::ApplicationModal);
    borrow->show();
}
void admin::delete_read()
{
    int FstRow=ui->tableWidget_2->rowCount();
    int row=ui->tableWidget_2->currentRow();
    if(FstRow>0)
    {
        QString reader_id=ui->tableWidget_2->item(row,0)->text();
        if(borrow_book::checkFine(reader_id.toInt()))
        {
            QMessageBox::information(NULL,"提示","the reader have unpaid fine",QMessageBox::Yes,QMessageBox::No);
            return;
        }
        if(check_borrow_book(reader_id))
        {
            QMessageBox::information(NULL,"提示","the reader have borrowed book",QMessageBox::Yes,QMessageBox::No);
            return;
        }
        mydatabase data=mydatabase();
        data.openDatabase();
        QSqlDatabase db=data.getConnection();
        QSqlQuery query=QSqlQuery(db);
        QString querystr=QString("delete from reader where readerid= %1").arg(reader_id);
        if(!query.exec(querystr))
        {
            QMessageBox::information(NULL,"提示","delete error",QMessageBox::Yes,QMessageBox::No);
        }
        else
        {
            QMessageBox::information(NULL,"提示","delete success",QMessageBox::Yes,QMessageBox::No);
            emit update_reader();
        }
    }
}
void admin::delete_bok()
{
    int FstRow=ui->tableWidget->rowCount();
    int row=ui->tableWidget->currentRow();
    if(FstRow>0)
    {
        int book_id=ui->tableWidget->item(row,0)->text().toInt();
        mydatabase data=mydatabase();
        data.openDatabase();
        QSqlDatabase db=data.getConnection();
        db.transaction();
        QSqlQuery query=QSqlQuery(db);
        QString querystr=QString("delete from book where bookid= %1").arg(book_id);
        if(!query.exec(querystr))
        {
            QMessageBox::information(NULL,"提示","delete error",QMessageBox::Yes,QMessageBox::No);
            return;
        }
        else
        {
            QSqlQuery query2=QSqlQuery(db);
            QString querystr2=QString("delete from book_cat where bookid= %1").arg(book_id);
            if(!query2.exec(querystr2))
            {
                QMessageBox::information(NULL,"提示","delete error",QMessageBox::Yes,QMessageBox::No);
                db.rollback();
                return;
            }
            QMessageBox::information(NULL,"提示","delete success",QMessageBox::Yes,QMessageBox::No);
            db.commit();
            emit update_book();
        }
    }
}
bool admin::check_catogary(QString catogary)
{
    mydatabase data=mydatabase();
    data.openDatabase();
    QSqlDatabase db=data.getConnection();
    QSqlQuery query=QSqlQuery(db);
    QString querystr=QString("select * from catogary where cname= '%1' ").arg(catogary);
    query.exec(querystr);
    if(query.next())
    {
        return true;
    }
    else return false;
}
bool admin::insert_catogary(QString catogary)
{
    mydatabase data=mydatabase();
    data.openDatabase();
    QSqlDatabase db=data.getConnection();
    QSqlQuery query=QSqlQuery(db);
    QString querystr=QString("insert into  catogary values('%1')").arg(catogary);
    if(query.exec(querystr))
    {
        return true;
    }
    else return false;
}
int admin::find_catogary_id(QString catogary)
{
    mydatabase data=mydatabase();
    data.openDatabase();
    QSqlDatabase db=data.getConnection();
    QSqlQuery query=QSqlQuery(db);
    QString querystr=QString("select catogarid from catogary where cname='%1'").arg(catogary);
    query.exec(querystr);
    while(query.next())
    {
        return query.value(0).toInt();
    }
    return -1;
}
void admin::xiugai_bok()
{
    int FstRow=ui->tableWidget->rowCount();
    int row=ui->tableWidget->currentRow();
    if(FstRow>0)
    {
        QString bookid=ui->tableWidget->item(row,0)->text();
        QString book_name=ui->tableWidget->item(row,1)->text();
        QString author=ui->tableWidget->item(row,2)->text();
        QString publisher=ui->tableWidget->item(row,3)->text();
        QDate pubtime=QDate::fromString (ui->tableWidget->item(row,4)->text(),"yyyy-MM-dd");
        QString cat=ui->tableWidget->item(row,5)->text();
        change_book *change=new change_book(bookid,book_name,author,publisher,pubtime,cat);
        change->setWindowModality(Qt::ApplicationModal);
        change->show();
    }
}
void admin::show_book_status()
{
    int FstRow=ui->tableWidget->rowCount();
    int row=ui->tableWidget->currentRow();
    if(FstRow>0)
    {
        QString bookid=ui->tableWidget->item(row,0)->text();
        book_store_status *change=new book_store_status(bookid);
        change->setWindowModality(Qt::ApplicationModal);
        change->show();
    }
}
void admin::update_fine()
{
    mydatabase data=mydatabase();
    data.openDatabase();
    QSqlDatabase db=data.getConnection();

    QString querystr="select readerid,bookid from borrow where datediff(mi,btime,getdate())>1 and not exists(select  *from fine where fine.readerid=borrow.readerid and fine.bookid=borrow.bookid)";
    QSqlQuery query=QSqlQuery(db);
    query.prepare(querystr);
    if(!query.exec())
    {
        QMessageBox::information(NULL,"提示","更新罚款失败",QMessageBox::Yes,QMessageBox::No);
        return;
    }
    while(query.next())
    {
        QString readerid=query.value(0).toString();
        QString bookid=query.value(1).toString();
        QSqlQuery query2=QSqlQuery(db);
        query2.prepare("insert into fine(readerid,bookid,ftime) values(?,?,getdate())");
        query2.bindValue(0,readerid);
        query2.bindValue(1,bookid);
        if(query2.exec())
        {
            qDebug()<<"success";
        }
        else
        {
            qDebug()<<query2.executedQuery();
            qDebug()<<query2.lastError();
        }

    }
}

void admin::on_pushButton_6_clicked()
{
    update_fine();
    ui->tableWidget_3->setRowCount(0);
    mydatabase data=mydatabase();
    data.openDatabase();
    QSqlDatabase db=data.getConnection();
    QString querystr="select fine.readerid,rname,fine.bookid,bname,ftime from fine,book,reader where fine.readerid=reader.readerid and book.bookid=fine.bookid";
    QString book_id=ui->lineEdit_6->text();
    if(book_id!="") querystr=querystr+" and fine.bookid=" +book_id;
    QString readerid=ui->lineEdit_8->text();
    if(readerid!="") querystr=querystr+" and reader.readerid=" +readerid;
    int rowCount;

        QSqlQuery query=QSqlQuery(db);
        query.prepare(querystr);
        if(!query.exec())
        {
            QMessageBox::information(NULL,"提示","查找失败",QMessageBox::Yes,QMessageBox::No);
            return;
        }
        else if(query.size()==0)
        {
            QMessageBox::information(NULL,"提示","查找失败",QMessageBox::Yes,QMessageBox::No);
            return;
        }
        while(query.next())
        {
            rowCount=ui->tableWidget_3->rowCount();
            ui->tableWidget_3->insertRow(rowCount);
            ui->tableWidget_3->setItem(rowCount,0,new QTableWidgetItem(query.value(0).toString()));

            ui->tableWidget_3->item(rowCount,0)->setTextAlignment(Qt::AlignCenter);

            ui->tableWidget_3->setItem(rowCount,1,new QTableWidgetItem(query.value(1).toString()));
            ui->tableWidget_3->item(rowCount,1)->setTextAlignment(Qt::AlignCenter);

            ui->tableWidget_3->setItem(rowCount,2,new QTableWidgetItem(query.value(2).toString()));
            ui->tableWidget_3->item(rowCount,2)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget_3->setItem(rowCount,3,new QTableWidgetItem(query.value(3).toString()));
            ui->tableWidget_3->item(rowCount,3)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget_3->setItem(rowCount,4,new QTableWidgetItem(query.value(4).toString().replace('T',' ')));
            ui->tableWidget_3->item(rowCount,4)->setTextAlignment(Qt::AlignCenter);
       }
}
void admin::return_fine()
{
    int FstRow=ui->tableWidget_3->rowCount();
    int row=ui->tableWidget_3->currentRow();
    if(FstRow>0)
    {
        int reader_id=ui->tableWidget_3->item(row,0)->text().toInt();
        int book_id=ui->tableWidget_3->item(row,2)->text().toInt();
        mydatabase data=mydatabase();
        data.openDatabase();
        QSqlDatabase db=data.getConnection();
        db.transaction();
        QSqlQuery query=QSqlQuery(db);
        QString querystr=QString("delete from fine where bookid= %1 and readerid= %2").arg(book_id).arg(reader_id);
        if(!query.exec(querystr))
        {
            QMessageBox::information(NULL,"提示","delete error",QMessageBox::Yes,QMessageBox::No);
            db.rollback();
            return;
        }

        QString querystr2=QString("select storeid,amount from book_store where bookid='%1'").arg(book_id);
        QSqlQuery query2=QSqlQuery(db);
        query2.prepare(querystr2);
        if(!query2.exec())
        {
              QMessageBox::information(NULL,"提示","查找失败",QMessageBox::Yes,QMessageBox::No);
              db.rollback();
              return;
        }
        while(query2.next())
        {
             int storeid=query2.value(0).toInt();
             int amount=query2.value(1).toInt();

             if(amount<10)
             {
                 QSqlQuery query3=QSqlQuery(db);

                 db.transaction();
                 query3.prepare("insert into borrow_history(readerid,bookid,btime,btype) values(?,?,?,?)");
                 query3.bindValue(0,reader_id);
                 query3.bindValue(1,book_id);
                 query3.bindValue(2,QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
                 query3.bindValue(3,0);
                 if(!query3.exec())
                 {
                     QMessageBox::information(NULL,"提示","插入失败",QMessageBox::Yes,QMessageBox::No);
                     db.rollback();
                     return;
                 }
                 else
                 {
                     QSqlQuery query4=QSqlQuery(db);
                     query4.prepare("update book_store set amount=amount+1 where storeid= ? and bookid=? ");
                     query4.addBindValue(storeid);
                     query4.addBindValue(book_id);
                     if(!query4.exec())
                     {
                        QMessageBox::information(NULL,"提示","更新失败",QMessageBox::Yes,QMessageBox::No);
                        db.rollback();
                        return;
                     }
                     else
                     {


                         QSqlQuery query5=QSqlQuery(db);
                         QString delete_borrow=QString("delete from borrow where readerid=%1 and bookid = %2")
                                 .arg(reader_id)
                                 .arg(book_id);
                         if(!query5.exec(delete_borrow))
                         {
                             QMessageBox::information(NULL,"提示","删除失败",QMessageBox::Yes,QMessageBox::No);
                             db.rollback();
                             return;
                         }

                         QMessageBox::information(NULL,"提示","删除成功",QMessageBox::Yes,QMessageBox::No);
                         db.commit();
                     }

                 }
             }

        }
    }
}

void admin::on_tableWidget_3_customContextMenuRequested(const QPoint &pos)
{
    QTableWidgetItem *currentItem = ui->tableWidget_3->itemAt( pos );

    if( currentItem == 0)
        return;

    QMenu *treeWidgeMenu = new QMenu(ui->tableWidget_3);

    treeWidgeMenu->addAction(rtn_fine);

    treeWidgeMenu->addSeparator();
    treeWidgeMenu->exec(QCursor::pos());
}
bool admin::check_borrow_book(QString rid)
{
    mydatabase data=mydatabase();
    data.openDatabase();
    QSqlDatabase db=data.getConnection();
    QSqlQuery query=QSqlQuery(db);
    QString querystr=QString("select * from borrow where readerid= '%1' ").arg(rid);
    query.exec(querystr);
    if(query.next())
    {
        return true;
    }
    else return false;
}

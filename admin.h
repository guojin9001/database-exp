#ifndef ADMIN_H
#define ADMIN_H
#include <QWidget>
#include"mydatabase.h"
#include"borrow_book.h"
#include"insert_reader.h"
#include"change_book.h"
#include<borrow_status.h>
#include<borrow_history.h>
#include"book_store_status.h"
#include<QMenu>
#include<QDebug>
namespace Ui {
class admin;
}

class admin : public QWidget
{
    Q_OBJECT

public:
    explicit admin(QWidget *parent = 0);
    ~admin();
    static bool check_catogary(QString catogary);
    static bool check_borrow_book(QString rid);
    static bool insert_catogary(QString catogary);
    static int find_catogary_id(QString catogary);
private slots:
    void on_pushButton_2_clicked();
    void xiugai();
    void xiugai_reader();
    void insert_rder();
    void vw_bo();
    void vw_bohis();
    void delete_read();
    void delete_bok();
    void xiugai_bok();
    void show_book_status();
    void return_fine();
    void on_pushButton_clicked();

    void update_fine();

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_tableWidget_2_customContextMenuRequested(const QPoint &pos);

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_tableWidget_3_customContextMenuRequested(const QPoint &pos);

private:
    Ui::admin *ui;
    QAction *act;
    QAction *xiugai_rdr;
    QAction *insert_rdr;
    QAction *view_borrow;
    QAction *view_borrow_history;
    QAction *delete_reader;
    QAction *delete_book;
    QAction *xiugai_book;
    QAction *book_status;
    QAction *rtn_fine;
signals:
   void update_reader();
   void update_book();
};

#endif // ADMIN_H

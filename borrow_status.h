#ifndef BORROW_STATUS_H
#define BORROW_STATUS_H

#include <QWidget>
#include<QMenu>
#include<QDebug>
#include<mydatabase.h>
#include"borrow_book.h"
namespace Ui {
class borrow_status;
}

class borrow_status : public QWidget
{
    Q_OBJECT

public:
    explicit borrow_status(QString rid, QWidget *parent = 0);

    ~borrow_status();
private slots:
    void return_book();
    void update_book(QString rid);
    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

private:
    Ui::borrow_status *ui;
    QAction *rtn_book;
};

#endif // BORROW_STATUS_H

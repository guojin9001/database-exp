#ifndef READER_H
#define READER_H

#include <QMainWindow>
#include<QDateTime>
#include"mydatabase.h"
#include"about.h"
#include<QDebug>
#include"borrow_book.h"
namespace Ui {
class reader;
}

class reader : public QMainWindow
{
    Q_OBJECT

public:
    explicit reader(QString rid, QWidget *parent = 0);
    ~reader();
    void update_book_history();
    void update_book();
private slots:
        void on_tableWidget_customContextMenuRequested(const QPoint &pos);
        void return_book();
        void xujie_book();
        void reader_info();
private:
    Ui::reader *ui;
    QString rid;
    QAction *rtn_book;
    QAction *xujie;
};

#endif // READER_H

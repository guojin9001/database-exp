#ifndef BORROW_BOOK_H
#define BORROW_BOOK_H

#include <QWidget>
#include"mydatabase.h"
#include<QDateTime>
namespace Ui {
class borrow_book;
}

class borrow_book : public QWidget
{
    Q_OBJECT
    int book_id;
public:
    explicit borrow_book(int bookid,QWidget *parent = 0);
    explicit borrow_book(QWidget *parent = 0);
    bool checkReader(int rid);
    static bool checkFine(int rid);
    ~borrow_book();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::borrow_book *ui;

};

#endif // BORROW_BOOK_H

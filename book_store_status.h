#ifndef BOOK_STORE_STATUS_H
#define BOOK_STORE_STATUS_H

#include <QWidget>
#include<mydatabase.h>
#include<QDebug>
namespace Ui {
class book_store_status;
}

class book_store_status : public QWidget
{
    Q_OBJECT

public:
    explicit book_store_status(QString bookid, QWidget *parent = 0);
    ~book_store_status();
    void update_book(QString rid);
private:
    Ui::book_store_status *ui;
};

#endif // BOOK_STORE_STATUS_H

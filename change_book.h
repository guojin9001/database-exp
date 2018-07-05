#ifndef CHANGE_BOOK_H
#define CHANGE_BOOK_H

#include <QWidget>
#include<mydatabase.h>
#include<QDebug>
#include<QMessageBox>
#include<admin.h>
namespace Ui {
class change_book;
}

class change_book : public QWidget
{
    Q_OBJECT

public:
    explicit change_book(QString bookid,QString book_name,QString author,QString publisher,QDate pubtime,QString cat, QWidget *parent = 0);
    ~change_book();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::change_book *ui;
};

#endif // CHANGE_BOOK_H

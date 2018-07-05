#ifndef INSERT_READER_H
#define INSERT_READER_H

#include <QWidget>
#include<mydatabase.h>
#include<QDebug>
namespace Ui {
class insert_reader;
}

class insert_reader : public QWidget
{
    Q_OBJECT

public:
    explicit insert_reader(int type, int rid,QString password,QString rname, QString sex, QString address, QWidget *parent = 0);
    ~insert_reader();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::insert_reader *ui;
    int type;
};

#endif // INSERT_READER_H

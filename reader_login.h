#ifndef READER_LOGIN_H
#define READER_LOGIN_H

#include <QWidget>
#include"mydatabase.h"
#include"reader.h"
namespace Ui {
class reader_login;
}

class reader_login : public QWidget
{
    Q_OBJECT

public:
    explicit reader_login(QWidget *parent = 0);
    ~reader_login();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::reader_login *ui;
signals:
    void retsig();
};

#endif // READER_LOGIN_H

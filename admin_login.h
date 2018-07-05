#ifndef ADMIN_LOGIN_H
#define ADMIN_LOGIN_H

#include <QWidget>
#include"mydatabase.h"
#include"admin.h"
#include<QMessageBox>
namespace Ui {
class admin_login;
}

class admin_login : public QWidget
{
    Q_OBJECT

public:
    explicit admin_login(QWidget *parent = 0);
    ~admin_login();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::admin_login *ui;
signals:
    void retsig();

};

#endif // ADMIN_LOGIN_H

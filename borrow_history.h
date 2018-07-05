#ifndef BORROW_HISTORY_H
#define BORROW_HISTORY_H

#include <QWidget>
#include"mydatabase.h"
namespace Ui {
class borrow_history;
}

class borrow_history : public QWidget
{
    Q_OBJECT

public:
    explicit borrow_history(QString rid, QWidget *parent = 0);
    void update_book(QString rid);
    ~borrow_history();

private:
    Ui::borrow_history *ui;
};

#endif // BORROW_HISTORY_H

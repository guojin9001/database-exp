#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    reader=new reader_login();
    reader->setWindowTitle("reader login");
    admin =new admin_login();
    admin->setWindowTitle("admin login");
    connect(reader,SIGNAL(retsig()),this,SLOT(ShowWindow()));
    connect(admin,SIGNAL(retsig()),this,SLOT(ShowWindow()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::ShowWindow()
{
    this->show();
}

void MainWindow::on_pushButton_clicked()
{
    this->hide();
    reader->show();
}

void MainWindow::on_pushButton_2_clicked()
{
    this->hide();
    admin->show();
}
